#include "selecciones.h"

Selecciones::Selecciones(string nombre,string tecnico,int ranking) {

    name=nombre;
    manager=tecnico;
    rank=ranking;
    for(unsigned int i=0;i<26;i++){
        players[i]= new jugadores("Nombre"+to_string(i),"apellido"+to_string(i),i);
    }
}




bool Selecciones::cargarstats(){}
bool Selecciones::actualizarstats(){}







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

