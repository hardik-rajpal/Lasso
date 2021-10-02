#include <simplecpp>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include "scoin.h"
#include<vector>
using namespace simplecpp;

void Lasso::draw_lasso_band() {
  double len = LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(255, 0, 0));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(165, 42, 42));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_scoin = NULL;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(165,42,42));

  lasso_band.setColor(COLOR(138, 43, 226));
  draw_lasso_band();

} // End Lasso::initLasso()
void Lasso::plot(double t, double *xptr, double *yptr){
  
  *xptr = lasso_start_x+lasso_vx*t;
  *yptr = lasso_start_y+lasso_vy*t+0.5*lasso_ay*t*t;
}
void Lasso::hidePlot(){
  for(int i=0;i<10;i++){
    plot_points[i].hide();
  }
  plot_visible=false;
}
void Lasso::showPlot(){
  
  lasso_ax = MovingObject::getAx();
  lasso_ay = MovingObject::getAy();
  hidePlot();
  double t = 0;
  x[0]=getXPos();
  y[0]=getYPos();
  int coordinate_index=0;
  bool in_playing_field=true;
  while(in_playing_field&&coordinate_index<10){
    plot(t, &x[coordinate_index], &y[coordinate_index]);
    t+=t_increment;
    in_playing_field = (x[coordinate_index]>PLAY_X_START&&x[coordinate_index]<WINDOW_X)&&(y[coordinate_index]>0&&y[coordinate_index]<PLAY_Y_HEIGHT);
    coordinate_index++;
  }
  for(int i=coordinate_index-2;i>-1;i--){
    plot_points[i].reset(x[i], y[i], 5);
    plot_points[i].show();
  }
  plot_visible=true;
}

void Lasso::yank(){
  //This bool got_checked_coins is to let the Level object collect coins from the lasso.
  got_unchecked_coins=true;
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, paused, rtheta, false);
  setLassoRadius(LASSO_RADIUS);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
  if(looped_coins.size()!=0) {
    num_coins+=looped_coins.size();
    for(int i=0; i<looped_coins.size();i++){
      looped_coins[i]->resetCoin();
    }
    looped_coins.clear();
  }
  if(looped_scoins.size()>0) {
    for(int i=0;i<looped_scoins.size();i++){
      looped_scoins[i]->activateScoin();
    }
    looped_scoins.clear();
  }
  MovingObject::set_ay(LASSO_G);
} // End Lasso::yank()
void Lasso::loopit(const vector<Coin*>& coins, const vector<Scoin*>& scoins) {
  if(lasso_looped) {return;} // Already looped
  //check_for_stuff checks collectively if coins and scoins are in the lasso's range.
  check_for_stuff(coins, scoins);
  lasso_loop.reset(getXPos(), getYPos(), COIN_SIZE);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  lasso_vy = -release_speed*sine(release_angle_deg);
  lasso_vx = release_speed*cosine(release_angle_deg);
  bool paused = true, rtheta = true;
  draw_lasso_band();
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, paused, rtheta, false);
} // End Lasso::addAngle()
void Lasso::setLassoRadius(int r){
  lasso_radius = r;
}
void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, paused, rtheta, false);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  MovingObject::nextStep(stepTime);
  if(getYPos()>PLAY_Y_HEIGHT||getYPos()<0) { yank();}
  if(getXPos()>WINDOW_X||getXPos()<PLAY_X_START) { yank();}
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()
void Lasso::throwloop(){
  lasso_looped = false;
  lasso_loop.reset(getXPos(), getYPos(), lasso_radius);
  lasso_loop.setFill(false);
  unpause();
}
void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= lasso_radius&&!coinPtr->isLooped()) {
    looped_coins.push_back(coinPtr);
    coinPtr->getAttachedTo(this);
    coinPtr->getLooped();
  }
} // End Lasso::check_for_coin()
void Lasso::check_for_stuff(const vector<Coin*>& coins, const vector<Scoin*>& scoins){
  for(int i=0; i<coins.size();i++){
    check_for_coin(coins[i]);
  }
  for(int i=0;i<scoins.size();i++){
    check_for_scoin(scoins[i]);
  }
}
void Lasso::check_for_scoin(Scoin *scoinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double scoin_x = scoinPtr->getXPos();
  double scoin_y = scoinPtr->getYPos();
  double xdiff = (lasso_x - scoin_x);
  double ydiff = (lasso_y - scoin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= lasso_radius&&!scoinPtr->isLooped()) {
     looped_scoins.push_back(scoinPtr);
     scoinPtr->getAttachedTo(this);
     scoinPtr->getLooped();
  }
}
void Lasso::increCoins(){
  num_coins++;
}
