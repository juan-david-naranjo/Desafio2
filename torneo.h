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
    void simularoctavos(Selecciones** ganadoreso16);
    void simularCuartos(Selecciones** ganadores8);
    void simularSemis(Selecciones** ganadores4);
    void simularTercerPuesto(Selecciones** perdedores2);
    void simularFinal(Selecciones** ganadores2);

    void prepararSorteo();
    bool backtrack(int grupoIdx,
                           int bomboIdx,
                           Selecciones* integrantes[4],
                           Selecciones** bombos[4]);
    bool esValido(Selecciones* candidato, Selecciones* actuales[], int numActuales);

    void imprimirGrupos() const;

};

#endif // TORNEO_H
