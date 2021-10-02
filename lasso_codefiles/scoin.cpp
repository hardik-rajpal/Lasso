#include "scoin.h"
#include "lasso.h"
#include<stdlib.h>
bool floatequal(float float1, float float2){
    return abs(float1-float2)<0.001;
}
void Scoin::initScoin() {
    scoin_start_x = 30+PLAY_X_START+rand()%(WINDOW_X-PLAY_X_START);
    scoin_start_y = PLAY_Y_HEIGHT;
    lifetime = int(100*scoin_sfactor)*STEP_TIME;
    timeleft = lifetime;
}
void Scoin::activateScoin(){
    one_time_task_done=false;
    pause();
    hideObj();
    active = true;
    if(want_bar){
      bar_height = 10;
      bar_width = INIT_BAR_WIDTH;
      outer_bar_width = bar_width + 4;
      outer_bar_height = bar_height + 4;
      outer_bar.reset(cx, cy, outer_bar_width, outer_bar_height);
      outer_bar.setFill();
      bar.reset(cx, cy, bar_width, bar_height);
      bar.setColor(color);
      bar.setFill();
      bar.show();
      outer_bar.show();
    }
}
void Scoin::nextStepScoin(double t){
    one_time_task_done=true;
    nextStep(t);
    if(active && timeleft>0){
      decrementTimeLeft();
    }
    else if(active){
      deactivateScoin();
    }
}
void Scoin::deactivateScoin(){
    active = false;
    task_done=false;
    resetScoin();
    bar.hide();
    outer_bar.hide(); 
}
void Scoin::decrementTimeLeft(){
    timeleft -= STEP_TIME;
    dbw = bar_width - ((bar_width*timeleft)/lifetime);
    bar.reset(cx-(dbw/2), cy, bar_width-dbw, bar_height);
}

void Scoin::resetScoin() {
    showObj();
    looped = false;
    one_time_task_done=false;
    double scoin_speed = COIN_SPEED;
    double scoin_angle_deg = 30+(rand()%120);
    scoin_ax = 0;
    scoin_ay = COIN_G*scoin_sfactor;
    bool paused = false, rtheta = true;
    timeleft = lifetime;
    reset_all(scoin_start_x, scoin_start_y, scoin_speed, scoin_angle_deg, paused, rtheta, true);
}
void Scoin::getLooped(){
    looped=true;
}
void Scoin::killScoin(){
    if(isActive()){deactivateScoin();}
    hideObj();
    pause();
    one_time_task_done=true;
    getLooped();
}
void Scoin::resurrScoin(){
    scoin_circle.reset(scoin_start_x, scoin_start_y, 1.75*COIN_SIZE);
    //the following commands set the scoins graphics, getting the id from the color of the scoin.
    scoin_circle.setColor(WHITE);
    scoin_circle.setFill();
    outer_circle.reset(scoin_start_x, scoin_start_y, 2.5*COIN_SIZE);
    outer_circle.setColor(color);
    outer_circle.setFill(true);
    if(color==MYGREEN){
      t.reset(scoin_start_x, scoin_start_y, "ll");
    }
    else if(color==MYBLUE){
      t.reset(scoin_start_x, scoin_start_y, "->");
    }
    else if(color==MYVIOLET){
      t.reset(scoin_start_x, scoin_start_y, ".5");
    }
    else if(color==MYGREY){
      t.reset(scoin_start_x, scoin_start_y, "Y");
    }
    else if(color==MYRED){
      t.reset(scoin_start_x, scoin_start_y, "m");
    }
    else if(color==MYBROWN){
      t.reset(scoin_start_x, scoin_start_y, "2.");
    }
    else if(color==MYMAGENTA){
      t.reset(scoin_start_x, scoin_start_y, "x");
    }
    else if(color==MYBLACK){
      t.reset(scoin_start_x, scoin_start_y, "o");
      scoin_circle.setColor(MYRED);
    }
    else if(color==MYDARKGREEN){
      t.reset(scoin_start_x, scoin_start_y, "+");
    }
    else if(color==MYPURPLE){
      t.reset(scoin_start_x, scoin_start_y, "s");
    }
    else{
      t.reset(scoin_start_x, scoin_start_y, "U");
    }
    resetScoin();
}