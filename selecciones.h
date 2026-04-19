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
    bool actualizarstats();

    ~Selecciones(); //destructor
};

#endif // SELECCIONES_H
