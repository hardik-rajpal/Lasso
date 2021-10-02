#define INIT_T_LIM 40
#define MIN_CTARGET 3
#include<simplecpp>
#include"lasso.h"
#include<string>
#include<cmath>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<fstream>
class Level{
    int lnumber;
    float init_time_limit;
    int coin_target;
    int num_lassos = 2;
    int total_coins=0;
    int total_coins_till_last_level=0;
    float coinspersec;
    string player_name;
    public:
    vector<Scoin*> alivescoins;
    Level(int arglnumber, float argtime_limit, int argcoin_target){
        lnumber = arglnumber;
        init_time_limit = argtime_limit;
        coin_target = argcoin_target;
        coinspersec = coin_target/init_time_limit;
    }
    float getTLim(){return init_time_limit;}
    int getLNum(){return lnumber;}
    int getCTarget(){return coin_target;}
    int getTotCoins(){return total_coins;}
    void incrementNumLassos(){num_lassos++;}
    string getmyPlayername(){return player_name;}
    void setLevelNum(int n){lnumber = n;}
    void decrementNumLassos(){num_lassos--;}
    int getNumLassos(){return num_lassos;}
    void setNumLassos(int n){num_lassos=n;}
    void setPlayerName(string name){player_name=name;}
    //TCTLL = total_coins_till_last_level
    void setTCTLL(int tctll){total_coins_till_last_level = tctll;}
    void setTotalCoins(int coinsinthislevel){total_coins=total_coins_till_last_level+coinsinthislevel;}
    void setTlim(double argtime_limit){init_time_limit = argtime_limit;}
    void setCTarget(int argcoin_target){coin_target = argcoin_target;}
    void setCPS(){coinspersec = coin_target/init_time_limit;}
    void levelUp(float *runtime, float *currtime, Lasso *lassoPtr);
    //checksaved() reads from a local file and displays info about saved games. It takes input from user and sets the level objects'
    //details to match the saved game.
    void checksaved();
    //reset will reset the level and display highscores
    void reset();
    //fail will display "level failed!", and call the reset()
    void fail();
    //firstInstructions displays details about the game to be started.
    void firstInstructions();
    //ask_new_or_saved asks the player if he wants to start a new game or load a saved one.
    bool ask_new_or_saved();
    //savegame() saves details about the game from level object to a local file.
    void savegame();
    //reset_coins_and_scoins() will change the two scoins that are available in the game.
    void reset_coins_and_scoins(vector<Scoin*> &scoins, vector<Coin*> &coins);
};
//Other useful functions, not member functions of Level.
//next_fig_dyn changes coordinates of the minute hand of the dynamite graphic.
void next_fig_dyn(int &dyn_x2, int &dyn_y2, bool &flipsign);
//homescreen() displays Lasso logo etc, at x,y as vertex.
void homescreen(int x, int y, Line *l1, Text &caption, Text &credits, Rectangle &backdrop_sky, Rectangle &backdrop_sand);
//checkHS checks if the player, having aborted or failed the level, has secured a high score or not and stores it on a local file.
bool checkHS(int myscore, string player_name, vector<string> *textlines, int*rank);
//getPlayerName takes input from user, getMyPlayerName takes data from Level object.
string getPlayerName(string last_name);
//split() stores parts of string a obtained by breaking it whenever c occurs in the vector b passed by reference.
void split(string a, char c, vector<string> &b);
