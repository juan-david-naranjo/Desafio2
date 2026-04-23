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

    int fixtures[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
    const int TOTAL_PARTIDOS = 72;

    Partido* todosLosPartidos[TOTAL_PARTIDOS];
    Fecha* fechas = new Fecha(2026, 6, 20);

    // Llenar el arreglo en orden: 6 partidos del grupo A, luego B, etc.
    int idx = 0;
    for (int gp = 0; gp < 12; gp++) {
        // grupo[gp]->imprimirGrupo();
        for (int f = 0; f < 6; f++) {
            Selecciones* E1 = grupo[gp]->getEquipo(fixtures[f][0]);
            Selecciones* E2 = grupo[gp]->getEquipo(fixtures[f][1]);
            todosLosPartidos[idx++] = new Partido(E1, E2, fechas);          //arreglo con todos los partidos
        }
    }

    // partido[gp*6 + f] = partido f del grupo gp
    // Dia d juega: fixture f = d/3, grupos gp = (d%3)*4 hasta (d%3)*4 + 3
    for (int dia = 0; dia < 18; dia++) {
        cout << "\n--- " << fechas->toString() << " ---\n";

        int f  = dia / 3;       // que fixture toca (0 al 5)
        int g0 = (dia % 3) * 4; // primer grupo del dia (0, 4, u 8)


        //ya sabemos el orden de los partidos entonces unicamente, ecogemos en bloques de 4 grupos, los primeros fixtures por dia
        for (int gp = g0; gp < g0 + 4; gp++) {
            Partido* p = todosLosPartidos[gp * 6 + f];
            p->simular(false);
            // p->showpartido();
            unsigned int E1gol=p->getGol(1);
            unsigned int E2gol=p->getGol(2);

            cout<<"GOL equipo 1 "<<E1gol<<endl;
            cout<<"GOL equipo 2 "<<E2gol<<endl;
            grupo[gp]->registrarResultado(f,E1gol,E2gol);        //aqui deberia actualizarse la tabla
        }
        fechas->avanzarDia();
    }


    // Limpiar
    for (int i = 0; i < TOTAL_PARTIDOS; i++)
        delete todosLosPartidos[i];
    delete fechas;
    for(unsigned int i=0;i<12;i++){
        grupo[i]->imprimirTabla();          //visualizamos como quedo la tabla
    }
}


void torneo::testgrupos(){
    for(int i=0;i<12;i+=1){
        // cout<<selecciones[i]->getname()<<endl;
        // cout<<selecciones[i+12]->getname()<<endl;
        // cout<<selecciones[i+24]->getname()<<endl;
        // cout<<selecciones[i+36]->getname()<<endl;
        grupo[i]= new Grupos((char)('A'+i),selecciones[i],selecciones[i+12],selecciones[i+24],selecciones[i+36]);
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