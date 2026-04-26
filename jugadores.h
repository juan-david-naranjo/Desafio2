#ifndef JUGADORES_H
#define JUGADORES_H
#include <string>
#include "medidor.h"
#include <iostream>
using namespace std;

class jugadores
{
    string name;
    string lastname;
    short unsigned int shirt;
    unsigned int goals;
    unsigned int minutes;
    unsigned int yellowcard;
    unsigned int redcard;
    unsigned int faults;
public:
    jugadores(string nombre,string apellido,signed int num);        //constructor

    //metodos
    void actualizarstats(unsigned int gol,unsigned int minutos,unsigned int amarilla,unsigned int roja, unsigned int faltas);
    const string& getname() const;
    void gol();
    void amarilla();
    void roja();
    void fault();
    void setminutos(unsigned int minutos);
    void showststats();
    // Getters para GestorArchivos
    unsigned int getamarilla();
    unsigned int getShirt()       const;
    unsigned int getGoals()       const;
    unsigned int getMinutes()     const;
    unsigned int getYellowCards() const;
    unsigned int getRedCards()    const;
    unsigned int getFaults()      const;

    ~jugadores();


};

#endif // JUGADORES_H
