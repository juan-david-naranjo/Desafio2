#ifndef SELECCIONES_H
#define SELECCIONES_H
#include "jugadores.h"
#include "statsteam.h"
#include "medidor.h"

class Selecciones
{

    string name;
    string manager;
    int rank;
    string fede;
    string conf;
    jugadores *players[26];
    statsteam stats;

public:
    Selecciones(string nombre,string tecnico,int ranking);      //constructor
    Selecciones(string nombre, string tecnico, int ranking, string federacion,     // constructor con stats CSV
                string confederacion,
                unsigned int gf, unsigned int gc,
                unsigned int pg, unsigned int pe, unsigned int pp);
    void showplayers();
    void showstats();
    bool cargarstats();
    void actualizarstats(int favorgoal,int countergoals);
    void wingame();
    void losegame();
    void equalgame();
    void onceTitular(jugadores *headlines[11]);

    //getters
    unsigned int getWin() const;
    unsigned int getLose()const;
    unsigned int getDraws() const;
    jugadores** convocados();
    const string& getname()const;
    string getmanager();
    unsigned int favorGoals();
    unsigned int counterGoals();
    int getRanking()const;
    string getConfederacion()     const;
    string getfederacion()     const;       //falto por colocar la federacion
    jugadores* getPlayer(int i)   const;
    jugadores* getPlayerByShirt(int n) const;

    ~Selecciones(); //destructor
};

#endif // SELECCIONES_H
