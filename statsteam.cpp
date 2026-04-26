#include "statsteam.h"

statsteam::statsteam() {
    favorGoals   = 0;
    counterGoals = 0;
    wins         = 0;
    draws        = 0;
    losses       = 0;
    yellowCards  = 0;
    redCards     = 0;
    faults       = 0;
    Medidor::registrarCreacion("StatsTeam",this);
}

statsteam::statsteam(unsigned int gf, unsigned int gc,
                     unsigned int pg, unsigned int pe, unsigned int pp) {
    favorGoals   = gf;
    counterGoals = gc;
    wins         = pg;
    draws        = pe;
    losses       = pp;
    yellowCards  = 0;
    redCards     = 0;
    faults       = 0;

}

void statsteam::goal(){
    favorGoals++;
}

void statsteam::goal(int gol){
    favorGoals+=gol;
}


void statsteam::countergoal(){
    counterGoals++;
}
void statsteam::countergoal(int counter){
    counterGoals+=counter;
}


void statsteam::win(){
    wins++;
}
void statsteam::win(int gano){
    wins+=gano;
}

void statsteam::lose(){
    losses++;
}
void statsteam::lose(int perdio){
    losses+=perdio;
}

void statsteam::equal(){
    draws++;
}
void statsteam::equal(int igual){
    draws+=igual;
}
//+++++++++++++++++++++++++++++++++++++++ |GETTERS| +++++++++++++++++++++++++++++++
// unsigned int statsteam::getfavorGoals(){return favorgoals;}
// unsigned int statsteam::getcounterGoals(){return countergoals;}
// unsigned int statsteam::getWins(){return wingames;}
// unsigned int statsteam::getLoses(){return losegames;}
// unsigned int statsteam::getEqual(){return equalgames;}
unsigned int statsteam::getfavorGoals()  const { return favorGoals;   }
unsigned int statsteam::getcounterGoals()const { return counterGoals; }
unsigned int statsteam::getWins()        const { return wins;         }
unsigned int statsteam::getDraws()       const { return draws;        }
unsigned int statsteam::getLosses()      const { return losses;       }
unsigned int statsteam::getYellowCards() const { return yellowCards;  }
unsigned int statsteam::getRedCards()    const { return redCards;     }
unsigned int statsteam::getFaults()      const { return faults;       }



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void statsteam::showstats() const {
    cout << "  Goles a favor  : " << favorGoals   << endl;
    cout << "  Goles en contra: " << counterGoals << endl;
    cout << "  Ganados        : " << wins         << endl;
    cout << "  Empatados      : " << draws        << endl;
    cout << "  Perdidos       : " << losses       << endl;
    cout << "  T.Amarillas    : " << yellowCards  << endl;
    cout << "  T.Rojas        : " << redCards     << endl;
    cout << "  Faltas         : " << faults       << endl;

}


void statsteam::update(unsigned int gf, unsigned int gc, bool win, bool draw) {
    favorGoals   += gf;
    counterGoals += gc;
    if (win)       wins++;
    else if (draw) draws++;
    else           losses++;
}

// Getters


// Setters
void statsteam::setfavorGoals(unsigned int v)   { favorGoals   = v; }
void statsteam::setcounterGoals(unsigned int v) { counterGoals = v; }
void statsteam::setWins(unsigned int v)         { wins         = v; }
void statsteam::setDraws(unsigned int v)        { draws        = v; }
void statsteam::setLosses(unsigned int v)       { losses       = v; }
void statsteam::addYellowCard()                 { yellowCards++;    }
void statsteam::addRedCard()                    { redCards++;       }
void statsteam::addFault()                      { faults++;         }



statsteam::~statsteam() {Medidor::registrarDestruccion("StatsTeam",this);}
