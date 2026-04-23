#include "torneo.h"

torneo::torneo() {
    iteraciones = 0;
    Memoriausada = 0;
    gestorarchivos=nullptr;
    for(int i = 0; i < 48; i++) selecciones[i] = nullptr;
    for(int i = 0; i < 12; i++) grupo[i] = nullptr;
}


//          metodos
void torneo::cargarDatos(string rutacsv,string datos_jugadores){
    gestorarchivos=new GestorArchivos(rutacsv,datos_jugadores);
    gestorarchivos->leerSelecciones(selecciones,48);
    // sortearGrupos();
    testgrupos();
}


void torneo::simularTorneo(){



}


void torneo::imprimirCampeon(){}

void torneo::simularFaseGrupos() {
    cout << "------|Simulacion Fase de Grupos|------------" << endl;

    // 1. Generar todos los partidos de todos los grupos
    // Cada grupo tiene 6 partidos (todos contra todos entre 4 equipos)
    // Los fixtures son: (0v1), (0v2), (0v3), (1v2), (1v3), (2v3)
    int fixtures[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};

    const int TOTAL_PARTIDOS = 72; // 12 grupos x 6 partidos
    Partido* todosLosPartidos[TOTAL_PARTIDOS];
    int idx = 0;

    Fecha* fechas = new Fecha(2026, 6, 20);

    for (int gp = 0; gp < 12; gp++) {
        for (int f = 0; f < 6; f++) {
            Selecciones* E1 = grupo[gp]->getEquipo(fixtures[f][0]);
            Selecciones* E2 = grupo[gp]->getEquipo(fixtures[f][1]);
            todosLosPartidos[idx++] = new Partido(E1, E2, fechas);
        }
    }

    // 2. Simular de 4 en 4 por dia
    const int MAX_POR_DIA = 4;
    int jugadosHoy = 0;

    for (int i = 0; i < TOTAL_PARTIDOS; i++) {
        if (jugadosHoy == MAX_POR_DIA) {
            fechas->avanzarDia(); // avanza al siguiente dia
            jugadosHoy = 0;
            cout << "\n--- " << fechas->toString()<< " ---\n";
        }

        if (jugadosHoy == 0)
            cout << "\n--- " << fechas->toString() << " ---\n";

        todosLosPartidos[i]->simular(false);
        todosLosPartidos[i]->showpartido();
        jugadosHoy++;
    }

    // 3. Limpiar
    for (int i = 0; i < TOTAL_PARTIDOS; i++) {
        delete todosLosPartidos[i];
    }
    delete fechas;
}


void torneo::testgrupos(){
    for(int i=0;i<12;i++){
        grupo[i]= new Grupos((char)('A'+i),selecciones[i],selecciones[i+1],selecciones[i+2],selecciones[i+3]);
    }
}



void torneo::sortearGrupos() {
    prepararSorteo();

    // Llenar los 4 bombos de 12 equipos cada uno
    Selecciones* bombo1[12], *bombo2[12], *bombo3[12], *bombo4[12];
    for (int i = 0; i < 12; i++) {
        bombo1[i] = selecciones[i];
        bombo2[i] = selecciones[i + 12];
        bombo3[i] = selecciones[i + 24];
        bombo4[i] = selecciones[i + 36];
    }

    // Mezclar bombos 2, 3 y 4 aleatoriamente para variar el sorteo
    // (Fisher-Yates shuffle — requiere <cstdlib> y srand() en main)
    for (int i = 11; i > 0; i--) {
        int r;
        r = rand() % (i + 1); swap(bombo2[i], bombo2[r]);
        r = rand() % (i + 1); swap(bombo3[i], bombo3[r]);
        r = rand() % (i + 1); swap(bombo4[i], bombo4[r]);
    }

    // Arreglo de punteros a los bombos para pasarlos al backtracking
    Selecciones** bombos[4] = { bombo1, bombo2, bombo3, bombo4 };
    for(int bomboidx=0;bomboidx<4;bomboidx++){
    for (int i = 0; i < 12; i++) {
        Selecciones* integrantes[4] = { nullptr, nullptr, nullptr, nullptr };

        if (!backtrack(i, bomboidx, integrantes, bombos)) {
            cout << "ERROR: No se pudo armar el grupo " << (char)('A' + i)
            << ". Revisa la distribución de confederaciones.\n";
        }
    }
    }
}
void torneo::construirTabla(){}

void torneo::prepararSorteo() {
    // 1. Poner a EE.UU. en posición 0
    for (int i = 0; i < 48; i++) {
        if (selecciones[i]->getname() == "Estados Unidos" ||
            selecciones[i]->getname() == "USA") {
            Selecciones* tmp = selecciones[0];
            selecciones[0]   = selecciones[i];
            selecciones[i]   = tmp;
            break;
        }
    }

    // 2. Bubble sort del índice 1 al 47 (EE.UU. queda fijo en 0)
    //    BUG ORIGINAL: el límite era 47-i+1, provocando acceso a selecciones[48]
    for (int i = 1; i < 47; i++) {
        for (int j = 1; j < 48 - i; j++) {
            if (selecciones[j]->getRanking() >
                selecciones[j + 1]->getRanking()) {
                Selecciones* tmp   = selecciones[j];
                selecciones[j]     = selecciones[j + 1];
                selecciones[j + 1] = tmp;
            }
        }
    }
}

bool torneo::esValido(Selecciones* candidato,
                      Selecciones* actuales[],
                      int numActuales) {
    string confCand = candidato->getConfederacion();
    int conteoUEFA  = (confCand == "UEFA") ? 1 : 0;

    for (int i = 0; i < numActuales; i++) {
        if (actuales[i] == nullptr) continue; // ← protección clave

        string confAct = actuales[i]->getConfederacion();

        if (confAct == confCand && confCand != "UEFA")
            return false;

        if (confAct == "UEFA") conteoUEFA++;
    }

    return conteoUEFA <= 2;
}


// ─── BACKTRACKING ──────────────────────────────────────────────────────────
// Intenta asignar un equipo de cada bombo al grupo `grupoIdx`.
// Retorna true si lo logra; deja los bombos en el estado correcto si falla.
bool torneo::backtrack(int grupoIdx,        // grupo que estamos armando (0-11)
                       int bomboIdx,        // bombo actual (0-3)
                       Selecciones* integrantes[4],
                       Selecciones** bombos[4]) {

    if (bomboIdx == 4) {
        // Todos los bombos asignados → crear el grupo
        grupo[grupoIdx] = new Grupos('A' + grupoIdx, integrantes[0],integrantes[1],integrantes[2],integrantes[3]);
        return true;
    }

    Selecciones** bomboActual = bombos[bomboIdx];

    for (int j = 0; j < 12; j++) {
        if (bomboActual[j] == nullptr) continue;

        if (esValido(bomboActual[j], integrantes, bomboIdx)) {
            // Elegir
            integrantes[bomboIdx] = bomboActual[j];
            bomboActual[j]        = nullptr;

            // Recurrir al siguiente bombo
            if (backtrack(grupoIdx, bomboIdx + 1, integrantes, bombos))
                return true;

            // Deshacer (backtrack)
            bomboActual[j]        = integrantes[bomboIdx];
            integrantes[bomboIdx] = nullptr;


        }
    }

    return false; // ningún candidato funcionó
}





void torneo::imprimirGrupos() const {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║          SORTEO MUNDIAL 2026 — GRUPOS            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // for (int i = 0; i < 12; i++) {
    //     cout << "\n  GRUPO " << (char)('A' + i) << "\n";
    //     cout << "  ┌──────────────────────────────────────────┐\n";

    //     // Selecciones** miembros = grupo[i]->showplayers(); // ajusta según tu API
    //     for (int j = 0; j < 4; j++) {
    //         cout << "  │  [Bombo " << (j + 1) << "] "
    //              << left << setw(22) << miembros[j]->getname()
    //              << " (" << miembros[j]->getConfederacion() << ")\n";
    //     }
    //     cout << "  └──────────────────────────────────────────┘\n";
    // }
}