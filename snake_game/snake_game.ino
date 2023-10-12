#include "Game.h"

/*
1. 플레이어는 가운데에서 생성된다.
2. 생성된 플레이어의 길이는 2다.
3. 플레이어는 항상 앞으로 전진한다.
4. 버튼을 누르면 플레이어의 진행 방향을 전환한다.
5. 시계방향 버튼을 누르면 시계방향으로 90도 회전한다.
6. 반시계방향 버튼을 누르면 반시계방향으로 90도 회전한다.
7. 맵에 생성된 포인트를 획등하면 플레이어의 길이가 1 늘어난다.
8. 플레이어가 맵 밖으로 나가거나 자기 자신과 부딪칠 경우 게임이 종료된다.
*/

Game* game;

void setup() {
  game = new Game();
}

unsigned long elapsedTime = millis();
int frame = 4;

void loop() {
  game->input();

  if (millis() - elapsedTime > (unsigned long)(1000.0 / (double)frame)){
    game->update();

    game->refreshInput();c:\Users\10che\Documents\Arduino\BoCED\snake_game\c_cpp_properties.json
    elapsedTime = millis();
  }
  game->render();

  delay(50);
}