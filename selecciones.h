#ifndef SELECCIONES_H
#define SELECCIONES_H
#include "jugadores.h"
#include "statsteam.h"


class Selecciones
{
    string name;
    string manager;
    int rank;
    jugadores *players[26];
    statsteam stats;

public:
    Selecciones(string nombre,string tecnico,int ranking);      //constructor
    void showplayers();
    void showstats();
    bool cargarstats();
    void actualizarstats(int favorgoal,int countergoals);
    void wingame();
    void losegame();
    void equalgame();
    void onceTitular(jugadores *headlines[11]);

    //getters
    jugadores** convocados();
    string getname();
    string getmanager();
    unsigned int favorGoals();
    unsigned int counterGoals();
    int getRanking()const;

    ~Selecciones(); //destructor
};

#endif // SELECCIONES_H
