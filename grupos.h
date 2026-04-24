#ifndef GRUPOS_H
#define GRUPOS_H
#include "selecciones.h"

// Máximo partidos en fase de grupos: C(4,2) = 6
#define PARTIDOS_POR_GRUPO 6
#define EQUIPOS_POR_GRUPO  4

// Entrada de la tabla de clasificación
struct EntradaTabla {
    Selecciones* equipo;
    unsigned int puntos;
    unsigned int pj;       // partidos jugados
    unsigned int pg;       // ganados
    unsigned int pe;       // empatados
    unsigned int pp;       // perdidos
    int          dg;       // diferencia de goles
    unsigned int gf;       // goles a favor en el grupo
};

class Grupos {
    char         letra;
    Selecciones* equipos[EQUIPOS_POR_GRUPO];
    // Partidos se almacenan como pares de índices; la simulación
    // la hace Partido, aquí sólo guardamos resultados
    unsigned int golesE1[PARTIDOS_POR_GRUPO];
    unsigned int golesE2[PARTIDOS_POR_GRUPO];
    int          indE1[PARTIDOS_POR_GRUPO];   // índice dentro de equipos[]
    int          indE2[PARTIDOS_POR_GRUPO];
    int          numPartidos;

    EntradaTabla tabla[EQUIPOS_POR_GRUPO];

    void inicializarTabla();

public:
    Grupos();
    Grupos(char letra, Selecciones* equipo1, Selecciones* equipo2,
           Selecciones* equipo3, Selecciones* equipo4);

    // Genera la fixture interna (todos contra todos)
    void generarFixture();

    // Registra el resultado de un partido ya simulado
    void registrarResultado(int idxPartido, unsigned int g1, unsigned int g2);

    // Construye la tabla de posiciones a partir de los resultados
    void generarTabla();

    // Getters
    char          getLetra()                   const;
    Selecciones*  getEquipo(int i)             const;
    EntradaTabla  getEntradaTabla(int i)       const;
    int           getIndE1(int p)              const;
    int           getIndE2(int p)              const;
    int           getNumPartidos()             const;
    int           getPuntos(int equipo)        const;

    // Acceso directo a los clasificados (sin arreglo)
    Selecciones*  getPrimero()   const;
    Selecciones*  getSegundo()   const;
    Selecciones*  getTercero()   const;
    EntradaTabla  getEntradaTercero() const;   // para comparar terceros entre grupos


    // Imprime la tabla en consola (formato texto)
    void setLetra(char letter);
    void imprimirTabla()                       const;
    void imprimirGrupo()                       const;



    ~Grupos();
};

#endif // GRUPOS_H
