#include "gestorarchivos.h"

GestorArchivos::GestorArchivos(const string& csv, const string& jugadores) {
    rutaCSV       = csv;
    rutaJugadores = jugadores;
}

// ─────────────────────────────────────────────────────────────
// Helpers privados
// ─────────────────────────────────────────────────────────────
string GestorArchivos::trim(const string& s) const {
    int inicio = 0;
    int fin    = (int)s.size() - 1;
    // Elimina BOM UTF-8 (0xEF 0xBB 0xBF) que puede venir del CSV
    while (inicio <= fin &&
           ((unsigned char)s[inicio] == 0xEF ||
            (unsigned char)s[inicio] == 0xBB ||
            (unsigned char)s[inicio] == 0xBF ||
            s[inicio] == ' ' || s[inicio] == '\t' ||
            s[inicio] == '\r'|| s[inicio] == '\n')) inicio++;
    while (fin >= inicio &&
           (s[fin] == ' ' || s[fin] == '\t' ||
            s[fin] == '\r'|| s[fin] == '\n')) fin--;
    if (inicio > fin) return "";
    return s.substr(inicio, fin - inicio + 1);
}

int GestorArchivos::parsearLinea(const string& linea, char delim,
                                  string* campos, int maxCampos) const {
    int count = 0;
    int start = 0;
    int len   = (int)linea.size();
    for (int i = 0; i <= len && count < maxCampos; i++) {
        if (i == len || linea[i] == delim) {
            campos[count++] = trim(linea.substr(start, i - start));
            start = i + 1;
        }
    }
    return count;
}

void GestorArchivos::actualizarSelecciones(Selecciones** equipos, int numEquipos) const {
    ofstream archivo(rutaCSV.c_str());
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir " << rutaCSV << endl;
        return;
    }

    // Mismas dos lineas de encabezado que se saltan al leer
    archivo << "Estadisticas Selecciones Mundial 2026\n";
    archivo << "Ranking fifa;Pais;Tecnico;Federacion;Confederacion;"
            << "Goles Favor;Goles Contra;Partidos Ganados;"
            << "Partidos Empatados;Partidos Perdidos\n";

    for (int i = 0; i < numEquipos; i++) {
        archivo << equipos[i]->getRanking()           << ";"
                << equipos[i]->getname()              << ";"
                << equipos[i]->getmanager()           << ";"
                << equipos[i]->getfederacion()        << ";"
                << equipos[i]->getConfederacion()     << ";"
                << equipos[i]->favorGoals()        << ";"
                << equipos[i]->counterGoals()       << ";"
                << equipos[i]->getWin()   << ";"
                << equipos[i]->getDraws() << ";"
                << equipos[i]->getLose()  << "\n";
    }

    archivo.close();
}

// ─────────────────────────────────────────────────────────────
// Lee el CSV de selecciones
// Formato esperado (desde línea 3, delimitador ';'):
//   ranking;pais;tecnico;federacion;confederacion;gf;gc;pg;pe;pp
// ─────────────────────────────────────────────────────────────
int GestorArchivos::leerSelecciones(Selecciones** equipos, int maxEquipos) const {
    ifstream archivo(rutaCSV.c_str());
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir " << rutaCSV << endl;
        return 0;
    }

    string linea;
    int    count = 0;

    // Saltar las dos primeras líneas (título y encabezado)
    getline(archivo, linea);
    getline(archivo, linea);

    while (getline(archivo, linea) && count < maxEquipos) {
        linea = trim(linea);
        if (linea.empty()) continue;

        const int MAX_CAMPOS = 10;
        string campos[MAX_CAMPOS];
        int    n = parsearLinea(linea, ';', campos, MAX_CAMPOS);
        if (n < 10) continue;   // línea incompleta

        int          ranking       = 0;
        string       pais          = campos[1];
        string       tecnico       = campos[2];
        string       federacion    = campos[3];
        string       confederacion = campos[4];
        unsigned int gf            = 0;
        unsigned int gc            = 0;
        unsigned int pg            = 0;
        unsigned int pe            = 0;
        unsigned int pp            = 0;

        // Conversión manual de string a entero (sin stoi para máxima compatibilidad)
        for (int i = 0; i < (int)campos[0].size(); i++)
            if (campos[0][i] >= '0' && campos[0][i] <= '9')
                ranking = ranking * 10 + (campos[0][i] - '0');
        for (int i = 0; i < (int)campos[5].size(); i++)
            if (campos[5][i] >= '0' && campos[5][i] <= '9')
                gf = gf * 10 + (campos[5][i] - '0');
        for (int i = 0; i < (int)campos[6].size(); i++)
            if (campos[6][i] >= '0' && campos[6][i] <= '9')
                gc = gc * 10 + (campos[6][i] - '0');
        for (int i = 0; i < (int)campos[7].size(); i++)
            if (campos[7][i] >= '0' && campos[7][i] <= '9')
                pg = pg * 10 + (campos[7][i] - '0');
        for (int i = 0; i < (int)campos[8].size(); i++)
            if (campos[8][i] >= '0' && campos[8][i] <= '9')
                pe = pe * 10 + (campos[8][i] - '0');
        for (int i = 0; i < (int)campos[9].size(); i++)
            if (campos[9][i] >= '0' && campos[9][i] <= '9')
                pp = pp * 10 + (campos[9][i] - '0');

        equipos[count] = new Selecciones(pais, tecnico, ranking,federacion,
                                          confederacion, gf, gc, pg, pe, pp);
        count++;
    }
    archivo.close();
    return count;
}

// ─────────────────────────────────────────────────────────────
// Guarda las stats de todos los jugadores de todos los equipos
// Formato CSV: pais;camiseta;goles;minutos;amarillas;rojas;faltas
// ─────────────────────────────────────────────────────────────
void GestorArchivos::guardarJugadores(Selecciones** equipos, int numEquipos) const {
    // Asegurar extensión .csv
    string ruta = rutaJugadores;
    if (ruta.size() < 4 || ruta.substr(ruta.size()-4) != ".csv")
        ruta += ".csv";

    ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "Error: no se pudo escribir " << ruta << endl;
        return;
    }

    // Encabezado CSV
    archivo << "Pais;Camiseta;Goles;Minutos;Tarjetas Amarillas;Tarjetas Rojas;Faltas\n";

    for (int e = 0; e < numEquipos; e++) {
        if (!equipos[e]) continue;
        for (int j = 0; j < 26; j++) {
            jugadores* jug = equipos[e]->getPlayer(j);
            if (!jug) continue;
            archivo << equipos[e]->getname()   << ";"
                    << jug->getShirt()          << ";"
                    << jug->getGoals()          << ";"
                    << jug->getMinutes()        << ";"
                    << jug->getYellowCards()    << ";"
                    << jug->getRedCards()       << ";"
                    << jug->getFaults()         << "\n";
        }
    }
    archivo.close();
}

// ─────────────────────────────────────────────────────────────
// Carga las stats de jugadores desde el archivo generado arriba
// ─────────────────────────────────────────────────────────────
void GestorArchivos::leerJugadores(Selecciones** equipos, int numEquipos) const {
    // Intentar con .csv primero, luego sin extensión (compatibilidad)
    string ruta = rutaJugadores;
    if (ruta.size() < 4 || ruta.substr(ruta.size()-4) != ".csv")
        ruta += ".csv";

    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        // Intentar sin la extensión (archivos legados)
        archivo.open(rutaJugadores.c_str());
        if (!archivo.is_open()) return;  // primera ejecución, no es error
    }
    string linea;
    // Saltar encabezado CSV
    getline(archivo, linea);
    while (getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;

        const int MAX_CAMPOS = 7;
        string campos[MAX_CAMPOS];
        int    n = parsearLinea(linea, ';', campos, MAX_CAMPOS);
        if (n < 7) continue;

        string       pais     = campos[0];
        unsigned int camiseta = 0, goles = 0, minutos = 0;
        unsigned int amarillas = 0, rojas = 0, faltas = 0;

        for (int i = 0; i < (int)campos[1].size(); i++)
            if (campos[1][i] >= '0' && campos[1][i] <= '9')
                camiseta = camiseta * 10 + (campos[1][i] - '0');
        for (int i = 0; i < (int)campos[2].size(); i++)
            if (campos[2][i] >= '0' && campos[2][i] <= '9')
                goles = goles * 10 + (campos[2][i] - '0');
        for (int i = 0; i < (int)campos[3].size(); i++)
            if (campos[3][i] >= '0' && campos[3][i] <= '9')
                minutos = minutos * 10 + (campos[3][i] - '0');
        for (int i = 0; i < (int)campos[4].size(); i++)
            if (campos[4][i] >= '0' && campos[4][i] <= '9')
                amarillas = amarillas * 10 + (campos[4][i] - '0');
        for (int i = 0; i < (int)campos[5].size(); i++)
            if (campos[5][i] >= '0' && campos[5][i] <= '9')
                rojas = rojas * 10 + (campos[5][i] - '0');
        for (int i = 0; i < (int)campos[6].size(); i++)
            if (campos[6][i] >= '0' && campos[6][i] <= '9')
                faltas = faltas * 10 + (campos[6][i] - '0');

        // Busca el equipo y actualiza el jugador por número de camiseta
        for (int e = 0; e < numEquipos; e++) {
            if (!equipos[e]) continue;
            if (equipos[e]->getname() == pais) {
                jugadores* jug = equipos[e]->getPlayerByShirt(camiseta);
                if (jug)
                    jug->actualizarstats(goles, minutos, amarillas, rojas, faltas);
                break;
            }
        }
    }
    archivo.close();
}

GestorArchivos::~GestorArchivos() {}
