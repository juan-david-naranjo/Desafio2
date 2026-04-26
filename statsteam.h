#ifndef STATSTEAM_H
#define STATSTEAM_H
#include <iostream>
#include "medidor.h"
using namespace std;

class statsteam {
    unsigned int favorGoals;
    unsigned int counterGoals;
    unsigned int wins;
    unsigned int draws;
    unsigned int losses;
    unsigned int yellowCards;
    unsigned int redCards;
    unsigned int faults;


public:
    statsteam();
    statsteam(unsigned int favor,unsigned int contra,unsigned int win,unsigned int loses,unsigned int equal);

    void win();
    void win(int gano);

    void lose();
    void lose(int perdio);

    void equal();
    void equal(int igual);

    void goal();
    void goal(int gol);

    void countergoal();
    void countergoal(int lose);



    void showstats() const;
    // unsigned int getfavorGoals();
    // unsigned int getcounterGoals();
    // unsigned int getWins();
    // unsigned int getLoses();
    // unsigned int getEqual();


    //Getters
    unsigned int getfavorGoals() const;
    unsigned int getcounterGoals() const;
    unsigned int getWins()        const;
    unsigned int getEqual()       const;
    /*unsigned int getLoses()      const;*/
    unsigned int getDraws()       const;
    unsigned int getLosses()      const;
    unsigned int getYellowCards() const;
    unsigned int getRedCards()    const;
    unsigned int getFaults()      const;


    // Actualiza sumando los resultados de un partido
    void update(unsigned int gf, unsigned int gc, bool win, bool draw);

    // Getters


    // Setters (usados por GestorArchivos al cargar el CSV)
    void setfavorGoals(unsigned int v);
    void setcounterGoals(unsigned int v);
    void setWins(unsigned int v);
    void setDraws(unsigned int v);
    void setLosses(unsigned int v);
    void addYellowCard();
    void addRedCard();
    void addFault();



    ~statsteam();
};

#endif // STATSTEAM_H
