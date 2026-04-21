#ifndef JUGADORES_H
#define JUGADORES_H
#include <string>
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
    void getname();
    void gol();
    void amarilla();
    void roja();
    void fault();
    void setminutos(unsigned int minutos);
    void showststats();
    unsigned int getamarilla();

    ~jugadores();


};

#endif // JUGADORES_H
