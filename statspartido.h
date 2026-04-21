#ifndef STATSPARTIDO_H
#define STATSPARTIDO_H
#include "jugadores.h"

using namespace std;


class statspartido
{
    unsigned int goals1=0;
    unsigned int goals2=0;
    float posesion1=0;
    float posesion2=0;
    jugadores *convocados1[11];
    jugadores *convocados2[11];
    unsigned short int cont1=0;
    unsigned short int cont2=0;

public:
    statspartido();
    void convocados(jugadores** headline,unsigned int team);
    void addplayer(unsigned short int team,jugadores *jugador);   //funcion para agregar jugadores
    void Playerstats();
    void SetPartido(bool porroga);

    //++++++++++++++ |GETTERS|+++++++++++++++

    unsigned int getgol(unsigned short int ocpion);
    float getpos(unsigned short int ocpion);
    void showconvocados();
    jugadores *Getplayer(unsigned int team,unsigned int indice);
};

#endif // STATSPARTIDO_H
