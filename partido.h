#ifndef PARTIDO_H
#define PARTIDO_H
#include <cmath>
#include <string>
#include <random>
#include <iostream>
#include "selecciones.h"
#include "statspartido.h"





class Partido
{
    string fecha;
    string hora;
    string sede="nombreSede";
    string arbitro[3]={"arbitro1","arbitro2","arbitro3"};
    Selecciones *equipo1;
    Selecciones *equipo2;
    statspartido stats;
    bool porroga;

public:
    Partido(Selecciones *team1,Selecciones *team2,string date,string hour); //constructor
    int generarGoles(double lambda);
    void simular();
    void oncetitular();
    void showpartido();



    ~Partido(); //destructor
};

#endif // PARTIDO_H
