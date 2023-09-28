#define __DEBUG__

const int PIN_BTN = 7;
const int PIN_LED_ARRAY[3] = { 13, 12, 8 };
const int PIN_SPEAKER = 4;

const int COUNTER_BIT = 3;
const int COUNTER_MAX_VALUE = 1 << COUNTER_BIT;

const int ARRAY_NOTE_FREQUENCY[8] = { 262, 294, 330, 349, 392, 440, 493, 523};

byte counter = 0;
int prevReadValue = LOW;

void setup() {
#ifdef __DEBUG__
  Serial.begin(9600);
#endif

  for (int i = 0; i < COUNTER_BIT; i++) {
    pinMode(PIN_LED_ARRAY[i], OUTPUT);
  }
  pinMode(PIN_BTN, INPUT);
}

void loop() {
  // increase counter when button released
  int readValue = digitalRead(PIN_BTN);
  int isButtonReleased = prevReadValue == HIGH && readValue == LOW;
  if (isButtonReleased) {
    counter++;
    if (counter == COUNTER_MAX_VALUE) {
      counter = 0;
    }
  }

#ifdef __DEBUG__
  Serial.println(counter);
#endif

  // enlighten LED by counter
  int i = 0;
  while (i < COUNTER_BIT) {
    int status = (1 << i) & counter;
    digitalWrite(PIN_LED_ARRAY[i++], status ? HIGH : LOW);
  }

  // change frequency
  if (isButtonReleased) {
    tone(PIN_SPEAKER, ARRAY_NOTE_FREQUENCY[counter], 1000);
  }

  // update button status 
  // for trigger only released state.
  prevReadValue = readValue;
}
