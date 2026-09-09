
#include <LiquidCrystal_I2C.h>

#define TRIG_FRONT 4
#define ECHO_FRONT 10

#define TRIG_RIGHT 9
#define ECHO_RIGHT 11

#define TRIG_LEFT 2
#define ECHO_LEFT A3

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 12
#define IN4 13

#define S2 A0
#define S3 A1
#define COLOR_OUT A2

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum Mode { MODE_MANUAL, MODE_AUTO };
Mode currentMode = MODE_MANUAL;

enum AutoPhase { AUTO_MAZE, AUTO_DONE };
AutoPhase autoPhase = AUTO_MAZE;

int motorSpeed = 180;
int manualTurnSpeed = 160;

const unsigned long COMMAND_TIMEOUT = 700;
unsigned long lastCommandTime = 0;
char lastManualCommand = 'S';

const int FRONT_LIMIT  = 37;
const int SLOW_ZONE    = 45;
const int SIDE_LIMIT   = 10;
const int END_DISTANCE = 80;
const int MIN_PWM      = 110;

int autoSpeed     = 160;
int autoTurnSpeed = 185;
int steerSpeed    = 150;

const int BACKUP_SPEED = 140;
const unsigned long BACKUP_TIME  = 150;
const unsigned long TURN_TIMEOUT = 900;

String savedColors[3];
int colorCount = 0;
String candidateColor = "UNKNOWN";
int stableCount = 0;
bool readyForNextColor = true;
int unknownCount = 0;
unsigned long lastResultChange = 0;
int resultIndex = 0;
unsigned long lastColorScan = 0;
const unsigned long COLOR_SCAN_INTERVAL = 180;

void setMotorSpeed(int speedA, int speedB)
{
  speedA = constrain(speedA, 0, 255);
  speedB = constrain(speedB, 0, 255);

  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);
}

void stopMotors()
{
  setMotorSpeed(0, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveBackward(int speed)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorSpeed(speed, speed);
}

void manualForward()
{
  moveForward(motorSpeed);
  Serial.println("FORWARD");
}

void manualBackward()
{
  moveBackward(motorSpeed);
  Serial.println("BACKWARD");
}

void manualTurnLeft()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorSpeed(manualTurnSpeed, manualTurnSpeed);
  Serial.println("LEFT");
}

void manualTurnRight()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorSpeed(manualTurnSpeed, manualTurnSpeed);
  Serial.println("RIGHT");
}

void moveForward(int speed)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorSpeed(speed, speed);
}

void turnRight(int speed)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorSpeed(speed, speed);
}

void turnLeft(int speed)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorSpeed(speed, speed);
}

void steerRight(int speed)
{
  moveForward(speed);
  analogWrite(ENA, constrain(speed, 0, 255));
  analogWrite(ENB, constrain(speed / 2, MIN_PWM, 255));
}

void steerLeft(int speed)
{
  moveForward(speed);
  analogWrite(ENA, constrain(speed / 2, MIN_PWM, 255));
  analogWrite(ENB, constrain(speed, 0, 255));
}

void backUp()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorSpeed(BACKUP_SPEED, BACKUP_SPEED);
  delay(BACKUP_TIME);

  stopMotors();
  delay(40);
}

long readDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) return 100;

  return duration * 0.034 / 2;
}

long readFilteredDistance(int trigPin, int echoPin)
{
  long readings[3];

  for (int i = 0; i < 3; i++)
  {
    readings[i] = readDistance(trigPin, echoPin);
    delay(5);
  }

  for (int i = 0; i < 2; i++)
  {
    for (int j = i + 1; j < 3; j++)
    {
      if (readings[j] < readings[i])
      {
        long temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }

  return readings[1];
}

void performTurn(bool right)
{
  unsigned long start = millis();

  while (millis() - start < TURN_TIMEOUT)
  {
    if (right) turnRight(autoTurnSpeed);
    else       turnLeft(autoTurnSpeed);

    delay(40);

    long front = readFilteredDistance(TRIG_FRONT, ECHO_FRONT);

    if (front > FRONT_LIMIT + 10) break;
  }

  stopMotors();
  delay(60);
}

void runMazeStep()
{
  long front = readFilteredDistance(TRIG_FRONT, ECHO_FRONT);
  long right = readFilteredDistance(TRIG_RIGHT, ECHO_RIGHT);
  long left  = readFilteredDistance(TRIG_LEFT, ECHO_LEFT);

  Serial.print("F: "); Serial.print(front);
  Serial.print("  R: "); Serial.print(right);
  Serial.print("  L: "); Serial.println(left);

  if (front > END_DISTANCE && right > END_DISTANCE && left > END_DISTANCE)
  {
    stopMotors();
    Serial.println("MAZE FINISHED");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Maze done!");
    delay(500);

    autoPhase = AUTO_DONE;
    return;
  }

  if (front <= FRONT_LIMIT)
  {
    stopMotors();
    delay(80);

    backUp();

    if (left > FRONT_LIMIT)
    {
      Serial.println("TURN LEFT");
      performTurn(false);
    }
    else if (right > FRONT_LIMIT)
    {
      Serial.println("TURN RIGHT");
      performTurn(true);
    }
    else
    {
      Serial.println("NO SAFE DIRECTION - BACKING UP");
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
      setMotorSpeed(140, 140);
      delay(300);
      stopMotors();
    }

    delay(50);
    return;
  }

  if (millis() - lastColorScan >= COLOR_SCAN_INTERVAL)
  {
    lastColorScan = millis();
    checkForColorMarker();
  }

  if (right < SIDE_LIMIT)
  {
    Serial.println("CORRECT LEFT");
    steerLeft(steerSpeed);
    delay(40);
  }
  else if (left < SIDE_LIMIT)
  {
    Serial.println("CORRECT RIGHT");
    steerRight(steerSpeed);
    delay(40);
  }
  else
  {
    int speed = autoSpeed;

    if (front < SLOW_ZONE)
    {
      speed = map(front, FRONT_LIMIT, SLOW_ZONE, MIN_PWM, autoSpeed);
      speed = constrain(speed, MIN_PWM, autoSpeed);
    }

    moveForward(speed);
  }
}

unsigned long readColorChannel(bool s2State, bool s3State)
{
  digitalWrite(S2, s2State);
  digitalWrite(S3, s3State);
  delay(35);

  unsigned long total = 0;
  for (int i = 0; i < 3; i++)
  {
    unsigned long pulse = pulseIn(COLOR_OUT, LOW, 100000);
    if (pulse == 0) return 0;
    total += pulse;
  }
  return total / 3;
}

String detectColor()
{
  unsigned long redFrequency   = readColorChannel(LOW, LOW);
  unsigned long greenFrequency = readColorChannel(HIGH, HIGH);
  unsigned long blueFrequency  = readColorChannel(LOW, HIGH);

  Serial.print("R="); Serial.print(redFrequency);
  Serial.print(" G="); Serial.print(greenFrequency);
  Serial.print(" B="); Serial.print(blueFrequency);

  if (redFrequency == 0 || greenFrequency == 0 || blueFrequency == 0)
  {
    Serial.println(" -> SENSOR ERROR");
    return "UNKNOWN";
  }

  float r = redFrequency;
  float g = greenFrequency;
  float b = blueFrequency;

  String color = "UNKNOWN";

  if (r < g * 0.80 && r < b * 0.80)
  {
    color = "RED";
  }
  else if (g < r * 0.95 && g < b * 1.05)
  {
    color = "GREEN";
  }
  else if (b < r * 0.90 && b < g * 0.92)
  {
    color = "BLUE";
  }

  Serial.print(" -> ");
  Serial.println(color);

  return color;
}

void showCurrentColor(String color)
{
  lcd.setCursor(0, 0);
  lcd.print("Seeing:         ");
  lcd.setCursor(8, 0);
  lcd.print(color);

  lcd.setCursor(0, 1);
  lcd.print("Color ");
  lcd.print(colorCount + 1);
  lcd.print(" of 3       ");
}

void showFinalResults()
{
  if (millis() - lastResultChange < 1500) return;

  lastResultChange = millis();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Result ");
  lcd.print(resultIndex + 1);
  lcd.print(" of 3");

  lcd.setCursor(0, 1);
  lcd.print(savedColors[resultIndex]);

  resultIndex++;
  if (resultIndex >= 3) resultIndex = 0;
}

void checkForColorMarker()
{
  if (colorCount >= 3) return;

  String currentColor = detectColor();
  showCurrentColor(currentColor);

  if (currentColor == "UNKNOWN")
  {
    unknownCount++;
    if (unknownCount >= 3) readyForNextColor = true;

    stableCount = 0;
    candidateColor = "UNKNOWN";
    return;
  }

  unknownCount = 0;

  if (currentColor == candidateColor)
  {
    stableCount++;
  }
  else
  {
    candidateColor = currentColor;
    stableCount = 1;
  }

  if (stableCount >= 2 && readyForNextColor)
  {
    savedColors[colorCount] = currentColor;
    colorCount++;
    readyForNextColor = false;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saved Color ");
    lcd.print(colorCount);
    lcd.setCursor(0, 1);
    lcd.print(currentColor);

    Serial.print("SAVED: ");
    Serial.println(currentColor);
  }
}

void resetAutoState()
{
  autoPhase = AUTO_MAZE;

  colorCount = 0;
  candidateColor = "UNKNOWN";
  stableCount = 0;
  readyForNextColor = true;
  unknownCount = 0;
  resultIndex = 0;
  lastResultChange = 0;
  lastColorScan = 0;
}

void enterManualMode()
{
  currentMode = MODE_MANUAL;
  stopMotors();

  lastManualCommand = 'S';
  lastCommandTime = millis();

  Serial.println("== MANUAL MODE ==");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MANUAL MODE");
}

void enterAutoMode()
{
  currentMode = MODE_AUTO;
  stopMotors();
  resetAutoState();

  Serial.println("== AUTO MODE ==");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AUTO MODE");
  lcd.setCursor(0, 1);
  lcd.print("Maze nav...");
}

void toggleMode()
{
  if (currentMode == MODE_MANUAL) enterAutoMode();
  else                            enterManualMode();
}

void handleIncomingChar(char cmd)
{
  if (cmd == '\n' || cmd == '\r' || cmd == ' ') return;

  if (cmd >= 'a' && cmd <= 'z') cmd = cmd - 'a' + 'A';

  switch (cmd)
  {
    case 'A': enterAutoMode();  return;
    case 'M': enterManualMode(); return;
    case 'T': toggleMode();     return;
  }

  switch (cmd)
  {
    case 'F':
      if (currentMode != MODE_MANUAL) enterManualMode();
      manualForward();
      break;

    case 'B':
      if (currentMode != MODE_MANUAL) enterManualMode();
      manualBackward();
      break;

    case 'L':
      if (currentMode != MODE_MANUAL) enterManualMode();
      manualTurnLeft();
      break;

    case 'R':
      if (currentMode != MODE_MANUAL) enterManualMode();
      manualTurnRight();
      break;

    case 'S':
      if (currentMode != MODE_MANUAL) enterManualMode();
      stopMotors();
      Serial.println("STOP");
      break;

    default:
      Serial.print("Unknown command: ");
      Serial.println(cmd);
      return;
  }

  lastManualCommand = cmd;
  lastCommandTime = millis();
}

void setup()
{
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pinMode(ECHO_LEFT, INPUT);

  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);

  stopMotors();

  Wire.begin();
  lcd.init();
  lcd.backlight();

  enterManualMode();

  Serial.println("==============================");
  Serial.println("ROBOT READY");
  Serial.println("F/B/L/R/S = manual drive");
  Serial.println("A = auto mode, M = manual mode, T = toggle");
  Serial.println("==============================");
}

void loop()
{
  while (Serial.available())
  {
    handleIncomingChar((char)Serial.read());
  }

  if (currentMode == MODE_MANUAL)
  {
    if (lastManualCommand != 'S' && millis() - lastCommandTime > COMMAND_TIMEOUT)
    {
      stopMotors();
      lastManualCommand = 'S';
      Serial.println("TIMEOUT - STOP");
    }
  }
  else
  {
    if (autoPhase == AUTO_MAZE)
    {
      runMazeStep();
    }
    else
    {
      showFinalResults();
    }
  }
}
