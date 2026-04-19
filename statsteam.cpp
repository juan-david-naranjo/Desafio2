#include "statsteam.h"

statsteam::statsteam(unsigned int favor,unsigned int contra,unsigned int win,unsigned int loses,unsigned int equal) {
    favorgoals=favor;
    countergoals=contra;
    wingames=win;
    losegames=loses;
    equalgames=equal;
}
statsteam::statsteam(){
    favorgoals=0;
    countergoals=0;
    wingames=0;
    losegames=0;
    equalgames=0;
}

void statsteam::goal(){
    favorgoals++;
}
void statsteam::countergoal(){
    countergoals++;
}
void statsteam::win(){
    wingames++;
}
void statsteam::lose(){
    losegames++;
}
void statsteam::equal(){
    equalgames++;
}

void statsteam::showstats(){
    cout<<"Goles a favor: "<<favorgoals<<endl;
    cout<<"Goles en contra: "<<countergoals<<endl;
    cout<<"Partidos ganados: "<<wingames<<endl;
    cout<<"Partidos perdidos: "<<losegames<<endl;
    cout<<"Partiddos empatados: "<<equalgames<<endl;

}

