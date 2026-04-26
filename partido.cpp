#include "partido.h"

Partido::Partido(Selecciones *team1,Selecciones *team2,Fecha *date) {

    equipo1=team1;
    equipo2=team2;
    resultado=0;
    fecha=date;
    porroga=false;
    stats.convocados(equipo1->convocados(),1);
    stats.convocados(equipo2->convocados(),2);
    Medidor::registrarCreacion("Partido",this);
}

void Partido::simular(bool esEliminatoria) {
    int Rank1=equipo1->getRanking();
    int Rank2=equipo2->getRanking();
    if (esEliminatoria) {
        simularEliminatoria(); // partido con porroga
        // porroga=true;
        stats.SetPartido(porroga);
        stats.calcularPosesion(Rank1,Rank2);

    } else {
        simularFaseGrupos();   // La nueva versión sin prórroga
        stats.SetPartido(porroga);
        stats.calcularPosesion(Rank1,Rank2);
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
    int golesE1 = generarGoles(lambdaA);
    int golesE2 = generarGoles(lambdaB);



    int golesRealesE1=Golesreales(1,golesE1);
    int golesRealesE2=Golesreales(2,golesE2);

    // Asignar goles a jugadores individuales
    // asignarGolesJugadores(1, golesRealesE1);
    // asignarGolesJugadores(2, golesRealesE2);

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
    int golesE1 = generarGoles(lambdaA);
    int golesE2 = generarGoles(lambdaB);
    // Asignar goles a jugadores individuales
    // asignarGolesJugadores(1, golesRealesE1);
    // asignarGolesJugadores(2, golesRealesE2);
    int golesRealesE1=Golesreales(1,golesE1);
    int golesRealesE2=Golesreales(2,golesE2);



    //actualizamos estadisticas de los equipos
    equipo1->actualizarstats(golesRealesE1, golesRealesE2);
    equipo2->actualizarstats(golesRealesE2,golesRealesE1);


    if(golesRealesE1==golesRealesE2){
        porroga=true;
        Manejarempate(golesRealesE1,golesRealesE2);

    }
    else if(golesRealesE1>golesRealesE2){
        // cout<<"Gano el equipo 1"<<endl;

        equipo1->wingame();
        equipo2->losegame();
        resultado=1;
    }else{
        // cout<<"Gano el equipo 2"<<endl;
        equipo2->wingame();
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

    int golesE1 = /*golesE1Reg + */generarGoles(lambdaA);
    int golesE2 = /*golesE2Reg +*/ generarGoles(lambdaB);



    // Asignar goles a jugadores individuales en la porroga
    int golesRealesE1=Golesreales(1,golesE1);
    int golesRealesE2=Golesreales(2,golesE2);

    equipo1->actualizarstats(golesRealesE1, golesRealesE2);
    equipo2->actualizarstats(golesRealesE2, golesRealesE1);


    if(golesE1==golesE2){
        // Penales: 50/50
        int prob=rand()%2;
        if(prob==1){

            equipo1->wingame();
            equipo2->losegame();
            resultado=1;
        }else{
            equipo1->losegame();
            equipo2->wingame();
            resultado=2;
        }
    }else if(golesE1>golesE2){
        equipo1->wingame();
        equipo2->losegame();
        resultado=1;
    }else{
        equipo2->wingame();
        equipo1->losegame();
        resultado=2;
    }
    // asignarGolesJugadores(1, generarGoles(lambdaA));
    // asignarGolesJugadores(2, generarGoles(lambdaB));
    simularfaltas();
}







// Distribuye 'numGoles' goles ya calculados entre los 11 titulares del equipo.
// Reemplaza a Golesreales: la Poisson ya es el resultado final, no un potencial.
void Partido::asignarGolesJugadores(short int equipo, int numGoles){
    for (int i = 0; i < numGoles; i++) {
        int indice = rand() % 11;
        jugadores* jug = stats.Getplayer(equipo, indice);
        if (jug) {
            jug->gol();         //se actualiza de manera generallas estadistica
            stats.registrarEvento(equipo, indice, 'g'); // partido
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
            stats.registrarEvento(equipo, indice, 'g'); // gol del jugador por partido
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
        FaltasPlayer(1,stats.Getplayer(1,i),i);
    }
    //faltas equipo 2
    for(unsigned short int i=0;i<11;i++){
        FaltasPlayer(2,stats.Getplayer(2,i),i);
    }
}



void Partido::FaltasPlayer(int equipo,jugadores* player,int indice) {
    unsigned int amarilla=player->getamarilla();

    // 1. Lógica de Faltas (13%, 2.75%, 0.7%)
    // Usamos 10000 para precisión de decimales
    if (rand() % 10000 < 1300) {
        player->fault();
        stats.registrarEvento(equipo, indice, 'f');
        tryamarilla(amarilla,player,equipo,indice);

        if (rand() % 10000 < 275)  {
            player->fault();
            player->amarilla();
            stats.registrarEvento(equipo, indice, 'f');
            stats.registrarEvento(equipo, indice, 'a');
            tryamarilla(amarilla,player,equipo,indice);

            if (rand() % 10000< 70){
                player->fault();
                player->amarilla();
                stats.registrarEvento(equipo, indice, 'f');
                stats.registrarEvento(equipo, indice, 'a');

            }
        }

    }

}

void Partido::tryamarilla(unsigned int amarilla,jugadores* player,int equipo,int indice){
    int rTarjeta = rand() % 10000;
    if (amarilla == 0) {
        if (rTarjeta < 600) { // 6%
            player->amarilla();
            stats.registrarEvento(equipo, indice, 'a');
            return;
        }
    }else if (amarilla == 1) {
        if (rTarjeta < 115) { // 1.15%
            player->amarilla(); // Esto debería activar la roja automáticamente en la clase Jugador
            stats.registrarEvento(equipo, indice, 'r');
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

    // Nombres y DTs
    string nom1 = equipo1->getname(); while((int)nom1.size() < 20) nom1 += ' ';
    string nom2 = equipo2->getname(); while((int)nom2.size() < 20) nom2 += ' ';
    string dt1  = equipo1->getmanager(); while((int)dt1.size() < 20) dt1 += ' ';
    string dt2  = equipo2->getmanager(); while((int)dt2.size() < 20) dt2 += ' ';

    cout << "  |  " << nom1 << "  vs  " << nom2 << "  |\n";
    cout << "  |  DT: " << dt1 << "      DT: " << dt2 << "  |\n";
    cout << "  +--------------------------------------------------+\n";

    // Resultado
    cout << "  |  GOLES   :  " << stats.getgol(1)
         << "  -  "            << stats.getgol(2) << "\n";
    cout << "  |  POSESION:  " << stats.getPosesion(1) << "%"
         << "  -  "            << stats.getPosesion(2) << "%\n";

    // Prorroga
    if (porroga) {
        if (resultado == 0)
            cout << "  |  >> Empate en prorroga -> PENALES\n";
        else
            cout << "  |  >> Se definio en prorroga\n";
    }
    // Ganador
    Selecciones* gan = getGanador();
    if (gan)
        cout << "  |  GANADOR : " << gan->getname() << "\n";
    else
        cout << "  |  EMPATE  (fase de grupos)\n";

    cout << "  +--------------------------------------------------+\n";

    // Goleadores, tarjetas y faltas usando getStatsJugador
    cout << "  |  GOLEADORES:\n";
    bool hayGoles = false;
    for (int eq = 1; eq <= 2; eq++) {
        for (int i = 0; i < 11; i++) {
            jugadores* jug = stats.Getplayer(eq, i);
            if (!jug) continue;
            const StatsJugadorPartido& s = stats.getStatsJugador(eq, i);
            if (s.goles > 0) {
                cout << "  |    * " << jug->getname() << " ("
                     << (eq==1 ? equipo1->getname() : equipo2->getname())
                     << ") - " << s.goles << " gol(es)\n";
                hayGoles = true;
            }
        }
    }
    if (!hayGoles) cout << "  |    Sin goles\n";

    cout << "  |  TARJETAS Y FALTAS:\n";
    bool hayTarjetas = false;
    for (int eq = 1; eq <= 2; eq++) {
        for (int i = 0; i < 11; i++) {
            jugadores* jug = stats.Getplayer(eq, i);
            if (!jug) continue;
            const StatsJugadorPartido& s = stats.getStatsJugador(eq, i);
            if (s.amarillas > 0 || s.rojas > 0 || s.faltas > 0) {
                cout << "  |    " << jug->getname()<< " ("
                << (eq==1 ? equipo1->getname() : equipo2->getname())<< ") - "
                << " - " << s.faltas    << " falta(s)"
                << "  "   << s.amarillas << " amarilla(s)"
                << "  "   << s.rojas     << " roja(s)\n";
                hayTarjetas = true;
            }
        }
    }
    if (!hayTarjetas) cout << "  |    Sin tarjetas\n";
    cout << "  +--------------------------------------------------+\n";




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
    Medidor::registrarDestruccion("Partido",this);
    // delete equipo1;
    // delete equipo2;
}