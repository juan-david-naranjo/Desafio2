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

void statsteam::goal(int gol){
    favorgoals+=gol;
}


void statsteam::countergoal(){
    countergoals++;
}
void statsteam::countergoal(int counter){
    countergoals+=counter;
}


void statsteam::win(){
    wingames++;
}
void statsteam::win(int gano){
    wingames+=gano;
}

void statsteam::lose(){
    losegames++;
}
void statsteam::lose(int perdio){
    losegames+=perdio;
}

void statsteam::equal(){
    equalgames++;
}
void statsteam::equal(int igual){
    equalgames+=igual;
}
//+++++++++++++++++++++++++++++++++++++++ |GETTERS| +++++++++++++++++++++++++++++++
unsigned int statsteam::getfavorGoals(){return favorgoals;}
unsigned int statsteam::getcounterGoals(){return countergoals;}
unsigned int statsteam::getWins(){return wingames;}
unsigned int statsteam::getLoses(){return losegames;}
unsigned int statsteam::getEqual(){return equalgames;}




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void statsteam::showstats(){
    cout<<"Goles a favor: "<<favorgoals<<endl;
    cout<<"Goles en contra: "<<countergoals<<endl;
    cout<<"Partidos ganados: "<<wingames<<endl;
    cout<<"Partidos perdidos: "<<losegames<<endl;
    cout<<"Partiddos empatados: "<<equalgames<<endl;

}

