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

void torneo::guardarDatos(){
    gestorarchivos->guardarJugadores(selecciones,48);
    gestorarchivos->actualizarSelecciones(selecciones,48);
}

void torneo::simularTorneo(){
    //funcion que simula todo el torneo

    simularFaseGrupos();
    simularDieciseisavos();

}


void torneo::imprimirCampeon(){}

void torneo::simularFaseGrupos() {
    cout << "\n+========================================================+\n";
    cout << "|          SIMULACION - FASE DE GRUPOS                   |\n";
    cout << "+========================================================+\n";

    int fixtures[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
    const int TOTAL_PARTIDOS = 72;

    Partido* todosLosPartidos[TOTAL_PARTIDOS];
    Fecha* fechas = new Fecha(2026, 6, 20);

    int idx = 0;
    for (int gp = 0; gp < 12; gp++) {
        for (int f = 0; f < 6; f++) {
            Selecciones* E1 = grupo[gp]->getEquipo(fixtures[f][0]);
            Selecciones* E2 = grupo[gp]->getEquipo(fixtures[f][1]);
            todosLosPartidos[idx++] = new Partido(E1, E2, fechas);
        }
    }

    for (int dia = 0; dia < 18; dia++) {
        cout << "\n  +-[ " << fechas->toString() << " ]";
        // Rellenar hasta ancho fijo
        cout << "-----------------------------------+\n";

        int f  = dia / 3;
        int g0 = (dia % 3) * 4;

        for (int gp = g0; gp < g0 + 4; gp++) {
            Partido* p = todosLosPartidos[gp * 6 + f];
            p->simular(false);
            unsigned int E1gol = p->getGol(1);
            unsigned int E2gol = p->getGol(2);
            grupo[gp]->registrarResultado(f, E1gol, E2gol);

            string nom1 = p->getEquipo(1)->getname();
            string nom2 = p->getEquipo(2)->getname();
            while ((int)nom1.size() < 20) nom1 += ' ';
            while ((int)nom2.size() < 20) nom2 += ' ';

            // Resultado compacto
            cout << "  |  [Grupo " << (char)('A' + gp) << "]  "
                 << nom1 << "  " << E1gol << " - " << E2gol
                 << "  " << nom2;

            Selecciones* ganador = p->getGanador();
            if (ganador)
                cout << "  =>  Gana: " << ganador->getname() << "\n";
            else
                cout << "  =>  Empate\n";

            // Estadisticas del partido: cabecera
            cout << "  |    +-- Stats jugadores destacados ";
            cout << "----------------------------+\n";

            // Mostrar los 11 titulares de cada equipo con sus stats
            // usando un formato de tabla ASCII compacto
            cout << "  |    | # Jugador                Goles  Min  Amar  Roj  Falt |\n";
            cout << "  |    +------------------------------------------------------+\n";

            p->showplayerstats();
            // Llamada al metodo existente para stats completos
            cout << "  |    +--------------------------------------------------+\n";
            p->showstats();
            cout << "  |\n";
        }
        cout << "  +------------------------------------------------------+\n";
        fechas->avanzarDia();
    }

    for (int i = 0; i < TOTAL_PARTIDOS; i++) delete todosLosPartidos[i];
    delete fechas;

    for (unsigned int i = 0; i < 12; i++) grupo[i]->generarTabla();

    // Imprimir tablas con bordes ASCII
    cout << "\n+========================================================+\n";
    cout << "|              TABLAS DE POSICIONES                      |\n";
    cout << "+========================================================+\n";
    for (unsigned int i = 0; i < 12; i++)
        grupo[i]->imprimirTabla();
}

void torneo::simularDieciseisavos() {

    Selecciones* primeros[12];
    Selecciones* segundos[12];
    EntradaTabla terceros[12];

    for (int i = 0; i < 12; i++) {
        primeros[i] = grupo[i]->getPrimero();
        segundos[i] = grupo[i]->getSegundo();
        terceros[i] = grupo[i]->getEntradaTercero();
    }

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11 - i; j++) {
            bool intercambiar =
                terceros[j].puntos < terceros[j+1].puntos ||
                (terceros[j].puntos == terceros[j+1].puntos &&
                 terceros[j].dg     < terceros[j+1].dg);
            if (intercambiar) {
                EntradaTabla tmp = terceros[j];
                terceros[j]      = terceros[j+1];
                terceros[j+1]    = tmp;
            }
        }
    }

    Selecciones* clasificados[32];
    for (int i = 0; i < 12; i++) clasificados[i]      = primeros[i];
    for (int i = 0; i < 12; i++) clasificados[i + 12] = segundos[i];
    for (int i = 0; i < 8;  i++) clasificados[i + 24] = terceros[i].equipo;

    cout << "\n+========================================================+\n";
    cout << "|           8 MEJORES TERCEROS CLASIFICADOS              |\n";
    cout << "+========================================================+\n";
    for (int i = 0; i < 8; i++) {
        string nom = clasificados[i+24]->getname();
        while ((int)nom.size() < 24) nom += ' ';
        cout << "  | " << (i+1) << ". " << nom
             << "  Pts: " << terceros[i].puntos
             << "  DG: "  << terceros[i].dg << "\n";
    }
    cout << "+========================================================+\n";

    cout << "\n+========================================================+\n";
    cout << "|            DIECISEISAVOS DE FINAL                      |\n";
    cout << "+========================================================+\n";
    Selecciones* ganadores[16];
    Fecha* fechas = new Fecha(2026, 7, 1);

    for (int i = 0; i < 16; i++) {
        Selecciones* E1 = clasificados[i];
        Selecciones* E2 = clasificados[31 - i];
        Partido* p = new Partido(E1, E2, fechas);
        p->simular(true);
        ganadores[i] = p->getGanador();

        string nom1 = E1->getname(); while ((int)nom1.size() < 20) nom1 += ' ';
        string nom2 = E2->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
        cout << "  | " << nom1 << " vs " << nom2
             << " =>  " << ganadores[i]->getname() << "\n";
        delete p;
    }
    cout << "+========================================================+\n";
    delete fechas;

    simularoctavos(ganadores);
}


void torneo::simularoctavos(Selecciones** ganadores16) {
    const int NUM_PARTIDOS = 8;
    Fecha* fecha = new Fecha(2026, 7, 10);
    Selecciones* ganadores8[8];

    cout << "\n+========================================================+\n";
    cout << "|                 OCTAVOS DE FINAL                       |\n";
    cout << "+========================================================+\n";

    for (int i = 0; i < NUM_PARTIDOS; i++) {
        Selecciones* E1 = ganadores16[i * 2];
        Selecciones* E2 = ganadores16[i * 2 + 1];
        Partido* partido = new Partido(E1, E2, fecha);
        partido->simular(true);
        ganadores8[i] = partido->getGanador();
        delete partido;

        string nom1 = E1->getname(); while ((int)nom1.size() < 20) nom1 += ' ';
        string nom2 = E2->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
        cout << "  | " << nom1 << " vs " << nom2
             << " =>  " << ganadores8[i]->getname() << "\n";
    }
    cout << "+========================================================+\n";

    delete fecha;
    simularCuartos(ganadores8);
}

void torneo::simularCuartos(Selecciones** ganadores8) {
    const int NUM_PARTIDOS = 4;
    Fecha* fecha = new Fecha(2026, 7, 18);
    Selecciones* ganadores4[4];

    cout << "\n+========================================================+\n";
    cout << "|                 CUARTOS DE FINAL                       |\n";
    cout << "+========================================================+\n";

    for (int i = 0; i < NUM_PARTIDOS; i++) {
        Selecciones* E1 = ganadores8[i * 2];
        Selecciones* E2 = ganadores8[i * 2 + 1];
        Partido* partido = new Partido(E1, E2, fecha);
        partido->simular(true);
        ganadores4[i] = partido->getGanador();
        delete partido;

        string nom1 = E1->getname(); while ((int)nom1.size() < 20) nom1 += ' ';
        string nom2 = E2->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
        cout << "  | " << nom1 << " vs " << nom2
             << " =>  " << ganadores4[i]->getname() << "\n";
    }
    cout << "+========================================================+\n";

    delete fecha;
    simularSemis(ganadores4);
}

void torneo::simularSemis(Selecciones** ganadores4) {
    const int NUM_PARTIDOS = 2;
    Fecha* fecha = new Fecha(2026, 7, 22);
    Selecciones* ganadores2[2];
    Selecciones* perdedores2[2];

    cout << "\n+========================================================+\n";
    cout << "|                   SEMIFINALES                          |\n";
    cout << "+========================================================+\n";

    for (int i = 0; i < NUM_PARTIDOS; i++) {
        Selecciones* E1 = ganadores4[i * 2];
        Selecciones* E2 = ganadores4[i * 2 + 1];
        Partido* partido = new Partido(E1, E2, fecha);
        partido->simular(true);
        ganadores2[i]  = partido->getGanador();
        perdedores2[i] = (ganadores2[i] == E1) ? E2 : E1;
        delete partido;

        string nom1 = E1->getname(); while ((int)nom1.size() < 20) nom1 += ' ';
        string nom2 = E2->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
        cout << "  | " << nom1 << " vs " << nom2
             << " =>  " << ganadores2[i]->getname() << "\n";
    }
    cout << "+========================================================+\n";

    delete fecha;
    simularTercerPuesto(perdedores2);
    simularFinal(ganadores2);
}

void torneo::simularTercerPuesto(Selecciones** perdedores2) {
    Fecha* fecha = new Fecha(2026, 7, 25);

    cout << "\n+========================================================+\n";
    cout << "|                TERCER Y CUARTO PUESTO                  |\n";
    cout << "+========================================================+\n";

    Partido* partido = new Partido(perdedores2[0], perdedores2[1], fecha);
    partido->simular(true);
    Selecciones* tercero = partido->getGanador();
    delete partido;

    string nom1 = perdedores2[0]->getname(); while ((int)nom1.size() < 20) nom1 += ' ';
    string nom2 = perdedores2[1]->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
    cout << "  | " << nom1 << " vs " << nom2 << "\n";
    cout << "  |  3er Puesto: " << tercero->getname() << "\n";
    cout << "+========================================================+\n";

    delete fecha;
}

void torneo::simularFinal(Selecciones** ganadores2) {
    Fecha* fecha = new Fecha(2026, 7, 26);

    cout << "\n+========================================================+\n";
    cout << "|              *** GRAN FINAL ***                        |\n";
    cout << "+========================================================+\n";

    Partido* partido = new Partido(ganadores2[0], ganadores2[1], fecha);
    partido->simular(true);
    Selecciones* campeon = partido->getGanador();
    delete partido;

    string nom1 = ganadores2[0]->getname(); while ((int)nom1.size() < 20) nom1 += ' ';
    string nom2 = ganadores2[1]->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
    cout << "  | " << nom1 << " vs " << nom2 << "\n";
    cout << "+========================================================+\n";
    cout << "|  CAMPEON DEL MUNDO 2026: ";
    string camp = campeon->getname();
    while ((int)camp.size() < 31) camp += ' ';
    cout << camp << "|\n";
    cout << "+========================================================+\n";

    delete fecha;
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

    // prepararSorteo() ya garantiza que selecciones[0] == USA (slot 0 del bombo 1)
    // y que selecciones[0..11] estan ordenados por ranking con USA primero.

    // Llenar los 4 bombos de 12 equipos cada uno
    Selecciones* bombo1[12], *bombo2[12], *bombo3[12], *bombo4[12];
    for (int i = 0; i < 12; i++) {
        bombo1[i] = selecciones[i];
        bombo2[i] = selecciones[i + 12];
        bombo3[i] = selecciones[i + 24];
        bombo4[i] = selecciones[i + 36];
    }

    // Mezclar SOLO los slots 1..11 del bombo1 (el slot 0 = USA queda fijo)
    // y todos los slots de los bombos 2, 3 y 4
    for (int i = 11; i > 1; i--) {          // bombo1: arranca en 1, protege slot 0
        int r = 1 + rand() % i;              // r en [1, i]
        swap(bombo1[i], bombo1[r]);
    }
    for (int i = 11; i > 0; i--) {
        int r;
        r = rand() % (i + 1); swap(bombo2[i], bombo2[r]);
        r = rand() % (i + 1); swap(bombo3[i], bombo3[r]);
        r = rand() % (i + 1); swap(bombo4[i], bombo4[r]);
    }

    // Inicializar los 12 grupos con letra
    for (int i = 0; i < 12; i++) {
        if (!grupo[i]) grupo[i] = new Grupos();
        grupo[i]->setLetra((char)('A' + i));
    }

    // Grupo A: USA (bombo1[0]) va fijo; completar con backtracking desde bombo 2
    {
        Selecciones** bombos[4] = { bombo1, bombo2, bombo3, bombo4 };
        Selecciones* integrantes[4] = { bombo1[0], nullptr, nullptr, nullptr };
        bombo1[0] = nullptr;           // marcar como usado
        if (!backtrackBombo(0, integrantes, bombos)) {
            cout << "ERROR: No se pudo armar el Grupo A con USA.\n";
        }
    }

    // Grupos B..L: backtracking completo desde bombo 1
    Selecciones** bombos[4] = { bombo1, bombo2, bombo3, bombo4 };
    for (int i = 1; i < 12; i++) {
        Selecciones* integrantes[4] = { nullptr, nullptr, nullptr, nullptr };
        if (!backtrackBombo(i, integrantes, bombos)) {
            cout << "ERROR: No se pudo armar el grupo " << (char)('A' + i)
                 << ". Revisa la distribucion de confederaciones.\n";
        }
    }
}
void torneo::construirTabla(){}

void torneo::prepararSorteo() {
    // 1. Ordenar los 48 equipos por ranking (bubble sort completo)
    for (int i = 0; i < 47; i++) {
        for (int j = 0; j < 47 - i; j++) {
            if (selecciones[j]->getRanking() > selecciones[j+1]->getRanking()) {
                Selecciones* tmp   = selecciones[j];
                selecciones[j]     = selecciones[j+1];
                selecciones[j+1]   = tmp;
            }
        }
    }
    // 2. Buscar United States en los 48 slots y moverlo al slot 0 del bombo 1.
    //    Se busca en todos (no solo en los primeros 12) porque su ranking puede
    //    ubicarlo en cualquier bombo antes de este ajuste.
    for (int i = 0; i < 48; i++) {
        if (selecciones[i]->getname() == "United States") {
            // Desplazar los que estaban antes de él una posicion hacia abajo
            Selecciones* tmp = selecciones[i];
            for (int k = i; k > 0; k--)
                selecciones[k] = selecciones[k-1];
            selecciones[0] = tmp;
            break;
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
// Si integrantes[0] ya viene asignado (caso USA en Grupo A), arranca desde bombo 1.
bool torneo::backtrackBombo(int grupoIdx,
                             Selecciones* integrantes[4],
                             Selecciones** bombos[4]) {
    // Determinar desde que bombo empezar segun los slots ya rellenos
    int startBombo = 0;
    for (int b = 0; b < 4; b++) {
        if (integrantes[b] != nullptr) startBombo = b + 1;
        else break;
    }

    // Funcion interna recursiva (simulada con funcion lambda-like via helper)
    // Como C++03/11 sin lambdas capturadas, usamos una funcion recursiva local
    // implementada directamente aqui con un loop de profundidad.
    // Usamos una pila manual de profundidad 4.
    struct Frame { int bomboIdx; int j; };
    Frame pila[4];
    int prof = startBombo;

    // Inicializar primera llamada
    pila[prof] = {prof, 0};

    while (prof >= startBombo) {
        int b = prof;             // bombo actual
        int& j = pila[prof].j;   // indice dentro del bombo (persistente entre iteraciones)

        if (b == 4) {
            // Todos los bombos asignados → crear el grupo
            grupo[grupoIdx] = new Grupos((char)('A' + grupoIdx),
                                         integrantes[0], integrantes[1],
                                         integrantes[2], integrantes[3]);
            return true;
        }

        bool avanzar = false;
        for (; j < 12; j++) {
            if (bombos[b][j] == nullptr) continue;
            if (esValido(bombos[b][j], integrantes, b)) {
                integrantes[b] = bombos[b][j];
                bombos[b][j]   = nullptr;
                j++;              // proxima iteracion de este nivel empieza en j+1
                prof++;           // bajar un nivel
                pila[prof] = {prof, 0};
                avanzar = true;
                break;
            }
        }

        if (!avanzar) {
            // Backtrack: subir un nivel
            integrantes[b] = nullptr;
            prof--;
            if (prof >= startBombo) {
                // Restaurar el equipo que habiamos elegido en el nivel anterior
                int prevB = prof;
                int prevJ = pila[prof].j - 1;   // j ya fue incrementado al elegir
                bombos[prevB][prevJ] = integrantes[prevB];
                integrantes[prevB]   = nullptr;
                // pila[prof].j ya apunta al siguiente candidato
            }
        }
    }

    return false; // ninguna combinacion funciono
}







void torneo::imprimirGrupos() const {
    cout << "\n+=======================================================+\n";
    cout << "|       SORTEO MUNDIAL 2026 - GRUPOS                    |\n";
    cout << "+=======================================================+\n";

    for (int i = 0; i < 12; i++) {
        cout << "\n  +---[ GRUPO " << (char)('A' + i) << " ]----------------------------+\n";
        cout << "  | #  Pais                     Confederacion    |\n";
        cout << "  +-------------------------------------------------+\n";
        for (int j = 0; j < 4; j++) {
            Selecciones* eq = grupo[i]->getEquipo(j);
            if (!eq) continue;
            string nombre = eq->getname();
            string conf   = eq->getConfederacion();
            // Padding fijo en ASCII puro
            while ((int)nombre.size() < 24) nombre += ' ';
            while ((int)conf.size()   < 14) conf   += ' ';
            cout << "  | " << (j+1) << "  " << nombre << " " << conf << " |\n";
        }
        cout << "  +-------------------------------------------------+\n";
    }
    cout << "\n";
}