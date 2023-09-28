const int COUNTER_BIT = 3;
const int COUNTER_MAX_VALUE = 1 << COUNTER_BIT;

const int PIN_BTN = 7;
const int PIN_LED_ARRAY[COUNTER_BIT] = { 13, 12, 8 };
const int PIN_SPEAKER = 4;

const int ARRAY_NOTE_FREQUENCY[8] = { 262, 294, 330, 349, 392, 440, 493, 523 };

int counter = 0;
int prevReadValue = LOW;
bool isButtonReleased = false;

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
  // update button state
  int readValue = digitalRead(PIN_BTN);
  int isButtonReleased = prevReadValue == HIGH && readValue == LOW;

  // increase counter when button released
  if (isButtonReleased) {
    increaseCounter();
  }
  // update LED state by counter
  updateLEDStateByCounter();
  // play sound only when button released
  // play sound with changed frequency by counter
  if (isButtonReleased) {
    playSoundByCounter();
  }
  // update button state
  // for trigger only released state.
  prevReadValue = readValue;

  // print only debug mode
#ifdef __DEBUG__
  Serial.println(counter);
#endif
}

void increaseCounter() {
  counter++;
  // if counter reached max value,
  // set counter to 0
  if (counter == COUNTER_MAX_VALUE) {
    counter = 0;
  }
}

void updateLEDStateByCounter() {
  int i = 0;
  while (i < COUNTER_BIT) {
    int status = (1 << i) & counter;
    digitalWrite(PIN_LED_ARRAY[i++], status ? HIGH : LOW);
  }
}

void playSoundByCounter() {
  tone(PIN_SPEAKER, ARRAY_NOTE_FREQUENCY[counter], 1000);
}
