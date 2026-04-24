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

string jugadores::getname() const{
    return name;
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
    cout<<"  |    |";
    cout<<name<<"                 ";
    cout<<goals<<"  ";
    cout<<minutes<<"  ";
    cout<<yellowcard<<"  ";
    cout<<faults<<"  ";
    cout<<redcard<<"    |\n";

}

//++++++++++++++++++++++++++++++++++    |GETTERS|+++++++++++++++++++++++++
unsigned int jugadores:: getamarilla(){
    return yellowcard;
}

unsigned int jugadores::getShirt()       const { return shirt;      }
unsigned int jugadores::getGoals()       const { return goals;      }
unsigned int jugadores::getMinutes()     const { return minutes;    }
unsigned int jugadores::getYellowCards() const { return yellowcard; }
unsigned int jugadores::getRedCards()    const { return redcard;    }
unsigned int jugadores::getFaults()      const { return faults;     }

jugadores::~jugadores(){}
