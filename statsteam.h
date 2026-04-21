#ifndef STATSTEAM_H
#define STATSTEAM_H
#include <iostream>
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
    statsteam(unsigned int gf, unsigned int gc, unsigned int pg,
              unsigned int pe, unsigned int pp);

    // Actualiza sumando los resultados de un partido
    void update(unsigned int gf, unsigned int gc, bool win, bool draw);

    // Getters
    unsigned int getfavorGoals() const;
    unsigned int getcounterGoals() const;
    unsigned int getWins()        const;
    unsigned int getDraws()       const;
    unsigned int getLosses()      const;
    unsigned int getYellowCards() const;
    unsigned int getRedCards()    const;
    unsigned int getFaults()      const;

    // Setters (usados por GestorArchivos al cargar el CSV)
    void setfavorGoals(unsigned int v);
    void setcounterGoals(unsigned int v);
    void setWins(unsigned int v);
    void setDraws(unsigned int v);
    void setLosses(unsigned int v);
    void addYellowCard();
    void addRedCard();
    void addFault();

    void showstats() const;

    ~statsteam();
};

#endif // STATSTEAM_H
