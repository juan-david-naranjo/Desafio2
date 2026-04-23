#ifndef PARTIDO_H
#define PARTIDO_H
#include <cmath>
#include <string>
#include <random>
#include <iostream>
#include "selecciones.h"
#include "statspartido.h"
#include "fecha.h"





class Partido
{
    Fecha *fecha;
    string sede="nombreSede";
    string arbitro[3]={"arbitro1","arbitro2","arbitro3"};
    Selecciones *equipo1;
    Selecciones *equipo2;
    statspartido stats;
    bool porroga;

public:
    Partido(Selecciones *team1,Selecciones *team2,Fecha *date);    //constructor
    int generarGoles(double lambda);
    void simularEliminatoria();
    void simularFaseGrupos();
    void showstats();
    void simular(bool esEliminatoria);
    void FaltasPlayer(jugadores* player);
    void tryamarilla(unsigned int amarilla,jugadores* player);
    void simularfaltas();



    void oncetitular();
    void showpartido();
    int Golesreales(short int equipo,int potencialgoles);
    void Manejarempate(int potencial1,int potencial2);



    ~Partido(); //destructor
};

#endif // PARTIDO_H
