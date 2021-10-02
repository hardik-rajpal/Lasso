#ifndef __LASSO_H__
#define __LASSO_H__

#include "MovingObject.h"
#include "coin.h"
#include "scoin.h"
#include<vector>
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.18
#define PAD 5
#define PLAY_X_START 140
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)
#define CENTERBAR1X ((PLAY_X_START+WINDOW_X)/2)+10
#define DISTXBTWBARS INIT_BAR_WIDTH + 10
#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH 0.5*LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 100
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 180

#define LASSO_G 30
#define COIN_G 30
#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5
#define INIT_BAR_WIDTH 100
//COLORS
#define MYBLUE COLOR(102, 102, 255)
#define MYGREEN COLOR(0, 204, 0)
#define MYVIOLET COLOR(204, 0, 204)
#define MYPURPLE COLOR(230, 0, 230)
#define MYRED COLOR(255, 51, 51)
#define MYGREY COLOR(127, 127, 127)
#define MYWHITE COLOR(255, 255, 255)
#define MYBROWN COLOR(210, 105, 30)
#define MYBLACK COLOR(0, 0, 0)
#define MYMAGENTA COLOR(255, 51, 152)
#define MYDARKGREEN COLOR(0, 153, 76)
#define MYSKYBLUE COLOR(135,206,235)
#define MYSANDCOLOR COLOR(224,179,124)
class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  Circle plot_points[10];
  double release_angle_deg;
  double lasso_ax;
  int lasso_radius = LASSO_RADIUS;
  double lasso_ay;
  
  
  vector<Coin*> looped_coins;
  vector<Scoin*> looped_scoins;
  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Scoin *the_scoin;
  int num_coins;
  double lasso_vx;
  double lasso_vy;
  //about the plotter:
  bool plot_visible=false;
  bool got_unchecked_coins = false;
  double t_increment = 0.6;
  double x[10], y[10];
  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    for(int i=0;i<10;i++){
      plot_points[i].setColor(MYPURPLE);
    }
    lasso_vx = getVX();
    lasso_vy = getVY();
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  //throwloop to reset the lasso's loop's radius everytime we throw it and unpause it.
  void throwloop();
  //loopit captures coins and scoins in vicinity
  void loopit(const vector<Coin*>& coins, const vector<Scoin*>& scoins);
  void addAngle(double angle_deg);//turns direction of launch.
  void addSpeed(double speed);//adds the to speed of the lasso.
  void resetCoinScore(){num_coins = 0;};//resets the lassos coins.
  void increCoins();//increases the number of coins in the lasso. Was written for cheat codes. Abandoned now.
  double getmyAngle(){return release_angle_deg;}
  bool has_unchecked_coins(){return got_unchecked_coins;}//lets the program know if the lasso has coins that have not been added to the level's coins.
  void checked_coins(){got_unchecked_coins=false;}
  void nextStep(double t);
  void check_for_stuff(const vector<Coin*>& coins, const vector<Scoin*>& scoins);
  void check_for_coin(Coin *coin);
  void check_for_scoin(Scoin *scoin);
  void setLassoRadius(int r);//resets the lassos loop radius.
  int getNumCoins() { return num_coins; }
  void plot(double t, double *xptr, double *yptr);//plot stores the coordinate at time t in xptr, yptr. 
  void showPlot();//shows the trajectory of the lasso.
  void hidePlot();//hides the trajectory of the lasso.
  bool isPlotVisible(){return plot_visible;}//checks if the plot is visible or not.
}; // End class Lasso

#endif
