#include "grupos.h"

Grupos::Grupos() {
    letra       = '?';
    numPartidos = 0;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++)  equipos[i] = nullptr;
    for (int i = 0; i < PARTIDOS_POR_GRUPO; i++) {
        golesE1[i] = golesE2[i] = 0;
        indE1[i]   = indE2[i]   = -1;
    }
    inicializarTabla();
}

Grupos::Grupos(char l, Selecciones* e1, Selecciones* e2,
               Selecciones* e3, Selecciones* e4) {
    letra       = l;
    numPartidos = 0;
    equipos[0]  = e1;  equipos[1] = e2;
    equipos[2]  = e3;  equipos[3] = e4;
    for (int i = 0; i < PARTIDOS_POR_GRUPO; i++) {
        golesE1[i] = golesE2[i] = 0;
        indE1[i]   = indE2[i]   = -1;
    }
    inicializarTabla();
    generarFixture();
}

void Grupos::inicializarTabla() {
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        tabla[i].equipo  = (i < EQUIPOS_POR_GRUPO) ? equipos[i] : nullptr;
        tabla[i].puntos  = 0;
        tabla[i].pj      = 0;
        tabla[i].pg      = 0;
        tabla[i].pe      = 0;
        tabla[i].pp      = 0;
        tabla[i].dg      = 0;
        tabla[i].gf      = 0;
    }
}

void Grupos::generarFixture() {
    // Todos contra todos una vez: pares (0,1),(0,2),(0,3),(1,2),(1,3),(2,3)
    numPartidos = 0;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        for (int j = i + 1; j < EQUIPOS_POR_GRUPO; j++) {
            indE1[numPartidos] = i;
            indE2[numPartidos] = j;
            numPartidos++;
        }
    }
    // numPartidos == 6 siempre
}

void Grupos::registrarResultado(int idxPartido, unsigned int g1, unsigned int g2) {
    if (idxPartido < 0 || idxPartido >= numPartidos) return;
    golesE1[idxPartido] = g1;
    golesE2[idxPartido] = g2;
}

void Grupos::generarTabla() {
    // Reinicia contadores
    inicializarTabla();
    // Asigna punteros de equipo a la tabla
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++)
        tabla[i].equipo = equipos[i];

    for (int p = 0; p < numPartidos; p++) {
        int    a  = indE1[p];
        int    b  = indE2[p];
        unsigned int ga = golesE1[p];
        unsigned int gb = golesE2[p];

        tabla[a].pj++;  tabla[b].pj++;
        tabla[a].gf += ga;  tabla[b].gf += gb;
        tabla[a].dg += (int)ga - (int)gb;
        tabla[b].dg += (int)gb - (int)ga;

        if (ga > gb) {
            tabla[a].pg++;  tabla[a].puntos += 3;
            tabla[b].pp++;
        } else if (ga < gb) {
            tabla[b].pg++;  tabla[b].puntos += 3;
            tabla[a].pp++;
        } else {
            tabla[a].pe++;  tabla[a].puntos++;
            tabla[b].pe++;  tabla[b].puntos++;
        }
    }

    // Ordenar tabla por puntos desc, luego dg desc, luego gf desc
    // Burbuja simple (4 elementos, no importa el costo)
    for (int i = 0; i < EQUIPOS_POR_GRUPO - 1; i++) {
        for (int j = 0; j < EQUIPOS_POR_GRUPO - 1 - i; j++) {
            bool swap = false;
            if (tabla[j].puntos < tabla[j+1].puntos) swap = true;
            else if (tabla[j].puntos == tabla[j+1].puntos &&
                     tabla[j].dg < tabla[j+1].dg)    swap = true;
            else if (tabla[j].puntos == tabla[j+1].puntos &&
                     tabla[j].dg == tabla[j+1].dg    &&
                     tabla[j].gf < tabla[j+1].gf)    swap = true;
            if (swap) {
                EntradaTabla tmp = tabla[j];
                tabla[j]         = tabla[j+1];
                tabla[j+1]       = tmp;
            }
        }
    }
}

// Getters
char         Grupos::getLetra()              const { return letra;          }
Selecciones* Grupos::getEquipo(int i)        const { return equipos[i];     }
EntradaTabla Grupos::getEntradaTabla(int i)  const { return tabla[i];       }
int          Grupos::getIndE1(int p)         const { return indE1[p];       }
int          Grupos::getIndE2(int p)         const { return indE2[p];       }
int          Grupos::getNumPartidos()        const { return numPartidos;     }
int Grupos::getPuntos(int equipo) const{return this->getEntradaTabla(equipo).puntos;}

void Grupos::imprimirTabla() const {
    cout << "GRUPO " << letra << endl;
    cout << "Equipo               | PJ | PG | PE | PP | DG | PTS" << endl;
    cout << "---------------------+----+----+----+----+----+----" << endl;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        if (!tabla[i].equipo) continue;
        string nombre = tabla[i].equipo->getname();
        // Padding nombre a 20 chars
        while ((int)nombre.size() < 20) nombre += ' ';
        cout << nombre << " | "
             << tabla[i].pj << "  | "
             << tabla[i].pg << "  | "
             << tabla[i].pe << "  | "
             << tabla[i].pp << "  | "
             << tabla[i].dg << "  | "
             << tabla[i].puntos << endl;
    }
    cout << endl;
}

void Grupos::imprimirGrupo() const {
    cout << "=== Grupo " << letra << " ===" << endl;
    for (int i = 0; i < EQUIPOS_POR_GRUPO; i++) {
        if (equipos[i])
            cout << "  " << equipos[i]->getname()
                 << " (Ranking " << equipos[i]->getRanking() << ")" << endl;
    }
    cout << endl;
}



Grupos::~Grupos() {
    // Los punteros a Selecciones son propiedad de Torneo; no se eliminan aquí
}
