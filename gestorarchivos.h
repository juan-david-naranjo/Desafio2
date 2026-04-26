#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H
#include "selecciones.h"
#include "medidor.h"
#include <fstream>

class GestorArchivos {
    string rutaCSV;
    string rutaJugadores;

    // Helper: lee una línea y la divide por el delimitador dado
    // Retorna el número de campos leídos (máx maxCampos)
    int parsearLinea(const string& linea, char delim,
                     string* campos, int maxCampos) const;

    // Helper: elimina espacios/retornos al inicio y fin de un string
    string trim(const string& s) const;

public:
    GestorArchivos(const string& csv, const string& jugadores);

    // Lee el CSV y llena el arreglo equipos[] (ya reservado con tamaño 48)
    // Retorna la cantidad real de equipos leídos
    int leerSelecciones(Selecciones** equipos, int maxEquipos) const;

    // Guarda stats históricas de jugadores de todos los equipos
    // Formato: una línea por jugador  →  pais;camiseta;goles;minutos;amarillas;rojas;faltas
    void guardarJugadores(Selecciones** equipos, int numEquipos) const;
    void actualizarSelecciones(Selecciones** equipos, int numEquipos) const;

    // Carga stats históricas de jugadores desde el archivo de jugadores
    void leerJugadores(Selecciones** equipos, int numEquipos) const;

    ~GestorArchivos();
};

#endif // GESTORARCHIVOS_H
