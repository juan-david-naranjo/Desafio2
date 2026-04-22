#include "selecciones.h"

Selecciones::Selecciones(string nombre,string tecnico,int ranking) {

    name=nombre;
    manager=tecnico;
    rank=ranking;
    for(unsigned int i=0;i<26;i++){
        players[i]= new jugadores("Nombre"+to_string(i),"apellido"+to_string(i),i);
    }
}

Selecciones::Selecciones(string nombre, string tecnico, int ranking,
                         string confederacion,
                         unsigned int gf, unsigned int gc,
                         unsigned int pg, unsigned int pe, unsigned int pp) {
    name    = nombre;
    manager = tecnico;
    rank    = ranking;
    conf    = confederacion;
    stats   = statsteam(gf, gc, pg, pe, pp);
    // Distribuye goles históricos uniformemente entre los 26 jugadores
    unsigned int golesXJugador = gf / 26;
    unsigned int resto         = gf % 26;
    for(unsigned int i = 0; i < 26; i++){
        players[i] = new jugadores("nombre"+to_string(i+1),
                                   "apellido"+to_string(i+1), i+1);
        // Los primeros 'resto' jugadores llevan un gol extra
        unsigned int golesJug = golesXJugador + (i < resto ? 1 : 0);
        if(golesJug > 0)
            players[i]->actualizarstats(golesJug, 0, 0, 0, 0);
    }
}


bool Selecciones::cargarstats(){}



void Selecciones::actualizarstats(int favorgoal,int countergoals){
    stats.goal(favorgoal);
    stats.countergoal(countergoals);

}

void Selecciones::wingame(){
    stats.win();
}

void Selecciones::losegame(){
    stats.lose();
}

void Selecciones::equalgame(){
    stats.equal();
}

void Selecciones::onceTitular(jugadores* headlines[11]){

    for(unsigned short int i=0;i<11;i++){
        headlines[i]=players[i];
    }
}



//++++++++++++++++++++++ |getters| ++++++++++++++++++++++++++
jugadores** Selecciones::convocados(){
    return players;
}

string Selecciones::getname(){
    return name;
}

string Selecciones::getmanager(){
    return manager;
}
unsigned int Selecciones:: favorGoals(){return stats.getfavorGoals();}
unsigned int Selecciones:: counterGoals(){return stats.getcounterGoals();}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



int Selecciones::getRanking()const{
    return rank;
}

string Selecciones::getConfederacion() const { return conf; }

jugadores* Selecciones::getPlayer(int i) const {
    if (i < 0 || i > 25) return nullptr;
    return players[i];
}

jugadores* Selecciones::getPlayerByShirt(int n) const {
    for (int i = 0; i < 26; i++)
        if ((int)players[i]->getShirt() == n) return players[i];
    return nullptr;
}

void Selecciones::showplayers(){
    for(int i=0;i<26;i++){
        players[i]->getname();
    }
}
void Selecciones::showstats(){
    stats.showstats();
}
Selecciones::~Selecciones(){
    for (int i = 0; i < 26; ++i) {
    delete players[i];
    }

}

