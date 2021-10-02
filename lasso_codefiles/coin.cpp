#include "coin.h"
#include "lasso.h"
#include <stdlib.h>
void Coin::initCoin() {
  coin_start_x = 30+PLAY_X_START+rand()%(WINDOW_X-PLAY_X_START);
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(218,165,32));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = 30+(rand()%120);
  coin_ax = 0;
  looped = false;
  coin_ay = COIN_G;
  bool paused = false, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, paused, rtheta, true);
}
void Coin::getLooped(){
  looped = true;
}
