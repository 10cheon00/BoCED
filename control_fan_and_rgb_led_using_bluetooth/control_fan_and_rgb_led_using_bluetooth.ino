#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define __DEBUG__
#define CAPACITY 256

// Bluetooth variables
SoftwareSerial BTSerial(4, 5);
String inputBuffer;
char messageBuffer[CAPACITY] = { 0 };
int btMessageType = 0;
int btMessageValue = 0;
enum BT_MESSAGE_TYPE {
  FAN_SPEED,
  LED_COLOR,
  LED_LIGHTNESS
};
const String BT_MESSAGE_KEY[3] = {
  "fanSpeed",
  "ledColor",
  "ledLightness"
};

// Arduino pin variables
const uint8_t PIN_FAN = 6;
const uint8_t PIN_LED_RED = 11;
const uint8_t PIN_LED_GREEN = 10;
const uint8_t PIN_LED_BLUE = 9;

// fan variables
const int FAN_SPEED_VALUE[3] = { 0, 127, 255 };
int fanSpeed = 0;

// led variables
uint8_t ledColor[3] = { 0 };
enum COLOR {
  RED,
  GREEN,
  PURPLE
};
const uint8_t LED_COLOR_VALUE[3][3] = {
  { 255, 0, 0 },
  { 0, 255, 0 },
  { 255, 0, 255 }
};
double lightness = 0.0;
const double LED_LIGHTNESS_VALUE[3] = {0.0, 0.5, 1.0};

void setup() {
#ifdef __DEBUG__
  Serial.begin(9600);
  while (!Serial)
    ;
#endif
  BTSerial.begin(9600);

  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  setLEDColor(COLOR::PURPLE);
  setLEDLightness(0);
  updateFanSpeed(0);
  updateLEDColorAndLightness();
}


void loop() {
  if (BTSerial.available()) {
    readBTMessage();
    deserializeBTMessage();
    if (btMessageType == BT_MESSAGE_TYPE::FAN_SPEED) {
      updateFanSpeed(btMessageValue);
    } else if (btMessageType == BT_MESSAGE_TYPE::LED_COLOR) {
      setLEDColor(btMessageValue);
      updateLEDColorAndLightness();
    } else if (btMessageType == BT_MESSAGE_TYPE::LED_LIGHTNESS) {
      setLEDLightness(btMessageValue);
      updateLEDColorAndLightness();
    }
    sendMessage();
  }
}

void readBTMessage() {
  inputBuffer = BTSerial.readString();
}

void deserializeBTMessage() {
  StaticJsonDocument<CAPACITY> doc;
  DeserializationError error = deserializeJson(doc, inputBuffer);

  if (error){
    Serial.println("Json parse error");
    return;
  }
  /*
   * Json Token Format
   * {
   *   "type": // this means BT_MESSAGE_TYPE 
   *   "value": // this value will be applied differently by type
   * }
   */
  btMessageType = doc["type"];
  btMessageValue = doc["value"];
  
  sprintf(messageBuffer, "Set %s to %d", BT_MESSAGE_KEY[btMessageType].c_str(), btMessageValue);
}

void updateFanSpeed(int speedIndex) {
  speedIndex = clamp(speedIndex, 0, 2);
  
  fanSpeed = FAN_SPEED_VALUE[speedIndex];
  Serial.println(fanSpeed);
  analogWrite(PIN_FAN, fanSpeed);
}

void setLEDColor(int colorIndex) {
  colorIndex = clamp(colorIndex, 0, 2);
  
  ledColor[0] = LED_COLOR_VALUE[colorIndex][0];
  ledColor[1] = LED_COLOR_VALUE[colorIndex][1];
  ledColor[2] = LED_COLOR_VALUE[colorIndex][2];
}

void setLEDLightness(int lightnessIndex) {
  lightnessIndex = clamp(lightnessIndex, 0, 2);
  lightness = LED_LIGHTNESS_VALUE[lightnessIndex];
}

int clamp(int value, int low, int high) {
  return max(min(value, high), low);
}

void updateLEDColorAndLightness() {
  double enlightenRed = (double)ledColor[0] * lightness;
  double enlightenGreen = (double)ledColor[1] * lightness;
  double enlightenBlue = (double)ledColor[2] * lightness;
  analogWrite(PIN_LED_RED, enlightenRed);
  analogWrite(PIN_LED_GREEN, enlightenGreen);
  analogWrite(PIN_LED_BLUE, enlightenBlue);
}

void sendMessage() {
#ifdef __DEBUG__
  Serial.println(messageBuffer);
#endif
  BTSerial.write(messageBuffer);
}