#ifndef TORNEO_H
#define TORNEO_H
#include "selecciones.h"
#include "partido.h"
#include "grupos.h"
#include "gestorarchivos.h"
#include <algorithm>
 #include <iomanip>


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

    void testgrupos();

    void construirTabla();

    void simularTorneo();

    void imprimirCampeon();

    void simularFaseGrupos();

    void sortearGrupos();
    void simularDieciseisavos();

    void prepararSorteo();
    bool backtrack(int grupoIdx,
                           int bomboIdx,
                           Selecciones* integrantes[4],
                           Selecciones** bombos[4]);
    bool esValido(Selecciones* candidato, Selecciones* actuales[], int numActuales);

    void imprimirGrupos() const;

};

#endif // TORNEO_H
