#include "statspartido.h"

statspartido::statspartido() {
    golesE1   = 0;
    golesE2   = 0;
    posesionE1 = 50.0f;
    posesionE2 = 50.0f;
    for (int i = 0; i < 11; i++) {
        titularesE1[i] = nullptr;
        titularesE2[i] = nullptr;
    }
}

void statspartido::convocados(jugadores** plantel, int equipo) {
    // Elige 11 aleatoriamente de los 26 del plantel
    // Usamos Fisher-Yates parcial sobre índices
    int indices[26];
    for (int i = 0; i < 26; i++) indices[i] = i;

    for (int i = 0; i < 11; i++) {
        int j = i + rand() % (26 - i);
        // intercambio
        int tmp   = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }

    if (equipo == 1) {
        for (int i = 0; i < 11; i++)
            titularesE1[i] = plantel[indices[i]];
    } else {
        for (int i = 0; i < 11; i++)
            titularesE2[i] = plantel[indices[i]];
    }
}

void statspartido::addGol(int equipo) {
    if (equipo == 1) golesE1++;
    else             golesE2++;
}

void statspartido::calcularPosesion(int rankE1, int rankE2) {
    // Equipo con mejor ranking (número menor) tiene más posesión.
    // Modelo simple: posesion proporcional a (1/rank) normalizada.
    // Se suma 1 para evitar división por cero si rank fuera 0.
    float invE1 = 1.0f / (float)(rankE1);
    float invE2 = 1.0f / (float)(rankE2);
    float total = invE1 + invE2;
    posesionE1  = (invE1 / total) * 100.0f;
    posesionE2  = (invE2 / total) * 100.0f;
}

unsigned int statspartido::getgol(int equipo) const {
    return (equipo == 1) ? golesE1 : golesE2;
}

float statspartido::getPosesion(int equipo) const {
    return (equipo == 1) ? posesionE1 : posesionE2;
}

jugadores* statspartido::Getplayer(int equipo, int indice) const {
    if (indice < 0 || indice > 10) return nullptr;
    return (equipo == 1) ? titularesE1[indice] : titularesE2[indice];
}

void statspartido::showconvocados() const {
    cout << "--- Titulares equipo 1 ---" << endl;
    for (int i = 0; i < 11; i++) {
        if (titularesE1[i]) titularesE1[i]->getname();
    }
    cout << "--- Titulares equipo 2 ---" << endl;
    for (int i = 0; i < 11; i++) {
        if (titularesE2[i]) titularesE2[i]->getname();
    }
}

statspartido::~statspartido() {
    // Los punteros son referencias a jugadores del plantel; no se eliminan aquí
}
