// medidor.h
#ifndef MEDIDOR_H
#define MEDIDOR_H
#include <ctime>
#include <string>
#include "fecha.h"
#include "gestorarchivos.h"
#include "jugadores.h"
#include "partido.h"
#include "selecciones.h"
#include "statspartido.h"
#include "grupos.h"
#include "statsteam.h"
#include "torneo.h"

using namespace std;

class Medidor {
public:
    static void registrarCreacion(const string& clase, const void* direccion);
    static void registrarDestruccion(const string& clase, const void* direccion);
    static void imprimirReporte();
    static void medirTamanios();

private:
    struct Evento {
        string      clase;
        const void* direccion;
        clock_t     timestamp;
        bool        esCreacion; // true=creacion, false=destruccion
    };

    static Evento eventos[500]; // suficiente para el torneo
    static int    numEventos;
};
#endif
