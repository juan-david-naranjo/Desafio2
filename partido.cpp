#include "partido.h"

Partido::Partido(Selecciones *team1,Selecciones *team2,Fecha *date) {

    equipo1=team1;
    equipo2=team2;
    resultado=0;
    fecha=date;
    porroga=false;
    stats.convocados(equipo1->convocados(),1);
    stats.convocados(equipo2->convocados(),2);
}

void Partido::simular(bool esEliminatoria) {
    if (esEliminatoria) {
        simularEliminatoria(); // La que ya tienes con prórroga
        porroga=true;
        stats.SetPartido(porroga);
    } else {
        simularFaseGrupos();   // La nueva versión sin prórroga
        stats.SetPartido(porroga);
    }
}

void Partido::simularFaseGrupos(){
    unsigned int gfA=equipo1->favorGoals();
    unsigned int gfB=equipo2->favorGoals();
    unsigned int gcA=equipo1->counterGoals();
    unsigned int gcB=equipo2->counterGoals();

    //constantes formula goles

    const double mu = 1.35;
    const double alpha = 0.6;
    const double beta = 0.4;

    //constantes formula goles

    //Goles equipo 1
    double lambdaA = mu * pow((gfA / mu), alpha) * pow((gcB / mu), beta);

    // goles equipo 2
    double lambdaB = mu * pow((gfB / mu), alpha) * pow((gcA / mu), beta);


    //GOLES PARTIDO
    int potencialGolesE1 = 10/*generarGoles(lambdaA)*/;
    int golesRealesE1 =Golesreales(1,potencialGolesE1);

    int potencialGolesE2 = 10/*generarGoles(lambdaB)*/;
    int golesRealesE2 = Golesreales(2,potencialGolesE2);

    equipo1->actualizarstats(golesRealesE1, golesRealesE2);
    equipo2->actualizarstats(golesRealesE2, golesRealesE1);

    if (golesRealesE1 == golesRealesE2) {
        // cout << "Empate en fase de grupos" << endl;
        equipo1->equalgame(); // Método nuevo para sumar 1 punto
        equipo2->equalgame();
    } else if (golesRealesE1 > golesRealesE2) {
        // cout << "gano1" << endl;
        equipo1->wingame();  // Suma 3 puntos
        equipo2->losegame(); // Suma 0 puntos
        resultado=1;
    } else {
        // cout << "gano2" << endl;
        equipo2->wingame();
        equipo1->losegame();
        resultado=2;
    }
    simularfaltas();

}



void Partido::simularEliminatoria(){
    unsigned int gfA=equipo1->favorGoals();
    unsigned int gfB=equipo2->favorGoals();
    unsigned int gcA=equipo1->counterGoals();
    unsigned int gcB=equipo2->counterGoals();


    //constantes formula goles

    const double mu = 1.35;
    const double alpha = 0.6;
    const double beta = 0.4;
    //Goles equipo 1
    double lambdaA = mu * pow((gfA / mu), alpha) * pow((gcB / mu), beta);

    // goles equipo 2
    double lambdaB = mu * pow((gfB / mu), alpha) * pow((gcA / mu), beta);


    //GOLES PARTIDO
    int potencialGolesE1 = generarGoles(lambdaA);
    int golesRealesE1 =Golesreales(1,potencialGolesE1);

    int potencialGolesE2 = generarGoles(lambdaB);
    int golesRealesE2 = Golesreales(2,potencialGolesE2);


    if(golesRealesE1==golesRealesE2){
        porroga=true;
        Manejarempate(potencialGolesE1,potencialGolesE2);

    }
    else if(golesRealesE1>golesRealesE2){
        // cout<<"Gano el equipo 1"<<endl;
        equipo1->actualizarstats(golesRealesE1,golesRealesE2);
        equipo1->wingame();
        equipo2->actualizarstats(golesRealesE2,golesRealesE1);
        equipo2->losegame();
        resultado=1;
    }else{
        // cout<<"Gano el equipo 2"<<endl;
        equipo2->actualizarstats(golesRealesE2,golesRealesE1);
        equipo2->wingame();
        equipo1->actualizarstats(golesRealesE1,golesRealesE2);
        equipo1->losegame();
        resultado=2;
    }
    simularfaltas();
}

void Partido::showstats(){
    stats.Playerstats();
}

void Partido::Manejarempate(int potencial1,int potencial2){


    int golesE1=Golesreales(1,potencial1);
    int golesE2=Golesreales(2,potencial2);
    if(golesE1==golesE2){
        int prob=rand()%2;
        if(prob==1){
            // cout<<"gano 1 penales "<<endl;
            equipo1->actualizarstats(golesE1,golesE2);
            equipo1->wingame();
            resultado=1;

        }else{
            // cout<<"gano 2 penales "<<endl;
            equipo2->actualizarstats(golesE2,golesE1);
            equipo2->losegame();
            resultado=2;
        }
    }else if(golesE1>golesE2){
        // cout<<"gano equipo 1 porroga"<<endl;
        equipo1->actualizarstats(golesE1,golesE2);
        equipo1->wingame();
        resultado=1;
    }else{
        cout<<"Gano equipo 2 porroga"<<endl;
        equipo2->actualizarstats(golesE2,golesE1);
        equipo2->wingame();
        resultado=2;
    }
    simularfaltas();
}







int Partido::Golesreales(short int equipo,int potencialgoles){
    int goles=0;
    for (int i = 0; i < potencialgoles; i++) {

        int indice = rand() % 11;
        // cout<<"gol? "<<indice<<endl;

        if ((rand() % 100) < 40) {
            goles++;
            // cout<<"gol "<<endl;
            // Acceso por puntero para actualizar al jugador original
            stats.Getplayer(equipo, indice)->gol();      //aqui ya se actualiza los goles del jugador
            stats.addGol(equipo);       //agrega enseguida los goles reales a las estadisticas del partido

        }
    }
    return goles;

}
void Partido::simularfaltas(){

    //simula las faltas del equipo 1 y equipo 2
    //faltas equipo 1
    for(unsigned short int i=0;i<11;i++){
        FaltasPlayer(stats.Getplayer(1,i));
    }
    //faltas equipo 2
    for(unsigned short int i=0;i<11;i++){
        FaltasPlayer(stats.Getplayer(2,i));
    }
}



void Partido::FaltasPlayer(jugadores* player) {
    unsigned int amarilla=player->getamarilla();

    // 1. Lógica de Faltas (13%, 2.75%, 0.7%)
    // Usamos 10000 para precisión de decimales
    if (rand() % 10000 < 1300) {
        player->fault();
        tryamarilla(amarilla,player);

        if (rand() % 10000 < 275)  {
            player->fault();
            player->amarilla();
            tryamarilla(amarilla,player);

            if (rand() % 10000< 70){
                player->fault();
                player->amarilla();

            }
        }

    }

}

void Partido::tryamarilla(unsigned int amarilla,jugadores* player){
    int rTarjeta = rand() % 10000;
    if (amarilla == 0) {
        if (rTarjeta < 600) { // 6%
            player->amarilla();
            return;
        }
    }else if (amarilla == 1) {
        if (rTarjeta < 115) { // 1.15%
            player->amarilla(); // Esto debería activar la roja automáticamente en tu clase Jugador
            return;
        }
    }else return;

}




int Partido::generarGoles(double lambda) {
    static default_random_engine generator(time(0));
    poisson_distribution<int> distribution(lambda);
    return distribution(generator);
}

void Partido::oncetitular(){
    stats.convocados(equipo1->convocados(),1);
    stats.convocados(equipo2->convocados(),2);
}


void Partido::showpartido(){
    // Verifica que los punteros no sean nulos antes de usarlos

        cout << equipo1->getname() << " vs " << equipo2->getname() << endl;
        // cout << "DT: " << equipo1->getmanager() << " | DT: " << equipo2->getmanager() << endl;
        // cout<<"goles equipo 1: "<<stats.getgol(1)<<endl;
        // cout<<"posicion equipo 1: "<<stats.getgol(1)<<endl;
        // cout<<"Titulares equipo 1"<<endl;
        // stats.showconvocados();


}


Selecciones* Partido:: getGanador(){
    if(resultado==0)return nullptr;
    if(resultado==1) return equipo1;
    else return equipo2;

}


Selecciones *Partido::getEquipo(int equipo){

    if(equipo==1) return equipo1;
    else if(equipo==2) return equipo2;
    else return nullptr;

}


unsigned int Partido:: getGol(unsigned int equipo) {
    return stats.getgol(equipo);
}






Partido::~Partido(){
    // delete equipo1;
    // delete equipo2;
}