#include "torneo.h"

torneo::torneo() {
    iteraciones = 0;
    Memoriausada = 0;
    gestorarchivos=nullptr;
    for(int i = 0; i < 48; i++) selecciones[i] = nullptr;
    for(int i = 0; i < 12; i++) grupo[i] = nullptr;
    Medidor::registrarCreacion("Torneo",this);
}


//          metodos
void torneo::cargarDatos(string rutacsv,string datos_jugadores){
    gestorarchivos=new GestorArchivos(rutacsv,datos_jugadores);
    gestorarchivos->leerSelecciones(selecciones,48);
    sortearGrupos();   // sorteo real con restricciones de confederacion y USA en Grupo A
    pausar("Sorteo Completado.");
    // testgrupos();   // solo para debug rapido sin sorteo
}

void torneo::guardarDatos(){
    gestorarchivos->guardarJugadores(selecciones,48);
    gestorarchivos->actualizarSelecciones(selecciones,48);
}

void torneo::simularTorneo(){
    //funcion que simula todo el torneo

    simularFaseGrupos();
    pausar("Fase de grupos completada.");
    simularDieciseisavos();
    guardarDatos();
    pausar("Pasar a Medidas de Eficiencia");


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
            cout << "  |-------------------- Stats jugadores destacados ";
            cout << "---------------------+\n";

            // // Mostrar los 11 titulares de cada equipo con sus stats
            // // usando un formato de tabla ASCII compacto
            // cout << "  |    | # Jugador                Goles  Min  Amar  Falt |\n";
            // cout << "  |    +--------------------------------------------------+\n";
            //     // Como showstats() imprime en su propio formato, lo llamamos
            //     // y avisamos al usuario:
            // // Llamada al metodo existente para stats completos
            // cout << "  |    +--------------------------------------------------+\n";
            // p->showstats();
            cout << "  |----------------------------------------------------------+\n";
            p->showpartido();
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
    // Selecciones* segundos[12];
    EntradaTabla segundos[12];
    EntradaTabla terceros[12];

    for (int i = 0; i < 12; i++) {
        primeros[i] = grupo[i]->getPrimero();
        segundos[i] = grupo[i]->getEntradaSegundo();
        terceros[i] = grupo[i]->getEntradaTercero();

    }
    //como los peores 4 segundos se enfentan a los restantes 1eros
    //intercambiamos los mejores 12 segundos
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11 - i; j++) {
            bool intercambiar =
                segundos[j].puntos < segundos[j+1].puntos ||
                (segundos[j].puntos == segundos[j+1].puntos &&
                 segundos[j].dg     < segundos[j+1].dg);
            if (intercambiar) {
                EntradaTabla tmp = segundos[j];
                segundos[j]      = segundos[j+1];
                segundos[j+1]    = tmp;
            }
        }
    }

    //Intercambiamos a los mejores 8 terceros

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


    cout << "\n+========================================================+\n";
    cout << "|           8 MEJORES TERCEROS CLASIFICADOS              |\n";
    cout << "+========================================================+\n";
    for (int i = 0; i < 8; i++) {
        string nom = terceros[i].equipo->getname();
        while ((int)nom.size() < 24) nom += ' ';
        cout << "  | " << (i+1) << ". " << nom
             << "  Pts: " << terceros[i].puntos
             << "  DG: "  << terceros[i].dg << "\n";
    }
    cout << "+========================================================+\n";



    // 5. Armar 16 cruces con backtracking  Condicion faltante
    // Regla 1: primeros[0..7]  vs terceros[0..7]  (8 partidos)
    // Regla 2: primeros[8..11] vs peores segundos[8..11] (4 partidos)
    // Regla 3: mejores segundos[0..7] entre si (4 partidos)
    // Regla 4: No pueden cruzarse equipos del mismo grupo
    Selecciones* E1s[16];
    Selecciones* E2s[16];
    bool usadoPrimero[12] = {false};
    bool usadoSegundo[12] = {false};
    bool usadoTercero[8]  = {false};

    // Regla 1: primeros vs terceros
    int partidoIdx = 0;
    for (int i = 0; i < 12 && partidoIdx < 8; i++) {
        if (usadoPrimero[i]) continue;
        for (int j = 0; j < 8; j++) {
            if (usadoTercero[j]) continue;
            if (cruceValido(primeros[i],terceros[j].equipo)) {
                E1s[partidoIdx] = primeros[i];
                E2s[partidoIdx] = terceros[j].equipo;
                usadoPrimero[i] = true;
                usadoTercero[j] = true;
                partidoIdx++;
                break;
            }
        }
        // Pasada 2: fallback sin restriccion
        if (!usadoPrimero[i]) {
            for (int j = 0; j < 8; j++) {
                if (usadoTercero[j]) continue;
                E1s[partidoIdx] = primeros[i];
                E2s[partidoIdx] = terceros[j].equipo;
                usadoPrimero[i] = true;
                usadoTercero[j] = true;
                partidoIdx++;
                cout << "  [!] Fallback: " << primeros[i]->getname()
                     << " vs equipo del mismo grupo (sin alternativa)\n";
                break;
            }
        }
    }

    // Regla 2: primeros restantes vs peores 4 segundos (indices 8..11)
    for (int i = 0; i < 12 && partidoIdx < 12; i++) {
        if (usadoPrimero[i]) continue;
        for (int j = 8; j < 12; j++) {
            if (usadoSegundo[j]) continue;
            if (cruceValido(primeros[i],segundos[j].equipo)) {
                E1s[partidoIdx] = primeros[i];
                E2s[partidoIdx] = segundos[j].equipo;
                usadoPrimero[i] = true;
                usadoSegundo[j] = true;
                partidoIdx++;
                break;
            }
        }
        // Pasada 2: fallback
        if (!usadoPrimero[i]) {
            for (int j = 8; j < 12; j++) {
                if (usadoSegundo[j]) continue;
                E1s[partidoIdx] = primeros[i];
                E2s[partidoIdx] = segundos[j].equipo;
                usadoPrimero[i] = true;
                usadoSegundo[j] = true;
                partidoIdx++;
                cout << "  [!] Fallback: " << primeros[i]->getname()
                     << " vs equipo del mismo grupo (sin alternativa)\n";
                break;
            }
        }
    }

    // Regla 3: mejores 8 segundos entre si (indices 0..7)
    for (int i = 0; i < 8 && partidoIdx < 16; i++) {
        if (usadoSegundo[i]) continue;
        for (int j = i + 1; j < 8; j++) {
            if (usadoSegundo[j]) continue;
            if (cruceValido(segundos[i].equipo,segundos[j].equipo)) {
                E1s[partidoIdx] = segundos[i].equipo;
                E2s[partidoIdx] = segundos[j].equipo;
                usadoSegundo[i] = true;
                usadoSegundo[j] = true;
                partidoIdx++;
                break;
            }
        }
        // Pasada 2: fallback
        if (!usadoSegundo[i]) {
            for (int j = i + 1; j < 8; j++) {
                if (usadoSegundo[j]) continue;
                E1s[partidoIdx] = segundos[i].equipo;
                E2s[partidoIdx] = segundos[j].equipo;
                usadoSegundo[i] = true;
                usadoSegundo[j] = true;
                partidoIdx++;
                cout << "  [!] Fallback: " << segundos[i].equipo->getname()
                     << " vs equipo del mismo grupo (sin alternativa)\n";
                break;
            }
        }
    }
    //auxiliar para mostrar confederacion con mas representantes
    Selecciones* clasificados16[32];
    for (int i = 0; i < 16; i++) {
        clasificados16[i]      = E1s[i];
        clasificados16[i + 16] = E2s[i];
    }



    cout << "\n+========================================================+\n";
    cout << "|            DIECISEISAVOS DE FINAL                      |\n";
    cout << "+========================================================+\n";
    ConfMasEquipos(clasificados16, 32, "16avos");

    Selecciones* ganadores[16];
    Fecha* fechas = new Fecha(2026, 7, 1);


    for (int i = 0; i < 16; i++) {
        Selecciones* E1 = E1s[i];
        Selecciones* E2 = E2s[i];

        Partido* p = new Partido(E1, E2, fechas);
        p->simular(true);
        ganadores[i] = p->getGanador();
        int E1gol=p->getGol(1);
        int E2gol=p->getGol(2);

        string nom1 = E1->getname(); while ((int)nom1.size() < 10) nom1 += ' ';
        string nom2 = E2->getname(); while ((int)nom2.size() < 20) nom2 += ' ';
        cout<< nom1 << "  " << E1gol << " - " << E2gol
             << "  " << nom2<<endl;

        // Estadisticas del partido: cabecera
        cout << "  |----------- Stats del Partido ";
        cout << "----------------------------+\n";

        // // Mostrar los 11 titulares de cada equipo con sus stats
        // // usando un formato de tabla ASCII compacto
        // cout << "  |    | # Jugador                Goles  Min  Amar  Falt |\n";
        // cout << "  |    +--------------------------------------------------+\n";

        // Llamada al metodo existente para stats completos
        cout << "  |-----------------------------------------------------+\n";
        p->showpartido();
        cout << "  |\n";
        delete p;
    }
    cout << "+========================================================+\n";
    delete fechas;

    pausar("Diesisavos De Final Completado.");
    simularoctavos(ganadores);
}


void torneo::simularoctavos(Selecciones** ganadores16) {
    const int NUM_PARTIDOS = 8;
    Fecha* fecha = new Fecha(2026, 7, 10);
    Selecciones* ganadores8[8];


    cout << "\n+========================================================+\n";
    cout << "|                 OCTAVOS DE FINAL                       |\n";
    cout << "+========================================================+\n";
    ConfMasEquipos(ganadores16,16, "8avos");

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
         cout << "  |----------------Stats del Partido------------------------- \n";
        // Llamada al metodo existente para stats completos
        cout << "  |--------------------------------------------------------+\n";
        partido->showpartido();
        cout << "  |\n";
    }
    cout << "+========================================================+\n";

    delete fecha;
    pausar("Octavos De Final Completado");
    simularCuartos(ganadores8);
}

void torneo::simularCuartos(Selecciones** ganadores8) {

    const int NUM_PARTIDOS = 4;
    Fecha* fecha = new Fecha(2026, 7, 18);
    Selecciones* ganadores4[4];

    cout << "\n+========================================================+\n";
    cout << "|                 CUARTOS DE FINAL                       |\n";
    cout << "+========================================================+\n";
    ConfMasEquipos(ganadores8, 8, "4tos");

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
        cout << "  |----------------Stats del Partido------------------------- \n";
        // Llamada al metodo existente para stats completos
        cout << "  |--------------------------------------------------------+\n";
        partido->showpartido();
        cout << "  |\n";

    }
    cout << "+========================================================+\n";

    delete fecha;
    pausar("Cuartos de Final Completados.");
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

        cout << "  |----------------Stats del Partido------------------------- \n";
        // Llamada al metodo existente para stats completos
        cout << "  |--------------------------------------------------------+\n";
        partido->showpartido();
        cout << "  |\n";

    }
    cout << "+========================================================+\n";

    delete fecha;
    pausar("Semifinales Completadas");
    simularTercerPuesto(perdedores2);
    pausar("Tercer Puesto Completada.");
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
    cout << "  | " << nom1 << " vs " << nom2
         << " =>  " << tercero->getname() << "\n";
    cout << "  |----------------Stats del Partido------------------------- \n";
    // Llamada al metodo existente para stats completos
    cout << "  |--------------------------------------------------------+\n";
    partido->showpartido();
    cout << "  |\n";
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
    cout << "  | " << nom1 << " vs " << nom2
         << " =>  " << campeon->getname() << "\n";

    cout << "  |----------------Stats del Partido------------------------- \n";
    // Llamada al metodo existente para stats completos
    cout << "  |--------------------------------------------------------+\n";
    partido->showpartido();
    cout << "  |\n";
    cout << "+========================================================+\n";
    cout << "|  CAMPEON DEL MUNDO 2026: ";
    string camp = campeon->getname();
    while ((int)camp.size() < 31) camp += ' ';
    cout << camp << "|\n";
    cout << "+========================================================+\n";
    pausar("Pasar a Datos del Torneo");
    GoleadorCampeon(campeon);
    MaximoGoleadores();

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



// ── Mezcla Fisher-Yates un arreglo de punteros (desde 'desde' inclusive) ───
static void mezclar(Selecciones** arr, int desde, int hasta) {
    for (int i = hasta; i > desde; i--) {
        int r = desde + rand() % (i - desde + 1);
        Selecciones* t = arr[i]; arr[i] = arr[r]; arr[r] = t;
    }
}

// ── Asigna un bombo completo a los 12 grupos respetando restricciones.
//    grupos_conf[g] = confederaciones ya asignadas al grupo g (hasta 4 chars).
//    Retorna true si pudo asignar los 12 equipos del bombo a los 12 grupos.
//    Usa reintentos con re-mezcla para aleatorizar sin recursion profunda.
static bool asignarBombo(Selecciones** bombo,   // 12 equipos (algunos nullptr)
                         Selecciones** destino, // destino[g] = puntero a llenar
                         Selecciones* grupo_actual[12][4], // estado actual
                         int columna)           // que columna (bombo) estamos llenando
{
    // Recoger equipos disponibles del bombo
    Selecciones* disponibles[12];
    int nDisp = 0;
    for (int i = 0; i < 12; i++)
        if (bombo[i]) disponibles[nDisp++] = bombo[i];

    // Intentar hasta 200 permutaciones aleatorias
    for (int intento = 0; intento < 200; intento++) {
        // Mezclar disponibles
        for (int i = nDisp - 1; i > 0; i--) {
            int r = rand() % (i + 1);
            Selecciones* t = disponibles[i]; disponibles[i] = disponibles[r]; disponibles[r] = t;
        }

        // Intentar asignar en orden de grupos
        bool ok = true;
        int asignados[12]; // disponibles[asignados[g]] va al grupo g
        int usado[12] = {};

        for (int g = 0; g < 12; g++) {
            bool encontrado = false;
            for (int k = 0; k < nDisp; k++) {
                if (usado[k]) continue;
                // Verificar restriccion: misma conf no-UEFA ya en el grupo,
                // o mas de 2 UEFA
                string confCand = disponibles[k]->getConfederacion();
                // Normalizar a mayusculas
                for (int ci = 0; ci < (int)confCand.size(); ci++)
                    if (confCand[ci] >= 'a' && confCand[ci] <= 'z')
                        confCand[ci] = confCand[ci] - 'a' + 'A';

                int uefaCount = (confCand == "UEFA") ? 1 : 0;
                bool conflicto = false;
                for (int b = 0; b < 4; b++) {
                    if (!grupo_actual[g][b]) continue;
                    string ca = grupo_actual[g][b]->getConfederacion();
                    for (int ci = 0; ci < (int)ca.size(); ci++)
                        if (ca[ci] >= 'a' && ca[ci] <= 'z') ca[ci] = ca[ci]-'a'+'A';
                    if (ca == confCand && confCand != "UEFA") { conflicto = true; break; }
                    if (ca == "UEFA") uefaCount++;
                }
                if (conflicto || uefaCount > 2) continue;

                asignados[g] = k;
                usado[k] = 1;
                encontrado = true;
                break;
            }
            if (!encontrado) { ok = false; break; }
        }

        if (ok) {
            // Aplicar asignacion
            for (int g = 0; g < 12; g++)
                grupo_actual[g][columna] = disponibles[asignados[g]];
            return true;
        }
    }
    return false;
}

void torneo::sortearGrupos() {
    prepararSorteo();

    // Copiar los 4 bombos
    Selecciones* bombo[4][12];
    for (int b = 0; b < 4; b++)
        for (int i = 0; i < 12; i++)
            bombo[b][i] = selecciones[b * 12 + i];

    // Mezclar bombo 1 sin tocar slot 0 (USA)
    mezclar(bombo[0], 1, 11);
    // Mezclar bombos 2, 3, 4 completo
    mezclar(bombo[1], 0, 11);
    mezclar(bombo[2], 0, 11);
    mezclar(bombo[3], 0, 11);

    // Matriz resultado
    Selecciones* gmat[12][4];
    for (int g = 0; g < 12; g++)
        for (int b = 0; b < 4; b++)
            gmat[g][b] = nullptr;

    // Bombo 0: USA fijo en grupo A, resto asignar
    gmat[0][0] = bombo[0][0];   // USA -> Grupo A
    bombo[0][0] = nullptr;      // marcarlo como usado

    // Asignar bombo 0 restante (grupos B-L)
    {
        Selecciones* resto[11]; int nr = 0;
        for (int i = 1; i < 12; i++) if (bombo[0][i]) resto[nr++] = bombo[0][i];
        // Mezclar resto
        for (int i = nr-1; i > 0; i--) {
            int r = rand()%(i+1); Selecciones*t=resto[i]; resto[i]=resto[r]; resto[r]=t;
        }
        // Asignar directamente (bombo 0 = cabezas de serie, una por grupo, sin restriccion conf entre si)
        // Solo verificamos que no haya misma confederacion que USA en grupo A
        // (USA es Concacaf, los demas son de distintas confs en bombo1)
        for (int g = 1; g < 12; g++) gmat[g][0] = resto[g-1];
    }

    // Asignar bombos 1, 2, 3
    for (int b = 1; b < 4; b++) {
        bool ok = asignarBombo(bombo[b], nullptr, gmat, b);
        if (!ok) {
            // Reintento con re-mezcla total
            mezclar(bombo[b], 0, 11);
            ok = asignarBombo(bombo[b], nullptr, gmat, b);
        }
        if (!ok) {
            cout << "ADVERTENCIA: bombo " << (b+1)
                 << " asignado sin restriccion (no hay solucion perfecta).\n";
            // Asignacion forzada ignorando restriccion
            int k = 0;
            for (int g = 0; g < 12; g++) {
                while (k < 12 && !bombo[b][k]) k++;
                if (k < 12) { gmat[g][b] = bombo[b][k++]; }
            }
        }
    }

    // Crear grupos
    for (int g = 0; g < 12; g++) {
        grupo[g] = new Grupos((char)('A' + g),
                              gmat[g][0], gmat[g][1],
                              gmat[g][2], gmat[g][3]);
    }
    cout << "Sorteo completado. United States -> Grupo A.\n";
}
void torneo::construirTabla(){}

void torneo::prepararSorteo() {
    // 1. Ordenar los 48 equipos por ranking ascendente (bubble sort)
    for (int i = 0; i < 47; i++) {
        for (int j = 0; j < 47 - i; j++) {
            if (selecciones[j]->getRanking() > selecciones[j+1]->getRanking()) {
                Selecciones* tmp = selecciones[j];
                selecciones[j]   = selecciones[j+1];
                selecciones[j+1] = tmp;
            }
        }
    }
    // Tras el sort: selecciones[0..11] = top-12 por ranking (bombo 1 natural)
    //              selecciones[12..23]= siguientes 12 (bombo 2 natural), etc.

    // 2. Buscar United States en cualquier posicion
    int idxUSA = -1;
    for (int i = 0; i < 48; i++) {
        if (selecciones[i]->getname() == "United States") {
            idxUSA = i;
            break;
        }
    }
    if (idxUSA == -1) {
        cout << "ADVERTENCIA: United States no encontrado en el CSV.\n";
        return;
    }

    // 3. Si USA NO esta ya en el bombo 1 (slots 0..11), aplicar regla FIFA:
    //    USA entra al slot 0 del bombo 1 (cabeza de serie anfitrion).
    //    El equipo que ocupaba el slot 11 del bombo 1 (el de menor ranking
    //    entre los cabezas de serie) baja al bombo 2, desplazando en cascada.
    if (idxUSA >= 12) {
        // Guardar al "expulsado" del bombo 1 (slot 11, el peor cabeza de serie)
        Selecciones* expulsado = selecciones[11];

        // Insertar USA en slot 0, desplazando slots 0..10 una posicion adelante
        Selecciones* usa = selecciones[idxUSA];
        // Primero liberar el slot de USA en su bombo original
        // desplazando hacia atras ese hueco
        for (int k = idxUSA; k > 0; k--)
            selecciones[k] = selecciones[k-1];
        selecciones[0] = usa;

        // Ahora selecciones[12] era el primer elemento del bombo2 original,
        // pero el corrimiento ya lo movio; el "expulsado" (antes en slot 11)
        // ahora esta en slot 12 tras el corrimiento. Solo necesitamos
        // verificar que quede en el bombo 2 (slots 12..23), lo cual ya ocurre
        // por el corrimiento en cascada.
        (void)expulsado; // ya fue desplazado naturalmente por el corrimiento
    } else {
        // USA ya esta en el bombo 1: solo moverlo al slot 0
        Selecciones* usa = selecciones[idxUSA];
        for (int k = idxUSA; k > 0; k--)
            selecciones[k] = selecciones[k-1];
        selecciones[0] = usa;
    }

    // // Verificacion
    // cout << "Anfitrion confirmado en slot 0 (Grupo A): "
    //      << selecciones[0]->getname()
    //      << " (ranking " << selecciones[0]->getRanking() << ")\n";
    // cout << "Bombo 1 (cabezas de serie):\n";
    // for (int i = 0; i < 12; i++)
    //     cout << "  [" << i << "] " << selecciones[i]->getname()
    //          << " (" << selecciones[i]->getConfederacion() << ")\n";
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

void torneo::tamanioTorneo(){

    cout << "\n+========================================================+\n";
    cout << "|              MEMORIA TOTAL ESTIMADA                    |\n";
    cout << "+========================================================+\n";

    // Cantidades conocidas del torneo
    const int N_SELECCIONES  = 48;
    const int N_JUGADORES    = 48 * 26;   // 26 por seleccion
    const int N_GRUPOS       = 12;
    const int N_PARTIDOS_GRP = 72;        // fase de grupos
    const int N_PARTIDOS_ELI = 32;        // 16avos+8vos+cuartos+semis+final+3erpuesto
    const int N_FECHAS       = 1;         // una sola instancia reutilizada

    size_t memSelecciones = sizeof(Selecciones) * N_SELECCIONES;
    size_t memJugadores   = sizeof(jugadores)   * N_JUGADORES;
    size_t memGrupos      = sizeof(Grupos)      * N_GRUPOS;
    size_t memPartidosGrp = sizeof(Partido)     * N_PARTIDOS_GRP;
    size_t memPartidosEli = sizeof(Partido)     * N_PARTIDOS_ELI;
    size_t memFecha       = sizeof(Fecha)       * N_FECHAS;

    size_t memTotal = memSelecciones + memJugadores + memGrupos
                      + memPartidosGrp + memPartidosEli + memFecha;

    cout << "  Selecciones  (" << N_SELECCIONES  << " obj): " << memSelecciones << " bytes\n";
    cout << "  Jugadores    (" << N_JUGADORES    << " obj): " << memJugadores   << " bytes\n";
    cout << "  Grupos       (" << N_GRUPOS       << " obj): " << memGrupos      << " bytes\n";
    cout << "  Partidos GRP (" << N_PARTIDOS_GRP << " obj): " << memPartidosGrp << " bytes\n";
    cout << "  Partidos ELI (" << N_PARTIDOS_ELI << " obj): " << memPartidosEli << " bytes\n";
    cout << "  Fecha        (" << N_FECHAS       << " obj): " << memFecha       << " bytes\n";
    cout << "  --------------------------------------------------\n";
    cout << "  Total estimado : " << memTotal       << " bytes\n";
    cout << "                   " << memTotal/1024  << " KB\n";
    cout << "+========================================================+\n";


    //Una captura de pantalla pesa en promedio entre 500Kb y 1Kb----eficiente
    //adicionalmente los partidos que se hacen , nunca se hacen simultaneamente , por lo que en realidad se consume menos memoria
    //ya que antes de simular algun partido, se libera el partido anterior, esto queda mas claro en la funcion  ImprimirReporte
    //las clases que siempre estan en la memoria son selecciones, statsteam, fecha,jugadores,grupos, porque estas ñlas necesitaremos para al final guardar datos
}

void torneo::GoleadorCampeon(Selecciones* campeon) const {
    jugadores* goleador = campeon->getGoleador();

    cout << "\n+========================================================+\n";
    cout << "|         MAXIMO GOLEADOR DE LA SELECCION CAMPEON        |\n";
    cout << "+========================================================+\n";
    cout << "  Seleccion : " << campeon->getname()    << "\n";
    cout << "  Jugador   : " << goleador->getname()   << "\n";
    cout << "  Goles     : " << goleador->getGoals()  << "\n";
    cout << "+========================================================+\n";
}


void torneo::MaximoGoleadores()const{
    jugadores* top3[3]={nullptr,nullptr,nullptr};
    Selecciones* selectop3[3]={nullptr,nullptr,nullptr};
    //Iteramos sobre cada seleccion y luego sobre cada jugador para hallar maximo goleador
    for (int s = 0; s < 48; s++) {
        for (int j = 0; j < 26; j++) {
            jugadores* jug = selecciones[s]->getPlayer(j);
            if (jug == nullptr) continue;

            unsigned int goles = jug->getGoals(); // ajusta al getter real

            // Ver si entra en el top 3
            for (int t = 0; t < 3; t++) {
                if (top3[t] == nullptr || goles > top3[t]->getGoals()) {

                    // Desplazar hacia abajo
                    for (int k = 2; k > t; k--){
                        selectop3[k]=selecciones[k-1];
                        top3[k] = top3[k - 1];
                    }
                    top3[t] = jug;
                    selectop3[t]=selecciones[s];
                    break;
                }
            }
        }
    }
    cout << "\n+========================================================+\n";
    cout << "|              MAXIMOS GOLEADORES DEL TORNEO             |\n";
    cout << "+========================================================+\n";
    for (int i = 0; i < 3; i++) {
        if (top3[i] == nullptr) continue;

        cout << "  " << (i+1) << ". " << top3[i]->getname()
             << " - " << top3[i]->getGoals() << " goles  ";
        cout<<"Seleccion: "<<selectop3[i]->getname()<<endl;
    }
    cout << "+========================================================+\n";
}

void torneo::ConfMasEquipos(Selecciones** equipos, int cantidad, const string& fase) const {
    string confConteo[10];
    int    conteos[10] = {0};
    int    numConfs    = 0;

    for (int i = 0; i < cantidad; i++) {
        string conf = equipos[i]->getConfederacion();
        bool encontrado = false;
        for (int j = 0; j < numConfs; j++) {
            if (confConteo[j] == conf) {
                conteos[j]++;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            confConteo[numConfs] = conf;
            conteos[numConfs]    = 1;
            numConfs++;
        }
    }

    int maxIdx = 0;
    for (int i = 1; i < numConfs; i++)
        if (conteos[i] > conteos[maxIdx]) maxIdx = i;

    cout << "\n  Confederacion con mas equipos en " << fase << ": "
         << confConteo[maxIdx] << " (" << conteos[maxIdx] << " equipos)\n";
    cout<<"\n";

    // Imprimir todas por si quieres verlas todas
    // for (int i = 0; i < numConfs; i++)
    //     cout << "    " << confConteo[i] << ": " << conteos[i] << " equipos\n";
}




void torneo::limpiarConsola() const {
#ifdef _WIN32
    system("cls");   // Windows
#else
    system("clear"); // Linux/Mac
#endif
}

char torneo::getGrupoDeEquipo(Selecciones* e) {
    for (int i = 0; i < 12; i++)
        for (int j = 0; j < 4; j++)
            if (grupo[i]->getEquipo(j) == e)
                return 'A' + i;
    return '?';
}
bool torneo::cruceValido(Selecciones* e1, Selecciones* e2) {
    return getGrupoDeEquipo(e1) != getGrupoDeEquipo(e2);
}

void torneo::pausar(const string& mensaje) const {
    cout << "\n  " << mensaje << " [Enter para continuar...]\n";
    cin.ignore();
    cin.get();
    limpiarConsola();
}

torneo::~torneo(){Medidor::registrarDestruccion("Torneo",this);}