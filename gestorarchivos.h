#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H
#include <string>
#include <fstream>
using namespace std;


class GestorArchivos
{
    string rutas;        //ojo dependiendo de cada computador
    string rutaplayers;
public:
    GestorArchivos(string rutaseleccion,string rutajugadores);
};

#endif // GESTORARCHIVOS_H
