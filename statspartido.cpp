#include "statspartido.h"

statspartido::statspartido() {}


void statspartido::addplayer(unsigned short int team,jugadores *jugador){

    switch (team) {
    case 1:
        convocados1[cont1]=jugador;
        cont1++;
        break;
    case 2:
        convocados2[cont2]=jugador;
        cont2++;
        break;
    default:
        break;
    }

}

void statspartido::convocados(jugadores** headline,unsigned int team){

    if (headline == nullptr) return;

    //headline 1 es el primer elemento de el arreglo con los onces convocados
    for(unsigned short int i=0;i<11;i++){
        if(team==1)convocados1[i]=*(headline+i);
        else convocados2[i]=*(headline+i);
    }
}


//+++++++++++++++++++++++++++++ |GETTERS|++++++++++++++++++++++++++++++++++++++
unsigned int statspartido::getgol(unsigned short opcion){
    if(opcion==1)return goals1;
    else return goals2;
}

float statspartido::getpos(unsigned short opcion){
    if(opcion==1)return posesion1;
    else return posesion2;
}

jugadores* statspartido::Getplayer(unsigned int team,unsigned int indice){
    if(indice>11){
        cout<<"Indice fuera de rango"<<endl;
        return nullptr;
    }
    switch (team){
    case 1:return convocados1[indice];
    case 2:return convocados2[indice];
    default: return nullptr;
    }
}




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void statspartido::showconvocados(){
    for(unsigned int i=0;i<11;i++){
        convocados1[i]->getname();
    }
}