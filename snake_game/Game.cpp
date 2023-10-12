#include "Game.h"
#include <Arduino.h>
#include <stdlib.h>
#include <time.h>

Game::Game() {
  led = new LedControl(PIN_LED_DIN, PIN_LED_CLK, PIN_LED_CS);
  led->shutdown(0, false);
  led->setIntensity(0, 8);
  led->clearDisplay(0);

  lcd = new LiquidCrystal_I2C(ADDRESS_LCD, 16, 2);
  lcd->init();
  lcd->backlight();

  pinMode(PIN_LEFT_BTN, INPUT);
  pinMode(PIN_RIGHT_BTN, INPUT);

  srand(time(NULL));

  refreshInput();
  
  gameState = INIT;
}

void Game::initialize() {
  angle = 0;
  bodyCount = 0;
  strcpy(lcdStr[0], "    Snake Game  ");
  strcpy(lcdStr[1], "  Press any Btn ");

  addBody(Coord(MAP_WIDTH / 2, MAP_HEIGHT / 2));
  addBody(Coord(MAP_WIDTH / 2, MAP_HEIGHT / 2 + 1));
  createPoint();
  point = 21;
}

void Game::refreshInput() {
  for(int i=0; i<PIN_SIZE; i++){
    btnState[i] = false;
  }
}

void Game::input() {
  btnState[PIN_LEFT_BTN] |= digitalRead(PIN_LEFT_BTN) == 1;
  btnState[PIN_RIGHT_BTN] |= digitalRead(PIN_RIGHT_BTN) == 1;
}

void Game::update() {
  switch (gameState) {
    case INIT:
      initialize();
      gameState = READY;
    case READY:
      if (isAnyBtnPressed()) {
        gameState = RUN;
      }
      break;
    case RUN:
      // run game logics.
      if (isGameOver()) {
        gameState = GameState::END;
        break;
      }
      if (bodyCoords[0] + directions[angle] == pointCoord) {
        addBody(pointCoord);
        createPoint();
        point++;
      }
      moveBody();
      if (isLeftBtnPressed()) {
        rotateLeft();
      }
      if (isRightBtnPressed()) {
        rotateRight();
      }

      // set lcd texts.
      writePointToLcdStr();
      break;
    case END:
      strcpy(lcdStr[0], "    YOU DIED!   ");
      strcpy(lcdStr[1], "  Press any Btn ");
      if (isAnyBtnPressed()) {
        gameState = INIT;
      }
      break;
  }

}

void Game::moveBody() {
  for (int i = bodyCount - 1; i > 0; i--) {
    bodyCoords[i] = bodyCoords[i - 1];
  }
  bodyCoords[0] += directions[angle];
}

bool Game::isAnyBtnPressed() {
  return isLeftBtnPressed() || isRightBtnPressed();
}

bool Game::isLeftBtnPressed() {
  return btnState[PIN_LEFT_BTN];
}

bool Game::isRightBtnPressed() {
  return btnState[PIN_RIGHT_BTN];
}

void Game::rotateLeft() {
  angle = (angle + 1) % 4;
}

void Game::rotateRight() {
  angle = (angle + 3) % 4;
}

void Game::writePointToLcdStr() {
  strcpy(lcdStr[0], "Point :         ");
  strcpy(lcdStr[1], "                ");

  int pointCopy = point, i = 15;
  while (pointCopy) {
    lcdStr[0][i--] = pointCopy % 10 + '0';
    pointCopy /= 10;
  }
  for (i = 0; i < point / 4; i++) {
    lcdStr[1][i] = '-';
  }
  lcdStr[1][i] = '*';
}

bool Game::isGameOver() {
  for (int i = 0; i < bodyCount; i++) {
    for (int j = i + 1; j < bodyCount; j++) {
      if (bodyCoords[i] == bodyCoords[j]) {
        return true;
      }
    }
  }
  int x = bodyCoords[0].x, y = bodyCoords[0].y;
  if (x < 0 || x >= Game::MAP_WIDTH || y < 0 || y >= Game::MAP_HEIGHT) {
    return true;
  }
  return false;
}

void Game::addBody(Coord coord) {
  for (int i = bodyCount++; i > 0; i--) {
    bodyCoords[i] = bodyCoords[i - 1];
  }
  bodyCoords[0].x = coord.x;
  bodyCoords[0].y = coord.y;
}

void Game::createPoint() {
  int coord[64], size = 0;
  for (int i = 0; i < 64; i++) {
    bool isExist = false;
    for(int j = 0; j < bodyCount; j++) {
      if (i == bodyCoords[j].x + bodyCoords[j].y * MAP_HEIGHT) {
        isExist = true;
      }
    }
    if (isExist) {
      continue;
    }
    coord[size++] = i;
  }
  int randomCoord = rand() % size;
  pointCoord.x = coord[randomCoord] % MAP_HEIGHT;
  pointCoord.y = coord[randomCoord] / MAP_HEIGHT;
}

void Game::render() {
  lcd->setCursor(0, 0);
  lcd->print(lcdStr[0]);
  lcd->setCursor(0, 1);
  lcd->print(lcdStr[1]);

  led->clearDisplay(0);
  for (int i = 0; i < bodyCount; i++) {
    led->setLed(0, bodyCoords[i].y, bodyCoords[i].x, true);
  }
  led->setLed(0, pointCoord.y, pointCoord.x, true);
}

void strcpy(char* dest, char* src) {
  while (*src) {
    *dest++ = *src;
  }
}