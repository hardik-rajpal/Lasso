#include<simplecpp>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include"levels.h"
#include"lasso.h"
using namespace std;
void Level::checksaved(){
    ifstream games("sg.csv");
    if(games.is_open()){
        vector<string> lines; string line;
        int score=0;
        while(getline(games, line)){
            lines.push_back(line);            
        }
        int n=lines.size();
        Rectangle backdrop_savedgames(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
        backdrop_savedgames.setColor(WHITE);
        backdrop_savedgames.setFill();
        Text instructions(0.5*WINDOW_X, 0.5*WINDOW_Y+20*(n+3)-200, "Choose saved game by entering the serial number and pressing Enter.");
        Text title(0.5*WINDOW_X, 0.5*WINDOW_Y-20-200, "Serial No., Player Name, Current Level, Total Coins, Time Limit, Coin Target, Lassos Left:");
        Text t[n];
        for(int i=0;i<n;i++){
            t[i].reset(0.5*WINDOW_X, 0.5*WINDOW_Y-200+20*i, lines[i]);
        }
        string snum="";
        Text snumt(0.5*WINDOW_X, 0.5*WINDOW_Y+20*(n+4)-200, " ");
        int sernumint;
        while (true){
            XEvent e;
            //wait(0.05);
            
            bool pendingEv = checkEvent(e);
            if(pendingEv){
                char c= charFromEvent(e);
                if(c=='\0'){continue;}
                if(c==8){//c==8 corresponds to backspace
                    snum = snum.substr(0, snum.length()-1);
                }//c==13 corresponds to Enter key.
                else if(c==13&&snum!=""){break;}
                else{
                    if(n==10&&c=='0'){
                        snum+=c;
                    }
                    else{
                        snum=c;
                    }//stringstreams convert string to ints.
                    stringstream str_c_to_int;
                    str_c_to_int<<snum;
                    str_c_to_int>>sernumint;
                }
                if(sernumint!=0&&sernumint<=n){
                    snumt.setMessage(snum);
                }
            }
        }
        line = lines[sernumint-1];
        vector<string> words;
        char comma = ',';
        //split is specified in levels.h
        split(line, comma, words);
        int lnum_int, tot_coins_int, c_target_int, num_lassos_int;
        double time_limit_double;
        stringstream converter;
            converter<<words[2];
            converter>>lnum_int;
            converter.clear();
            converter<<words[3];
            converter>>tot_coins_int;
            converter.clear();
            converter<<words[4];
            converter>>time_limit_double;
            converter.clear();
            converter<<words[5];
            converter>>c_target_int;
            converter.clear();
            converter<<words[6];
            converter>>num_lassos_int;
        player_name = words[1];
        lnumber=lnum_int;
        num_lassos=num_lassos_int;
        total_coins_till_last_level = tot_coins_int;
        total_coins = total_coins_till_last_level;
        init_time_limit = time_limit_double;
        coin_target=c_target_int;
        coinspersec = coin_target/init_time_limit;
    }
    else{
        Rectangle backdrop_checksave_error(0.5*WINDOW_X, 0.5*WINDOW_Y,200, 200);
        backdrop_checksave_error.setColor(MYWHITE);
        backdrop_checksave_error.setFill();
        Text error_checksave(0.5*WINDOW_X, 0.5*WINDOW_Y, "Required File NOT Found. Click to continue.");
        getClick();
    }
}
bool Level::ask_new_or_saved(){
    Rectangle backdrop(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
    backdrop.setColor(COLOR(255, 255, 255));
    backdrop.setFill();
    Rectangle backdrop_sky, backdrop_sand;

    Line l1[19];
    Text caption, credits;
    
    homescreen(125, 50, l1, caption, credits, backdrop_sky, backdrop_sand);
    
    int x1, x2, x3, x4, y1, y2;
    x1=0.5*WINDOW_X-50-0.5*textWidth("New Game");
    x2=0.5*WINDOW_X-50+0.5*textWidth("New Game");
    x3=0.5*WINDOW_X+50-0.5*textWidth("Saved Game");
    x4=0.5*WINDOW_X+50+0.5*textWidth("Saved Game");
    y1=0.5*WINDOW_Y-8+150;
    y2=0.5*WINDOW_Y+8+150;
    Rectangle newgamerect(0.5*WINDOW_X-50, y1+8, x2-x1+10, y2-y1+5);
    newgamerect.setFill();
    Rectangle savedgamerect(0.5*WINDOW_X+50, y1+8, x4-x3+10, y2-y1+5);
    savedgamerect.setFill();
    Text newgamet(0.5*WINDOW_X-50, y1+8, "New Game");
    Text savedgamet(0.5*WINDOW_X+50, y1+8, "Saved Game");
    
    while(true){
        int w = getClick(); int x=w/65536;int y=w%65536;
        if(x1<=x&&x<=x2&&y1<=y&&y<=y2){
            return true;
        }
        else if(x3<=x&&x<=x4&&y1<=y&&y<=y2){
            checksaved();
            return false;
        }
        else{
            continue;
        }
    }
}
void Level::levelUp(float *runtime, float *currtime, Lasso *lassoPtr){
    *currtime = 0;
    float incr_difficulty = (10+(rand()%20))/1000.0;
    coinspersec += incr_difficulty;
    init_time_limit = 40*(1+rand()%5);
    coin_target = coinspersec*init_time_limit;
    *runtime = init_time_limit;
    Rectangle levelupbox(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
    levelupbox.setColor(COLOR(255, 255, 255));
    levelupbox.setFill();
    char leveluspstr[256] = "Level passed!";
    sprintf(leveluspstr, "Level %d passed!", lnumber);
    Text levelup(0.5*WINDOW_X, 0.5*WINDOW_Y, leveluspstr);
    lnumber++;
    char newlevelinfostr[256] = "Next Level: Level ";
    sprintf(newlevelinfostr, "Up Next: Level %d !", lnumber);
    char timelimitstr[256] = "Time limit: ";
    sprintf(timelimitstr, " Coins to be collected: %d. Time Limit: %.2f", coin_target, init_time_limit);
    Text newlevelinfo(0.5*WINDOW_X, 0.5*WINDOW_Y + 20, newlevelinfostr);
    Text timelimitinfo(0.5*WINDOW_X, 0.5*WINDOW_Y + 40, timelimitstr);
    Text proceed(0.5*WINDOW_X, 0.5*WINDOW_Y+60, "Click anywhere to begin next level.");
    total_coins_till_last_level += lassoPtr->getNumCoins();
    lassoPtr->resetCoinScore();
    getClick();
}
void Level::reset(){
    vector<string> textlines;
    Rectangle hiscore_r(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
    hiscore_r.setColor(COLOR(255, 255, 255));
    hiscore_r.setFill();
    int myrank=-1;
    bool hiscore_acheived = checkHS(total_coins, player_name, &textlines, &myrank);
    if(hiscore_acheived){
        Text hiscore_acheivedtxt(0.5*WINDOW_X, 0.5*WINDOW_Y, "Hiscore Acheived!");
        Text click_to_continue(0.5*WINDOW_X, 0.5*WINDOW_Y+20, "Click to continue!");
        getClick();
    }
    {//Writing the next code in this block so that the variables are temporary and text disappears later.
        Text scores[textlines.size()];
        Text title(0.5*WINDOW_X, 0.5*WINDOW_Y - 120, "Players, Their Highscores:");
        int i=0;
        for(;i<textlines.size();i++){
            if(myrank==i){
                scores[i].setColor(GREEN);
            }
            scores[i].reset(0.5*WINDOW_X, 0.5*WINDOW_Y+20*i-100, textlines[i]);
        }
        Text click_to_continue(0.5*WINDOW_X, 0.5*WINDOW_Y+20*i-100, "Click to continue!");
        getClick();
    }
    num_lassos = 2;
    lnumber = 1;
    total_coins=0;
    total_coins_till_last_level=0;
    setCTarget(MIN_CTARGET);
    setTlim(INIT_T_LIM);
    setCPS();
}
void Level::fail(){
    Rectangle fail_rect(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
    fail_rect.setColor(COLOR(255, 255, 255));
    fail_rect.setFill();
    char levelfailstr[256] = "Level failed!";
    sprintf(levelfailstr, "Level %d failed!", lnumber);
    Text levelfail(0.5*WINDOW_X, 0.5*WINDOW_Y, levelfailstr);
    Text cont(0.5*WINDOW_X, 0.5*WINDOW_Y+30, "Click anywhere to continue.");
    getClick();
    reset();
}
void Level::firstInstructions(){
    Rectangle f_instr_rect(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
    f_instr_rect.setColor(MYWHITE);
    f_instr_rect.setFill();
    char upcoming_game_str[256] = "Upcoming game:";
    Text upcoming_game_txt(0.5*WINDOW_X, 0.5*WINDOW_Y, upcoming_game_str);
    char newlevelinfostr[256] = "Next Level: Level ";
    sprintf(newlevelinfostr, "Level %d !", lnumber);
    char timelimitstr[256] = "Time limit: ";
    sprintf(timelimitstr, " Coins to be collected: %d. Time Limit: %.2f", coin_target, init_time_limit);
    Text newlevelinfo(0.5*WINDOW_X, 0.5*WINDOW_Y + 20, newlevelinfostr);
    Text timelimitinfo(0.5*WINDOW_X, 0.5*WINDOW_Y + 40, timelimitstr);
    Text proceed(0.5*WINDOW_X, 0.5*WINDOW_Y+60, "Click anywhere to continue.");
    getClick();
}
void Level::savegame(){
    ifstream gamesi("sg.csv");
    if(gamesi.is_open()){
        vector<string> lines; string line, lnum_str, totcoins_str, time_left_str, coin_target_str, num_lassos_str;
        int score=0;
        while(getline(gamesi, line)){
            lines.push_back(line);            
        }
        int n=lines.size();
        lines.push_back(" ");
        for(int i=n;i>0;i--){
            lines[i]=lines[i-1];
            lines[i][0]='1'+i;
        }
        while(lines.size()>10){
            lines.pop_back();
        }
        n=lines.size();
        if(lines.size()==10){
            lines[9] = "10"+lines[9].substr(1, lines[9].length()-1);
        }
        stringstream converter;
        converter<<lnumber;
        converter>>lnum_str;
        converter.clear();
        converter<<total_coins_till_last_level;
        converter>>totcoins_str;
        converter.clear();
        converter<<init_time_limit;
        converter>>time_left_str;
        converter.clear();
        converter<<coin_target;
        converter>>coin_target_str;
        converter.clear();
        converter<<num_lassos;
        converter>>num_lassos_str;
        line = "1,"+player_name+","+lnum_str+","+totcoins_str+","+time_left_str+","+coin_target_str+","+num_lassos_str;
        lines[0]=line;
        gamesi.close();
        ofstream gameso("sg.csv");
        for(int i=0;i<n-1;i++){
            gameso<<lines[i]<<"\n";
        }
        gameso<<lines[n-1];
        Rectangle backdrop_saving_game(0.5*WINDOW_X, 0.5*WINDOW_Y,200, 200);
        backdrop_saving_game.setColor(MYWHITE);
        backdrop_saving_game.setFill();
        Text affirmation(0.5*WINDOW_X, 0.5*WINDOW_Y, "Game Saved. Click to continue.");
        getClick();    
    }
    else{
        Rectangle backdrop_saving_game_error(0.5*WINDOW_X, 0.5*WINDOW_Y,200, 200);
        backdrop_saving_game_error.setColor(MYWHITE);
        backdrop_saving_game_error.setFill();
        Text error_saving_game(0.5*WINDOW_X, 0.5*WINDOW_Y, "Game NOT Saved. Required File NOT Found. Click to continue.");
        getClick();
    }
}
void Level::reset_coins_and_scoins(vector<Scoin*> &scoins, vector<Coin*> &coins){
    int nscoins = scoins.size();
    for(int i=0;i<alivescoins.size();i++){
        alivescoins[i]->killScoin();
      }
      //Picking new scoins for the next level.
      alivescoins.clear();
      int num_scoins_allowed=2;
      int scoinsind[nscoins];
      for(int i=0;i<nscoins;i++){
        scoinsind[i] = i;
      }
      while(num_scoins_allowed){
        int j = rand()%nscoins;
        if(scoinsind[j]!=-1){
          alivescoins.push_back(scoins[j]);
          scoins[j]->resurrScoin();
          scoins[j]->deactivateScoin();
          scoinsind[j]=-1;
          num_scoins_allowed--;
        }
      }    
      for(int i=0;i<coins.size();i++){
        coins[i]->resetCoin();
      }
      for(int i=0;i<alivescoins.size();i++){
        alivescoins[i]->resurrScoin();
      }
}
void split(string a, char c, vector<string> &b){
    int j=0;
    for(int i=0;i<a.length();i++){
        if(a[i]==c){
            b.push_back(a.substr(j, i-j));
            j=i+1;
        }
        if(i==a.length()-1){
            b.push_back(a.substr(j, i+1-j));
        }
    }
}
string getPlayerName(string last_name){
    string names=last_name;
    Rectangle backdrop(0.5*WINDOW_X, 0.5*WINDOW_Y, WINDOW_X, WINDOW_Y);
    backdrop.setColor(COLOR(255, 255, 255));
    backdrop.setFill();
    Text instructions(0.5*WINDOW_X, 0.5*WINDOW_Y-30, "Please enter the Player's name: (8 characters max)");
    Text name(0.5*WINDOW_X, 0.5*WINDOW_Y, " ");
    Text enter(0.5*WINDOW_X, 0.5*WINDOW_Y+30, "Press Enter to continue to the game");
    while (true){
    XEvent e;
    bool pendingEv = checkEvent(e);
        if(pendingEv){
            char c= charFromEvent(e);
            if(c=='\0'){
                continue;
            }
            if(c==8){//c==8 corresponds to backspace
                names = names.substr(0, names.length()-1);
            }
            else if(c==13){//c==13 corresponds to Enter key.
                return names;
            }//WE make sure the player's name is less than or equal to 8
            else if(names.length()<8){
                names = names + c;        
            }
        name.reset(0.5*WINDOW_X, 0.5*WINDOW_Y, names);name.setMessage(names);
        }
    }
}
bool checkHS(int myscore, string player_name, vector<string> *textlines, int*rank){
    bool hiscore_acheived=false;
    vector<int>iscores;
    vector<string> players;
    ifstream hiscfi("hiscores.csv");
    if(hiscfi.is_open()){
        string line; string scorestr= "";
        int score=0;
        while(getline(hiscfi, line)){
            //lines.push_back(line);
            string playernamestr;
            int j=0;
            for(;line[j]!=',';j++){}            
            playernamestr=line.substr(0, j);
            scorestr=line.substr(j+1, line.size());
            players.push_back(playernamestr);
            stringstream str_c_to_int;
            str_c_to_int<<scorestr;
            str_c_to_int>>score;
            iscores.push_back(score);
        }
        for(int i=0;i<iscores.size();i++){
            if(iscores[i]<=myscore){
                hiscore_acheived = true;
                iscores.insert(iscores.begin()+i, myscore);
                players.insert(players.begin()+i, player_name);
                *rank = i;
                while(iscores.size()>10){
                    iscores.pop_back();
                    players.pop_back();
                }
                hiscfi.close();
                
                ofstream hiscfo("hiscores.csv");
            
                for(int l=0;l<players.size()-1;l++){
                    stringstream str_c_to_int;
                    str_c_to_int<<iscores[l];
                    str_c_to_int>>scorestr;
                    line=players[l]+","+scorestr;
                    textlines->push_back(line);
                    hiscfo<<line<<"\n";
                }
                stringstream str_c_to_int;
                    str_c_to_int<<iscores[players.size()-1];
                    str_c_to_int>>scorestr;
                    line = players[players.size()-1]+","+scorestr;
                hiscfo<<line;
                textlines->push_back(line);
                break;
            }
            else if(i==iscores.size()-1){

                for(int l=0;l<players.size();l++){
                    stringstream str_c_to_int;
                    str_c_to_int<<iscores[l];
                    str_c_to_int>>scorestr;
                    line=players[l]+","+scorestr;
                    textlines->push_back(line);
                }
            }
        }
    }
    else{
        Rectangle high_score_error(0.5*WINDOW_X, 0.5*WINDOW_Y,200, 200);
        high_score_error.setColor(MYWHITE);
        high_score_error.setFill();
        Text high_score_game(0.5*WINDOW_X, 0.5*WINDOW_Y, "File for Highscores NOT Found. Click to continue.");
        getClick();
    }
    return hiscore_acheived;   
}
void next_fig_dyn(int &dyn_x2, int &dyn_y2, bool &flipsign){
    int temp;
    temp = dyn_x2;
    dyn_x2 = dyn_y2;
    dyn_y2 = temp;
    if(flipsign){dyn_y2 = -dyn_y2;dyn_x2 = -dyn_x2;}
    flipsign = !flipsign;
}
void homescreen(int x, int y, Line *l1, Text &caption, Text &credits, Rectangle &backdrop_sky, Rectangle &backdrop_sand){
    double t = 10;
    backdrop_sky.setColor(MYSKYBLUE);
    backdrop_sky.setFill();
    backdrop_sand.setColor(MYSANDCOLOR);
    backdrop_sand.setFill();
    backdrop_sky.reset(0.5*WINDOW_X, 0.5*(y+200+2*t),WINDOW_X,y+200+2*t);
    backdrop_sand.reset(0.5*WINDOW_X, y+200+7*t+0.5*(WINDOW_Y-(y+200+2*t)), WINDOW_X, WINDOW_Y-(y+200+2*t));
    //I have not used a loop for setThickness because there's a bug in simplecpp and the member doesn't work fine in a loop.
    l1[0].init(x, y, x, y+200);
    l1[0].setThickness(t);
    l1[1].init(x, y+200, x+100, y+200);
    l1[1].setThickness(t);
    l1[2].init(x+170, y+150, x+220, y+150);
    l1[2].setThickness(t);
    l1[3].init(x+195, y+100, x+150, y+200);
    l1[3].setThickness(t);
    l1[4].init(x+195, y+100, x+240, y+200);
    l1[4].setThickness(t);
    l1[5].init(x+290, y+100, x+340, y+100);
    l1[5].setThickness(t);
    l1[6].init(x+290, y+150, x+340, y+150);
    l1[6].setThickness(t);
    l1[7].init(x+290, y+200, x+340, y+200);
    l1[7].setThickness(t);
    l1[8].init(x+290+100, y+100, x+440, y+100);
    l1[8].setThickness(t);
    l1[9].init(x+290+100, y+150, x+440, y+150);
    l1[9].setThickness(t);
    l1[10].init(x+290+100, y+200, x+440, y+200);
    l1[10].setThickness(t);
    l1[11].init(x+290, y+100, x+290, y+150);
    l1[11].setThickness(t);
    l1[12].init(x+340, y+150, x+340, y+200);
    l1[12].setThickness(t);
    l1[13].init(x+390, y+100, x+390, y+150);
    l1[13].setThickness(t);
    l1[14].init(x+440, y+150, x+440, y+200);
    l1[14].setThickness(t);
    l1[15].init(x+490, y+100, x+490, y+200);
    l1[15].setThickness(t);
    l1[16].init(x+540, y+100, x+540, y+200);
    l1[16].setThickness(t);
    l1[17].init(x+490, y+100, x+540, y+100);
    l1[17].setThickness(t);
    l1[18].init(x+490, y+200, x+540, y+200);
    l1[18].setThickness(t);
    for(int i=0;i<19;i++){
        l1[i].setColor(MYBROWN);
    }
    string a="The most authentic experience of the Wild West";
    caption.reset(x+0.5*textWidth(a), y+260, a);
    caption.setColor(MYDARKGREEN);
    a="a game by Prof. B. Raman and H. Rajpal";
    credits.reset(x+540-0.5*textWidth(a), y+230, a);
}