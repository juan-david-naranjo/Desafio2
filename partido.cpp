#include "partido.h"

Partido::Partido(Selecciones *team1,Selecciones *team2,string date,string hour) {

    equipo1=team1;
    equipo2=team2;
    fecha=date;
    hora=hour;
    stats.convocados(equipo1->convocados(),1);
    stats.convocados(equipo2->convocados(),2);
}


void Partido::simular(){
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

    int potencialGolesE1 = generarGoles(lambdaA);
    int golesRealesE1 = 0;

    cout<<"Ingreso"<<endl;
    for (int i = 0; i < potencialGolesE1; i++) {


        cout<<"Ingreso"<<endl;
        int indice = rand() % 11;


        if ((rand() % 100) < 4) {
            golesRealesE1++;
            // Acceso por puntero para actualizar al jugador original
            // stats.Getplayer(1, indice)->anotarGol();
            cout << "¡Gol validado de ";
            stats.Getplayer(1, indice)->getname();
        } else {
            cout << "El remate de ";
            stats.Getplayer(1, indice)->getname();
            cout<<" pegó en el palo (Falló el 4%)"<<endl;
        }
    }






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
        cout << "DT: " << equipo1->getmanager() << " | DT: " << equipo2->getmanager() << endl;
        cout<<"goles equipo 1: "<<stats.getgol(1)<<endl;
        cout<<"posicion equipo 1: "<<stats.getgol(1)<<endl;
        cout<<"Titulares equipo 1"<<endl;
        stats.showconvocados();


}












Partido::~Partido(){
    // delete equipo1;
    // delete equipo2;
}