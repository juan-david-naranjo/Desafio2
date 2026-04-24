// medidor.cpp
#include "medidor.h"
#include <iostream>
using namespace std;

Medidor::Evento Medidor::eventos[500];
int             Medidor::numEventos = 0;

void Medidor::registrarCreacion(const string& clase, const void* direccion) {
    if (numEventos >= 500) return;
    eventos[numEventos++] = { clase, direccion, clock(), true };
}

void Medidor::registrarDestruccion(const string& clase, const void* direccion) {
    if (numEventos >= 500) return;
    eventos[numEventos++] = { clase, direccion, clock(), false };
}

void Medidor::imprimirReporte() {
    cout << "\n+========================================================+\n";
    cout << "|           REPORTE TIEMPO DE VIDA OBJETOS               |\n";
    cout << "+========================================================+\n";
    cout << "  Clase          Direccion    Vida (ms)\n";
    cout << "  ──────────────────────────────────────\n";

    for (int i = 0; i < numEventos; i++) {
        if (!eventos[i].esCreacion) continue;

        // Buscar su destruccion correspondiente
        clock_t fin = -1;
        for (int j = i + 1; j < numEventos; j++) {
            if (!eventos[j].esCreacion &&
                eventos[j].direccion == eventos[i].direccion &&
                eventos[j].clase     == eventos[i].clase) {
                fin = eventos[j].timestamp;
                break;
            }
        }

        cout << "  ";
        string clase = eventos[i].clase;
        while ((int)clase.size() < 15) clase += ' ';
        cout << clase;

        // Imprimir direccion como numero
        cout << "  0x" << eventos[i].direccion << "  ";

        if (fin != -1) {
            double ms = (double)(fin - eventos[i].timestamp)
            / CLOCKS_PER_SEC * 1000.0;
            cout << ms << " ms\n";
        } else {
            cout << "aun vivo\n";
        }
    }
    cout << "+========================================================+\n";
}

void Medidor::medirTamanios() {
    cout << "\n+========================================================+\n";
    cout << "|              TAMAÑO DE OBJETOS EN MEMORIA              |\n";
    cout << "+========================================================+\n";
    cout << "  sizeof(Selecciones)    : " << sizeof(Selecciones)    << " bytes\n";
    cout << "  sizeof(Grupos)         : " << sizeof(Grupos)         << " bytes\n";
    cout << "  sizeof(Partido)        : " << sizeof(Partido)        << " bytes\n";
    cout << "  sizeof(Fecha)          : " << sizeof(Fecha)          << " bytes\n";
    cout << "  sizeof(EntradaTabla)   : " << sizeof(EntradaTabla)   << " bytes\n";
    cout << "  sizeof(GestorArchivos) : " << sizeof(GestorArchivos) << " bytes\n";
    cout << "+========================================================+\n";
}