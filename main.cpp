#include <iostream>
#include "selecciones.h"
#include "partido.h"
#include "torneo.h"
#include "medidor.h"
using namespace std;

int main()
{
    srand(time(NULL));
    // srand(time(nullptr));
    // Selecciones españa=Selecciones("España","Escaloni",2);
    // Selecciones colombia=Selecciones("Colombia","lorenzo",6);

    // Partido partido(&españa,&colombia,"21/06/2026","10:10");
    // partido.simular(true);

    // españa.showstats();
    // partido.showstats();
    string seleccion = "C:/Users/naran/Informatica2/Desafio2/selecciones_mundial.csv";
    string Registroplayers="Registro_jugadores";
    torneo Mundial_2026;
    // Mundial_2026.cargarDatos(seleccion,Registroplayers);
    // Mundial_2026.simularFaseGrupos();
    // Mundial_2026.simularDieciseisavos();
    // Mundial_2026.guardarDatos();
    Medidor::medirTamanios();


    cout<<"Hola mundo"<<endl;
    // cout<<"problema"<<endl;
    return 0;
}
