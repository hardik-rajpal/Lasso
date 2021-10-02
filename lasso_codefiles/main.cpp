#include<cstdio>
#include<cstdlib>
#include <simplecpp>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
#include "scoin.h"
#include "levels.h"
#include<stdlib.h>
#include<iomanip>
using namespace simplecpp;
main_program {
  initCanvas("Lasso", WINDOW_X, WINDOW_Y); 
  bool game_just_started = true;
  //Level object holds information about current level, player's name, total coins, etc.
  Level Currentlevel(1, INIT_T_LIM, MIN_CTARGET);
  //ask if the player wants a new game or wants to resume a saved one.
  string name;
  bool newgame_bool = Currentlevel.ask_new_or_saved();
  if(newgame_bool){
    name = getPlayerName("");
  }
  Currentlevel.firstInstructions(); 
  //Setting starting values for time variables:
  float stepTime = STEP_TIME;
  float runTime;
  float currTime;
  int coin_target;
  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
 
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  //Draw boundaries around playing field  
  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));
  //Initializing details about the game displays and boxes around them:
  Rectangle bottom_box(PLAY_X_START+100+2*PAD, PLAY_Y_HEIGHT+50, 200, 80);
  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+2*PAD+0.5*textWidth(msg), PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+2*PAD+0.5*textWidth(coinScoreStr), PLAY_Y_HEIGHT+50, coinScoreStr);
  char targetstr[256] = "Coins to collect: ";
  Text targetinfo;//(PLAY_X_START+2*PAD+0.5*textWidth(targetstr), PLAY_Y_HEIGHT + 80, targetstr);

  Rectangle player_box(0.5*PLAY_X_START, 39, PLAY_X_START-2*PAD, 40);
  Text player(2*PAD+0.5*textWidth("Player:"), 28, "Player:");
  Text playername(2*PAD+0.5*textWidth(name), 45, name);  
  
  Rectangle lassos_rem_box(0.5*PLAY_X_START, 138, PLAY_X_START-2*PAD, 50);
  Text lassos_rem_txt_dashboard(2*PAD+0.5*textWidth("Lassos left:"),122, "Lassos left:");
  char lasso_rem_str_dashboard[256] = "This will read number of lassos left after sprintf";
  sprintf(lasso_rem_str_dashboard, "%d", Currentlevel.getNumLassos());
  Text lasso_rem_num_txt(2*PAD+0.5*textWidth(lasso_rem_str_dashboard),153, lasso_rem_str_dashboard);
  
  Rectangle levelbox(0.5*PLAY_X_START, 225, PLAY_X_START-2*PAD, 20);
  char levelnstr[256] = "Level: ";
  sprintf(levelnstr, "Level: %d", Currentlevel.getLNum());
  Text levelinfo(2*PAD+0.5*textWidth(levelnstr), 225, levelnstr);

  Rectangle totalcoins_box(0.5*PLAY_X_START, 311, PLAY_X_START-2*PAD, 40);
  Text total_coins_so_far(2*PAD+0.5*textWidth("Total Coins:"), 300, "Total Coins:");
  char total_coins_so_far_str[256]="This will say total coins after sprintf";
  sprintf(total_coins_so_far_str, "%d", Currentlevel.getTotCoins());
  Text total_coins_so_far_txt(2*PAD+0.5*textWidth(total_coins_so_far_str), 322, total_coins_so_far_str);
  
  Rectangle timeleft_box(0.5*PLAY_X_START, 410, PLAY_X_START-2*PAD, 60);
  char timeleftstr[256];
  sprintf(timeleftstr, "%.2f", runTime-currTime);
  Text timeleft_title(2*PAD + 0.5*textWidth("Time left:"), 400, "Time left:");
  Text timeleft(4*PAD + 0.5*textWidth(timeleftstr), 420, Currentlevel.getTLim());
  //Dynamite graphic design:
  Rectangle r[3];
  int dyn_cx = PLAY_X_START-25, dyn_cy = 410;
  for(int i=0;i<3;i++){
    r[i].show();
    r[i].init(dyn_cx+10*(i-1), dyn_cy, 5, 50);
    r[i].setColor(COLOR(255, 0, 0));
    r[i].setFill();
  }
  Circle clock(dyn_cx, dyn_cy, 12);
  clock.setFill();
  Circle clock2(dyn_cx, dyn_cy, 10);
  clock2.setColor(COLOR(255, 255, 255));
  clock2.setFill();
  int dyn_dx=0, dyn_dy=8; bool flipsign = true;
  Line minute(dyn_cx, dyn_cy, dyn_cx+dyn_dx, dyn_cy+dyn_dy);  
  //Declaring coins and scoins, ie. Special coins:
  paused = false; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = (rand()%180);
  double coin_ax = 0;
  double coin_ay = COIN_G;
  //Two vectors to hold pointers to make handling of coins and scoins easier.
  vector <Scoin*> scoins;
  vector <Coin*> coins;
  //It's easier to do coin1, coin2, coin3, writing code for array coin[3] was not working out easily.
  Coin coin1(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta);
  Coin coin2(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta);
  Coin coin3(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta);
  coins.push_back(&coin1);
  coins.push_back(&coin2);
  coins.push_back(&coin3);
  //Scoins: nscoins is total number of scoins in the game.
  int nscoins = 10;
  Scoin raycoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.25, MYBLUE, CENTERBAR1X, PLAY_Y_HEIGHT + 20);
  Scoin clockcoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.1, MYGREEN, CENTERBAR1X + DISTXBTWBARS, PLAY_Y_HEIGHT + 20);
  Scoin dentropycoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.5, MYGREY, CENTERBAR1X + 2*DISTXBTWBARS+ 10, PLAY_Y_HEIGHT+20);
  Scoin slicecoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.3, MYVIOLET, CENTERBAR1X, PLAY_Y_HEIGHT);
  Scoin magnetscoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.4, MYRED, CENTERBAR1X, PLAY_Y_HEIGHT+40);  
  Scoin twiscoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.0, MYBROWN, CENTERBAR1X, PLAY_Y_HEIGHT);  
  Scoin lifescoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.0, MYDARKGREEN, CENTERBAR1X, PLAY_Y_HEIGHT);
  Scoin bombscoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 0.8, MYBLACK, CENTERBAR1X, PLAY_Y_HEIGHT);
  Scoin scissorscoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.5, MYMAGENTA, CENTERBAR1X+DISTXBTWBARS, PLAY_Y_HEIGHT+40);
  Scoin projecterscoin(coin_speed, 30+rand()%120, coin_ax, coin_ay, paused, rtheta, 1.5, MYPURPLE, CENTERBAR1X+2*DISTXBTWBARS+10, PLAY_Y_HEIGHT+40);
  //The following coins don't last for more than one instant, so no time bars:
  twiscoin.setNoBar();
  lifescoin.setNoBar();
  bombscoin.setNoBar();
  slicecoin.setNoBar();
  scoins.push_back(&raycoin);
  scoins.push_back(&clockcoin);
  scoins.push_back(&dentropycoin);
  scoins.push_back(&slicecoin);
  scoins.push_back(&magnetscoin);
  scoins.push_back(&twiscoin);
  scoins.push_back(&scissorscoin);
  scoins.push_back(&projecterscoin);
  scoins.push_back(&lifescoin);
  scoins.push_back(&bombscoin);
  //alivescoins are the ones that can be activated during the current level. They change every level. ResurrCoin() spawns that coin.
  //Picking new scoins for the next level.
  Currentlevel.reset_coins_and_scoins(scoins, coins);

  for(;;){
    lasso.resetCoinScore();
    if(!game_just_started){
      newgame_bool = Currentlevel.ask_new_or_saved();
      //if the choice is newgame, get the new player's name. The argument "" means there's no history of players yet.
      if(newgame_bool){
        name = getPlayerName(name);
      }
      Currentlevel.firstInstructions();
      Currentlevel.reset_coins_and_scoins(scoins, coins);

    }
    if(newgame_bool){
      //store the name in Level object.
      Currentlevel.setPlayerName(name);
      playername.reset(2*PAD+0.5*textWidth(name), 45, name);
      Currentlevel.setLevelNum(1);
      Currentlevel.setNumLassos(2);
      Currentlevel.setTlim(INIT_T_LIM);
      Currentlevel.setCTarget(MIN_CTARGET);
      sprintf(levelnstr, "Level: 1");
      levelinfo.reset(2*PAD+0.5*textWidth(levelnstr), 225, levelnstr);
      currTime=0;
      runTime=Currentlevel.getTLim();
      coin_target=Currentlevel.getCTarget();
      //time limit and coin target values are by default set for new game.
    } 
    else{
      //if saved game was picked, store the player name loaded from saved game in this main.cpp variable.
      name = Currentlevel.getmyPlayername();
      playername.reset(2*PAD+0.5*textWidth(name), 45, name);
      sprintf(levelnstr, "Level: %d", Currentlevel.getLNum());
      levelinfo.reset(2*PAD+0.5*textWidth(levelnstr), 225, levelnstr);
      currTime=0;
      runTime=Currentlevel.getTLim();
      coin_target=Currentlevel.getCTarget();
    }
    sprintf(total_coins_so_far_str, "%d", Currentlevel.getTotCoins());
    total_coins_so_far_txt.reset(2*PAD+0.5*textWidth(total_coins_so_far_str), 322,total_coins_so_far_str);
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.reset(PLAY_X_START+2*PAD+0.5*textWidth(coinScoreStr), PLAY_Y_HEIGHT+50, coinScoreStr);
    sprintf(lasso_rem_str_dashboard, "%d", Currentlevel.getNumLassos());
    lasso_rem_num_txt.reset(2*PAD+0.5*textWidth(lasso_rem_str_dashboard),153,lasso_rem_str_dashboard);
    sprintf(targetstr, "Coins to collect: %d", coin_target);
    targetinfo.reset(PLAY_X_START+2*PAD+0.5*textWidth(targetstr), PLAY_Y_HEIGHT + 80, targetstr);
    //show details about starting level.
        
    // When w is pressed, throw lasso
    // If lasso within range, make coin stick
    // When z is pressed, turn launch direction clockwise; x is pressed, turn launch direction anti-clockwise.
    //When a is pressed, loop lasso 
    //When d is pressed, yank lasso 
    // When x is pressed, increase speed.
    // When z is pressed, decrease speed.
    // When p is pressed, pause the game.Then: a to abort, p to unpause and s to save the game's progress.
    // When q is pressed, quit
    bool wants_to_abort=false;
    for(;;) {
      game_just_started=false;
      XEvent e;
      bool pendingEv = checkEvent(e);
      if(pendingEv){
        char c = charFromEvent(e);
        msg[msg.length()-1] = c;
        charPressed.reset(PLAY_X_START+2*PAD+0.5*textWidth(msg), PLAY_Y_HEIGHT+20, msg);
        switch(c) {
        case 'w':
          if(lasso.isPaused()){
            if(magnetscoin.isActive()){
              lasso.setLassoRadius(2*LASSO_RADIUS);
            }
            if(scissorscoin.isActive()){
              lasso.setLassoRadius(0.5*LASSO_RADIUS);
            }
            if(raycoin.isActive()){
              lasso.set_ax(0); lasso.set_ay(0);
            }
            lasso.throwloop();
          }
          break;
        case 'd':
          lasso.yank();
          break;
        case 'a':
          lasso.loopit(coins, scoins);
          break;
        case 'z'://I've removed the upper and lower bounds on the angle turned.
          if(lasso.isPaused()){
            lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);
            if(projecterscoin.isActive()){
              if(raycoin.isActive()){
                lasso.set_ax(0); lasso.set_ay(0);
              }
              lasso.showPlot();
            }
          }
          break;
        case 'x':
          if(lasso.isPaused()) {
            lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);
            if(projecterscoin.isActive()){
              if(raycoin.isActive()){
                lasso.set_ax(0); lasso.set_ay(0);
              }
              lasso.showPlot();
            }
          }
          break;
        case 's':
          if(lasso.isPaused()) {
            lasso.addSpeed(-RELEASE_SPEED_STEP);
          }
          break;
        case 'e':
          if(lasso.isPaused()) {
            lasso.addSpeed(+RELEASE_SPEED_STEP);
          }
          break;
          //The next case is written in a block because without the {}, I was getting a compilation error.
        case 'p':{
          //This pauses the game.
          Rectangle pauserect_background(0.5*WINDOW_X, 0.5*WINDOW_Y, 200, 200);
          pauserect_background.setColor(COLOR(255, 255, 255));
          pauserect_background.setFill();
          Text pause_text1(0.5*WINDOW_X, 0.5*WINDOW_Y, "Game Paused. Press:");
          Text pause_text2(0.5*WINDOW_X, 0.5*WINDOW_Y+20, "'p' to unpause.");
          Text pause_text3(0.5*WINDOW_X, 0.5*WINDOW_Y+40, "'s' to save the progress.");
          Text pause_text(0.5*WINDOW_X, 0.5*WINDOW_Y+60, "'a' to abort game");
          //Setting up the loop to receive input from keyboard.
          
          while (true){      
            XEvent k;
            bool pendingEv1 = checkEvent(k);
            if(pendingEv1){
              char pausedin=charFromEvent(k);
              if(pausedin=='p'){
                break;
              }
              else if(pausedin=='a'){
                wants_to_abort = true;
                break;
              }
              else if(pausedin=='s'){
                Currentlevel.savegame();
                break;
              }
              else{
                continue;
              }
            }
          }
        }
        break;
        case 'q':
          exit(0);
        default:
          break;
        }
      }//End of if(pendingEv1)
  //Enuring Scoins do their required tasks if activated.
      if(dentropycoin.isActive()){
        for(int i=0;i<coins.size();i++){
          if(abs(coins[i]->getVX())>0&&!(coins[i]->isLooped())){
            coins[i]->set_vx(0);//makes sure velocity is only verticular.
          }
        }
        for(int i=0;i<scoins.size();i++){
          if(abs(scoins[i]->getVX())>0&&!(scoins[i]->isLooped())){
            scoins[i]->set_vx(0);
          }
        }
      }

      if(slicecoin.isActive()){
        currTime = 0.5*(currTime+runTime);
        sprintf(timeleftstr, "%.2f", runTime-currTime);
        timeleft.setMessage(timeleftstr);
        Text slicedtxt(0.5*WINDOW_X, 0.5*WINDOW_Y, "TIME SLICED!");
        Text click_to_continue_txt(0.5*WINDOW_X, 0.5*WINDOW_Y+30, "Click to Continue.");
        getClick();
        slicecoin.deactivateScoin();
      }
      
      if(twiscoin.isActive()){
        if(2*(runTime-currTime)<999){
          currTime = 2*currTime-runTime;
        }
        else{
          currTime = runTime-999;
        }
        sprintf(timeleftstr, "%.2f", runTime-currTime);
        timeleft.setMessage(timeleftstr);
        Text slicedtxt(0.5*WINDOW_X, 0.5*WINDOW_Y, "TIME TWICED!");
        Text click_to_continue_txt(0.5*WINDOW_X, 0.5*WINDOW_Y+30, "Click to Continue.");
        getClick();
        twiscoin.deactivateScoin();
      }

      if(lifescoin.isActive()){
        Currentlevel.incrementNumLassos();
        sprintf(lasso_rem_str_dashboard, "%d", Currentlevel.getNumLassos());
        lasso_rem_num_txt.reset(2*PAD+0.5*textWidth(lasso_rem_str_dashboard),153,lasso_rem_str_dashboard);
        Text extra_lasso(0.5*WINDOW_X, 0.5*WINDOW_Y, "EXTRA LASSO!");
        Text click_to_continue_txt(0.5*WINDOW_X, 0.5*WINDOW_Y+30, "Click to Continue.");
        getClick();
        lifescoin.deactivateScoin();
      }
      if(bombscoin.isActive()){
        Currentlevel.decrementNumLassos();
        sprintf(lasso_rem_str_dashboard, "%d", Currentlevel.getNumLassos());
        lasso_rem_num_txt.reset(2*PAD+0.5*textWidth(lasso_rem_str_dashboard),153,lasso_rem_str_dashboard);
        Text lasso_burned(0.5*WINDOW_X, 0.5*WINDOW_Y, "You burned your lasso!");
        Text click_to_continue_txt(0.5*WINDOW_X, 0.5*WINDOW_Y+30, "Click to Continue.");
        if(Currentlevel.getNumLassos()<1){
          wants_to_abort=true;
          //Return to homescreen:
        }
        getClick();
        bombscoin.deactivateScoin();
      }
      if(scissorscoin.isActive()&&(!scissorscoin.isOTTDone()&&magnetscoin.isActive())){
        magnetscoin.deactivateScoin();
      }
      if(magnetscoin.isActive()&&(!magnetscoin.isOTTDone())&&scissorscoin.isActive()){
        scissorscoin.deactivateScoin();
      }
  //Next Step Sequences, Making sure objects move and graphics change as currTime increases:
      //for the dynamite graphic:
      next_fig_dyn(dyn_dx, dyn_dy, flipsign);
      minute.reset(dyn_cx, dyn_cy, dyn_cx+dyn_dx, dyn_cy+dyn_dy);
      //Lasso:
      lasso.nextStep(stepTime);
      if(projecterscoin.isActive()&&!(projecterscoin.isOTTDone())){
        lasso.showPlot();
      }
      if((!projecterscoin.isActive())&&lasso.isPlotVisible()){
        lasso.hidePlot();
      }
      //There's a coin that freezes motion, checking if that is active.
      if(clockcoin.isActive()){
        for(int i=0;i<coins.size();i++){
          if(coins[i]->isLooped()){
            coins[i]->nextStep(stepTime);
          }
          coins[i]->nextStep(0);
        }
        for(int i=0;i<Currentlevel.alivescoins.size();i++){
          if(Currentlevel.alivescoins[i]->isLooped()){
            Currentlevel.alivescoins[i]->nextStepScoin(stepTime);
          }
          else{
            Currentlevel.alivescoins[i]->nextStepScoin(0);
          }
        }
      }
      else{
        //Normal nextstep for coins and scoins if clockcoin is not active.
        for(int i=0;i<coins.size();i++){
          coins[i]->nextStep(stepTime);
        }
        for(int i=0;i<Currentlevel.alivescoins.size();i++){
          Currentlevel.alivescoins[i]->nextStepScoin(stepTime);
        }
      }
      currTime += stepTime;
      //Checking if the player has collected enough coins to levelup. We are checking the lasso for coins.
      if(lasso.has_unchecked_coins()){
        lasso.checked_coins();
        Currentlevel.setTotalCoins(lasso.getNumCoins());
        sprintf(total_coins_so_far_str, "%d", Currentlevel.getTotCoins());
        total_coins_so_far_txt.reset(2*PAD+0.5*textWidth(total_coins_so_far_str), 322,total_coins_so_far_str);
        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.reset(PLAY_X_START+2*PAD+0.5*textWidth(coinScoreStr), PLAY_Y_HEIGHT+50, coinScoreStr);
        if(lasso.getNumCoins()>=coin_target){
          //Lasso has enough coins to levelUp, levelUp sequence follows:
          Currentlevel.levelUp(&runTime, &currTime, &lasso);
          sprintf(levelnstr, "Level: %d", Currentlevel.getLNum());
          levelinfo.reset(2*PAD+0.5*textWidth(levelnstr), 225, levelnstr);
          coin_target = Currentlevel.getCTarget();
          sprintf(targetstr, "Coins to collect: %d", coin_target);
          targetinfo.reset(PLAY_X_START+2*PAD+0.5*textWidth(targetstr), PLAY_Y_HEIGHT + 80, targetstr);
          Currentlevel.reset_coins_and_scoins(scoins, coins);
        }
      }
  //RESETTING INFO DISPLAYS HERE ONWARDS:
      
      

      float zero = 0;
      sprintf(timeleftstr, "%.2f", max(runTime-currTime,zero));
      timeleft.setMessage(timeleftstr);
      
      
      //Check if the time-dynamite has gone off.
      if(currTime>=runTime){
        Currentlevel.decrementNumLassos();
        sprintf(lasso_rem_str_dashboard, "%d", Currentlevel.getNumLassos());
        lasso_rem_num_txt.reset(2*PAD+0.5*textWidth(lasso_rem_str_dashboard),153,lasso_rem_str_dashboard);
        Rectangle backdrop_lasso_lost(0.5*WINDOW_X, 0.5*WINDOW_Y, 300, 300);
        backdrop_lasso_lost.setColor(MYWHITE);
        backdrop_lasso_lost.setFill();
        Text lasso_lost_txt(0.5*WINDOW_X, 0.5*WINDOW_Y-20, "You burned your lasso!");
        char lasso_rem_str[256] = "Lassos Remaining:";
        sprintf(lasso_rem_str, "Lassos Remaining: %d", Currentlevel.getNumLassos());
        Text lasso_rem_txt(0.5*WINDOW_X, 0.5*WINDOW_Y, lasso_rem_str);
        Text click_to_continue(0.5*WINDOW_X, 0.5*WINDOW_Y+20, "Click to continue.");
        currTime=0;
        getClick();
        //If all the lassos or lives are used up, the player has failed.
        if(Currentlevel.getNumLassos()<1){
          wants_to_abort=true;
          //Return to homescreen:
        }    
      }
      if(wants_to_abort){
        lasso.yank();
        Currentlevel.fail();
        break;
      }
    } // End for(;;)
  }
  wait(3);
} // End main_program
