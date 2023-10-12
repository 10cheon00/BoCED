#ifndef __GAME__
#define __GAME__

#include "Coord.h"

#include "LiquidCrystal_I2C.h"
#include "LedControl.h"

class Game {
public:
  Game();
  void initialize();
  void refreshInput();
  void input();
  void update();
  void render();

private:
  bool isAnyBtnPressed();
  bool isLeftBtnPressed();
  bool isRightBtnPressed();
  bool isGameOver();
  void addBody(Coord coord);
  void createPoint();
  void moveBody();
  void rotateLeft();
  void rotateRight();
  void writePointToLcdStr();

private:
  static const int MAP_WIDTH = 8;
  static const int MAP_HEIGHT = 8;

  enum GameState { INIT, READY, RUN, END };
  GameState gameState;

  Coord bodyCoords[64];
  int bodyCount = 0;

  Coord directions[4] = {
      {0, -1}, // UP
      {1, 0},  // RIGHT
      {0, 1},  // DOWN
      {-1, 0}  // LEFT
  };
  int angle;

  Coord pointCoord;
  int point;  
private:
  LiquidCrystal_I2C* lcd;
  LedControl* led;
  const static int PIN_SIZE = 14;
  const int PIN_LED_DIN = 12;
  const int PIN_LED_CS = 11;
  const int PIN_LED_CLK = 10;
  const uint8_t ADDRESS_LCD = 0x27u;
  const int PIN_LEFT_BTN = 8;
  const int PIN_RIGHT_BTN = 7;
  bool btnState[PIN_SIZE];
  char lcdStr[2][17];
};

#endif