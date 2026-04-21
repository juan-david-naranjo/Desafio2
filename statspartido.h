#ifndef STATSPARTIDO_H
#define STATSPARTIDO_H
#include "jugadores.h"

class statspartido {
    unsigned int golesE1;
    unsigned int golesE2;
    float posesionE1;   // porcentaje 0-100
    float posesionE2;
    jugadores* titularesE1[11];
    jugadores* titularesE2[11];

public:
    statspartido();

    // Asigna los 11 convocados de un equipo (equipo: 1 o 2)
    void convocados(jugadores** plantel, int equipo);

    // Registra un gol para el equipo indicado
    void addGol(int equipo);

    // Calcula posesión proporcional al ranking FIFA
    // rankE1 y rankE2: posición en ranking (menor = mejor)
    void calcularPosesion(int rankE1, int rankE2);

    // Getters
    unsigned int getgol(int equipo)      const;
    float        getPosesion(int equipo) const;
    jugadores*   Getplayer(int equipo, int indice) const;

    void showconvocados() const;

    ~statspartido();
};

#endif // STATSPARTIDO_H
