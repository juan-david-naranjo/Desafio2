#ifndef TORNEO_H
#define TORNEO_H
#include "selecciones.h"
#include "partido.h"
#include "grupos.h"
#include "gestorarchivos.h"
#include <algorithm>

// #include <iomanip>


using namespace std;

class torneo
{
    GestorArchivos *gestorarchivos;
    Selecciones *selecciones[48];
    Grupos *grupo[12];
    long iteraciones;
    long Memoriausada;

public:
    torneo();
    void cargarDatos(string rutacsv,string datos_jugadores);
    void guardarDatos();

    void testgrupos();

    void construirTabla();

    void simularTorneo();

    void imprimirCampeon();

    void simularFaseGrupos();

    void sortearGrupos();

    void simularDieciseisavos();
    void simularoctavos(Selecciones** ganadoreso16);
    void simularCuartos(Selecciones** ganadores8);
    void simularSemis(Selecciones** ganadores4);
    void simularTercerPuesto(Selecciones** perdedores2);
    void simularFinal(Selecciones** ganadores2);



    void prepararSorteo();

    void ConfMasEquipos(Selecciones** equipos, int cantidad, const string& fase) const;
    void tamanioTorneo();
    void imprimirGrupos() const;
    void limpiarConsola() const;
    void pausar(const string& mensaje) const;
    void MaximoGoleadores()const;
    char getGrupoDeEquipo(Selecciones* e);
    bool cruceValido(Selecciones* e1, Selecciones* e2);
    void GoleadorCampeon(Selecciones* campeon) const;

    ~torneo();

};

#endif // TORNEO_H
