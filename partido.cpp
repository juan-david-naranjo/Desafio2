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
    // Partidos jugados históricamente por cada equipo (mínimo 1 para evitar /0)
    unsigned int pjA = equipo1->getWin() + equipo1->getLose() + equipo1->getDraws();
    unsigned int pjB = equipo2->getWin() + equipo2->getLose() + equipo2->getDraws();
    if (pjA == 0) pjA = 1;
    if (pjB == 0) pjB = 1;

    // Promedios de goles por partido (valores alrededor de 1.0-2.0, acordes con µ=1.35)
    double gfA = (double)equipo1->favorGoals()  / pjA;
    double gfB = (double)equipo2->favorGoals()  / pjB;
    double gcA = (double)equipo1->counterGoals()/ pjA;
    double gcB = (double)equipo2->counterGoals()/ pjB;

    // Valores mínimos para evitar pow(0, alpha) = 0
    if (gfA < 0.1) gfA = 0.1;
    if (gfB < 0.1) gfB = 0.1;
    if (gcA < 0.1) gcA = 0.1;
    if (gcB < 0.1) gcB = 0.1;

    // Constantes fijas del modelo
    const double mu    = 1.35;
    const double alpha = 0.6;
    const double beta  = 0.4;

    // λ_A = µ * (GF_A / µ)^α * (GC_B / µ)^β
    double lambdaA = mu * pow(gfA / mu, alpha) * pow(gcB / mu, beta);
    // λ_B = µ * (GF_B / µ)^α * (GC_A / µ)^β
    double lambdaB = mu * pow(gfB / mu, alpha) * pow(gcA / mu, beta);

    // La distribución de Poisson ya entrega los goles del partido directamente
    int golesRealesE1 = generarGoles(lambdaA);
    int golesRealesE2 = generarGoles(lambdaB);

    // Asignar goles a jugadores individuales
    asignarGolesJugadores(1, golesRealesE1);
    asignarGolesJugadores(2, golesRealesE2);

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
    // Partidos jugados históricamente (mínimo 1 para evitar /0)
    unsigned int pjA = equipo1->getWin() + equipo1->getLose() + equipo1->getDraws();
    unsigned int pjB = equipo2->getWin() + equipo2->getLose() + equipo2->getDraws();
    if (pjA == 0) pjA = 1;
    if (pjB == 0) pjB = 1;

    // Promedios de goles por partido
    double gfA = (double)equipo1->favorGoals()  / pjA;
    double gfB = (double)equipo2->favorGoals()  / pjB;
    double gcA = (double)equipo1->counterGoals()/ pjA;
    double gcB = (double)equipo2->counterGoals()/ pjB;

    if (gfA < 0.1) gfA = 0.1;
    if (gfB < 0.1) gfB = 0.1;
    if (gcA < 0.1) gcA = 0.1;
    if (gcB < 0.1) gcB = 0.1;

    // Constantes fijas del modelo
    const double mu    = 1.35;
    const double alpha = 0.6;
    const double beta  = 0.4;

    double lambdaA = mu * pow(gfA / mu, alpha) * pow(gcB / mu, beta);
    double lambdaB = mu * pow(gfB / mu, alpha) * pow(gcA / mu, beta);

    // Goles directamente desde Poisson
    int golesRealesE1 = generarGoles(lambdaA);
    int golesRealesE2 = generarGoles(lambdaB);


    if(golesRealesE1==golesRealesE2){
        porroga=true;
        Manejarempate(golesRealesE1,golesRealesE2);

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

void Partido::Manejarempate(int golesE1Reg, int golesE2Reg){
    // Prórroga: se generan nuevos goles con lambdas reducidas (factor 0.5 → ~30 min extra)
    unsigned int pjA = equipo1->getWin() + equipo1->getLose() + equipo1->getDraws();
    unsigned int pjB = equipo2->getWin() + equipo2->getLose() + equipo2->getDraws();
    if (pjA == 0) pjA = 1;
    if (pjB == 0) pjB = 1;

    double gfA = (double)equipo1->favorGoals()  / pjA;
    double gfB = (double)equipo2->favorGoals()  / pjB;
    double gcA = (double)equipo1->counterGoals()/ pjA;
    double gcB = (double)equipo2->counterGoals()/ pjB;
    if (gfA < 0.1) gfA = 0.1;
    if (gfB < 0.1) gfB = 0.1;
    if (gcA < 0.1) gcA = 0.1;
    if (gcB < 0.1) gcB = 0.1;

    const double mu = 1.35, alpha = 0.6, beta = 0.4;
    // Prórroga ≈ 30 min extra → escalar lambda por 30/90 ≈ 0.33
    double lambdaA = mu * pow(gfA / mu, alpha) * pow(gcB / mu, beta) * 0.33;
    double lambdaB = mu * pow(gfB / mu, alpha) * pow(gcA / mu, beta) * 0.33;

    int golesE1 = golesE1Reg + generarGoles(lambdaA);
    int golesE2 = golesE2Reg + generarGoles(lambdaB);

    if(golesE1==golesE2){
        // Penales: 50/50
        int prob=rand()%2;
        if(prob==1){
            equipo1->actualizarstats(golesE1,golesE2);
            equipo1->wingame();
            resultado=1;
        }else{
            equipo2->actualizarstats(golesE2,golesE1);
            equipo2->wingame();
            resultado=2;
        }
    }else if(golesE1>golesE2){
        equipo1->actualizarstats(golesE1,golesE2);
        equipo1->wingame();
        resultado=1;
    }else{
        equipo2->actualizarstats(golesE2,golesE1);
        equipo2->wingame();
        resultado=2;
    }
    asignarGolesJugadores(1, generarGoles(lambdaA));
    asignarGolesJugadores(2, generarGoles(lambdaB));
    simularfaltas();
}







// Distribuye 'numGoles' goles ya calculados entre los 11 titulares del equipo.
// Reemplaza a Golesreales: la Poisson ya es el resultado final, no un potencial.
void Partido::asignarGolesJugadores(short int equipo, int numGoles){
    for (int i = 0; i < numGoles; i++) {
        int indice = rand() % 11;
        jugadores* jug = stats.Getplayer(equipo, indice);
        if (jug) {
            jug->gol();
            stats.addGol(equipo);
        }
    }
}

// Conservado por compatibilidad (ya no se llama internamente)
int Partido::Golesreales(short int equipo,int potencialgoles){
    int goles=0;
    for (int i = 0; i < potencialgoles; i++) {
        int indice = rand() % 11;
        if ((rand() % 100) < 40) {
            goles++;
            stats.Getplayer(equipo, indice)->gol();
            stats.addGol(equipo);
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