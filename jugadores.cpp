#include "jugadores.h"


jugadores::jugadores(string nombre, string apellido,int num) {
    name=nombre;
    lastname=apellido;
    shirt=num;
    goals=0;
    minutes=0;
    yellowcard=0;
    redcard=0;
    faults=0;
}

void jugadores::actualizarstats(unsigned int gol,unsigned int minutos,unsigned int amarilla,unsigned int roja, unsigned int faltas){
    goals+=gol;
    minutes+=minutos;
    yellowcard+=amarilla;
    redcard+=roja;
    faults+=faltas;
}

void jugadores::getname(){
    cout<<name;
    cout<<" ";
    cout<<lastname;
    cout<<" ";
    cout<<shirt<<endl;
}


void jugadores::gol(){
    goals++;
}

void jugadores::amarilla(){
    if(yellowcard<2)yellowcard++;
    else redcard++;
}
void jugadores::roja(){
    redcard++;
}


void jugadores::fault(){
    faults++;
}

void jugadores::setminutos(unsigned int minutos){
    minutes+=minutos;
}


void jugadores::showststats(){
    cout<<name<<endl;
    cout<<"Goles: "<<goals<<endl;
    cout<<"faltas: "<<faults<<endl;
    cout<<"minutos jugados: "<<minutes<<endl;
    cout<<"amarillas: "<<yellowcard<<endl;
    cout<<"Rojas: "<<redcard;

}

//++++++++++++++++++++++++++++++++++    |GETTERS|+++++++++++++++++++++++++
unsigned int jugadores:: getamarilla(){
    return yellowcard;
}

jugadores::~jugadores(){}