#ifndef __SCOIN_H__
#define __SCOIN_H__

#include "MovingObject.h"

class Scoin : public MovingObject {
  int color;
  double scoin_start_x;
  double scoin_start_y;
  double scoin_sfactor;
  double release_speed;
  double release_angle_deg;
  double scoin_ax;
  double scoin_ay;
  bool active = false;
  bool looped = false;
  bool want_bar = true;
  bool one_time_task_done=false;
  double lifetime;
  double timeleft;
  double bar_width;
  double bar_height;
  double outer_bar_width;
  double outer_bar_height;
  double dbw;
  double cx;
  double cy;
  bool task_done = false;
  // Moving parts
  
  Circle outer_circle;
  Circle scoin_circle;
  Text t;
  
  //When active, bars:
  Rectangle outer_bar;
  Rectangle bar;

 public:
 Scoin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, double argscoin_sfactor, int argcolor, double argcx, double argcy) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    scoin_ax = argax;
    scoin_ay = argay;
    scoin_sfactor = argscoin_sfactor;
    color = argcolor;
    cx = argcx;
    cy = argcy;
    addPart(&scoin_circle);
    addPart(&outer_circle);
    addPart(&t);
    hideObj();
    initScoin(); 
    killScoin();
  }
  bool isOTTDone(){return one_time_task_done;}//some scoins have an instant task. this function returns the associated bool.
  void decrementTimeLeft();//Reduces the width of the bar.
  void activateScoin();//starts the timer and the bar which shows the time left for the scoin's property.
  void deactivateScoin();//resets the scoin in the playing field, the bar-timer disappears.
  void getLooped();//sets the bool looped as true. helpful in checking if it is looped.
  bool isLooped(){return looped;}
  bool isActive(){return active;}
  void nextStepScoin(double t);//similar to nextstep of moving object but it also decreases the length of the bar if the scoin is active.
  void setNoBar(){want_bar=false;}//some coins work for an instant only. no bar required for these coins.
  void initScoin();//initializes the scoin.
  void resetScoin();//resets the scoin.
  void killScoin();//makes the scoin invisible, uncapturable and inactive. So that it's not a part of the current level.
  void resurrScoin();//makes the scoin a part of the current level; makes it visible, capturable, activatable.
}; // End class Coin

#endif
