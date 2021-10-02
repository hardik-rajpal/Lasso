#include <simplecpp>
#include "MovingObject.h"
#include "lasso.h"
#include<stdlib.h>
using namespace simplecpp;

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  if(getYPos()>PLAY_Y_HEIGHT){
    vy = -abs(vy);
  }
  
  if(getYPos()<0){
    vy = abs(vy); 
  }
  if(getXPos()>WINDOW_X){
    vx=-abs(vx);
  }
  if(getXPos()<PLAY_X_START){
    vx=abs(vx);
  }
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;  
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}
void MovingObject::hideObj(){
  for(int i=0;i<parts.size();i++){
    parts[i]->hide();
  }
}
void MovingObject::showObj(){
  for(int i=0;i<parts.size();i++){
    parts[i]->show();
  }
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double &argx, double &argy, double speed, double angle_deg, bool argpaused, bool rtheta, bool randomize_position) {
  if(randomize_position){
    argx = 30+PLAY_X_START+rand()%(WINDOW_X-PLAY_X_START-100);
  }
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; paused = argpaused;
  
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}