#include <iostream>

#include "torneo.h"
#include "medidor.h"
using namespace std;

int main()
{
    srand(time(NULL));
    string seleccion = "C:/Users/naran/Informatica2/Desafio2/selecciones_mundial.csv";
    string Registroplayers="Registro_jugadores";
    torneo Mundial_2026;
    Mundial_2026.cargarDatos(seleccion,Registroplayers);
    Mundial_2026.simularTorneo();
    Mundial_2026.tamanioTorneo();
    Mundial_2026.pausar("Pasar a Medidas de Eficiencia");
    Medidor::imprimirReporte();



    // cout<<"problema"<<endl;
    return 0;
}
