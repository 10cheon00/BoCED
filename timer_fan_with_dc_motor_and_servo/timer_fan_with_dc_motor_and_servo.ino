#include <Servo.h>

#define __DEBUG__

const int PIN_D_RSTN = A0;
const int PIN_TR_BASE = A1;
const int PIN_BTN_TIMER = 4;
const int PIN_BTN_POWER = 7;
const int PIN_LED_POWER = 8;
const int PIN_SERVO = 9;
const int PIN_MOTOR = 10;

Servo timerServo;
const int TIMER_MAX = 10;
const int SERVO_ANGLE_MAX = 180;
const int TIMER_PRESET_COUNT = 3;
const int ARRAY_TIMER_PRESET[TIMER_PRESET_COUNT] = {0, TIMER_MAX / 2, TIMER_MAX };
double timer = 0.0f;
int timerPresetIndex = 0;

bool power = false;

int powerBtnState = 0;
int prevPowerBtnState = 0;

int timerBtnState = 0;
int prevTimerBtnState = 0;

int dynamicResistance = 0;
int motorSpeed = 0;


void setup() {
  pinMode(PIN_BTN_TIMER, INPUT);
  pinMode(PIN_BTN_POWER, INPUT);
  pinMode(PIN_LED_POWER, OUTPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_D_RSTN, INPUT);
  pinMode(PIN_TR_BASE, OUTPUT);

  timerServo.attach(PIN_SERVO);

#ifdef __DEBUG__
  Serial.begin(9600);
#endif
}

void loop() {
  updatePowerBtnState();
  if (isPowerBtnDown()) {
    togglePowerState();
  }

  updateTimerBtnState();
  if (isTimerBtnDown()) {
    changeTimer();
  }
  updateTimerServo();

  if (isPowerOn()) {
    turnOnPowerLED();

    readDynamicResistance();
    mapDynamicResistanceToMotorSpeed();
    updateMotorSpeed();

    decreaseTimer();
    if(isTimerEnd()) {
      togglePowerState();
    }
  
    delay(100);
  }
  else{
    turnOffPowerLED();
    setMotorSpeedZero();
  }
}

void updatePowerBtnState() {
  prevPowerBtnState = powerBtnState;
  powerBtnState = digitalRead(PIN_BTN_POWER);
}

bool isPowerBtnDown() {
  return !prevPowerBtnState && powerBtnState;
}

void togglePowerState() {
  power = !power;
}

bool isPowerOn() {
  return power;
}

void updateTimerBtnState() {
  prevTimerBtnState = timerBtnState;
  timerBtnState = digitalRead(PIN_BTN_TIMER);
}

bool isTimerBtnDown() {
  return !prevTimerBtnState && timerBtnState;
}

void changeTimer() {
  timerPresetIndex++;
  if (timerPresetIndex == TIMER_PRESET_COUNT) {
    timerPresetIndex = 0;
  }
  timer = ARRAY_TIMER_PRESET[timerPresetIndex];
}

void turnOnPowerLED() {
  digitalWrite(PIN_LED_POWER, HIGH);
}

void readDynamicResistance() {
  dynamicResistance = analogRead(PIN_D_RSTN);
}

void mapDynamicResistanceToMotorSpeed() {
  motorSpeed = map(dynamicResistance, 0, 1023, 0, 255);
}

void updateMotorSpeed() {
  analogWrite(PIN_TR_BASE, motorSpeed);
}

void decreaseTimer() {
  if(timer >= 0.1f){
    timer -= 0.1f;
  }
}

void updateTimerServo() {
  timerServo.write((timer / TIMER_MAX) * SERVO_ANGLE_MAX);
}

bool isTimerEnd() {
  return timer < 0.1f;
}

void turnOffPowerLED() {
  digitalWrite(PIN_LED_POWER, LOW);
}

void setMotorSpeedZero() {
  motorSpeed = 0;
  analogWrite(PIN_TR_BASE, motorSpeed);
}