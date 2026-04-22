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
    void Playerstats();
    void addplayer(unsigned short int team,jugadores *jugador);   //funcion para agregar jugadores
    void addGol(int equipo);
    void SetPartido(bool porroga);
    void calcularPosesion(int rankE1, int rankE2);

    //++++++++++++++ |GETTERS|+++++++++++++++

    // unsigned int getgol(unsigned short int ocpion);
    unsigned int getgol(int equipo)      const;
    float getPosesion(int equipo) const;
    void showconvocados() const;
    jugadores*   Getplayer(int equipo, int indice) const;
    // jugadores *Getplayer(unsigned int team,unsigned int indice);




    ~statspartido();

};

#endif // STATSPARTIDO_H
