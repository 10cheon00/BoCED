#include <SoftwareSerial.h>

#define __DEBUG__

SoftwareSerial BTSerial(4, 5);

const int PIN_PAN = 9;
const int PAN_SPEED[3] = {0, 96, 255};
const char PAN_SPEED_STR[3][7] = {"STOP", "NORMAL", "FAST"};

int choice = 0;

void setup() {
  pinMode(PIN_PAN, OUTPUT);

#ifdef __DEBUG__
  Serial.begin(9600);
#endif
  BTSerial.begin(9600);
}


void loop() {
  if(BTSerial.available()){
    updatePanSpeed();
    sendMessageToMaster();
#ifdef __DEBUG__
  char buf[1024] = {0};
  sprintf(buf, "Your pan speed : %s", PAN_SPEED_STR[choice]);
  Serial.println(PAN_SPEED_STR[choice]);
#endif
  }
}

void updatePanSpeed() {
  choice = (char)BTSerial.read() - '0';
  analogWrite(PIN_PAN, PAN_SPEED[choice]);
}

void sendMessageToMaster() {
  char buf[1024] = {0};
  sprintf(buf, "Your pan speed : %s", PAN_SPEED_STR[choice]);
  BTSerial.write(buf);
}