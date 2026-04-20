#ifndef GRUPOS_H
#define GRUPOS_H
#include <iostream>
#include "selecciones.h"
#include "partido.h"

using namespace std;

class Grupos
{
    char letra;
    Selecciones *equipos[4];
    Partido *partidos[6];
    Selecciones *tabla[4];


public:
    Grupos();
};

#endif // GRUPOS_H
