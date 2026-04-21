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

void statsteam::update(unsigned int gf, unsigned int gc, bool win, bool draw) {
    favorGoals   += gf;
    counterGoals += gc;
    if (win)       wins++;
    else if (draw) draws++;
    else           losses++;
}

// Getters
unsigned int statsteam::getfavorGoals()  const { return favorGoals;   }
unsigned int statsteam::getcounterGoals()const { return counterGoals; }
unsigned int statsteam::getWins()        const { return wins;         }
unsigned int statsteam::getDraws()       const { return draws;        }
unsigned int statsteam::getLosses()      const { return losses;       }
unsigned int statsteam::getYellowCards() const { return yellowCards;  }
unsigned int statsteam::getRedCards()    const { return redCards;     }
unsigned int statsteam::getFaults()      const { return faults;       }

// Setters
void statsteam::setfavorGoals(unsigned int v)   { favorGoals   = v; }
void statsteam::setcounterGoals(unsigned int v) { counterGoals = v; }
void statsteam::setWins(unsigned int v)         { wins         = v; }
void statsteam::setDraws(unsigned int v)        { draws        = v; }
void statsteam::setLosses(unsigned int v)       { losses       = v; }
void statsteam::addYellowCard()                 { yellowCards++;    }
void statsteam::addRedCard()                    { redCards++;       }
void statsteam::addFault()                      { faults++;         }

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

statsteam::~statsteam() {}
