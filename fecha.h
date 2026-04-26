#ifndef FECHA_H
#define FECHA_H
#include <string>
#include "medidor.h"

using namespace std;
class Fecha
{
    unsigned int mes;
    unsigned int dia;
    unsigned int anio;
public:
    Fecha();
    Fecha(unsigned int anio,unsigned int month,unsigned int day);

    //GETTERS
    unsigned int getMes()const;
    unsigned int getDia()const;
    unsigned int getAnio()const;
    //SETTERS
    void setAnio(unsigned int year);
    void setMes(unsigned int month);
    void setDia(unsigned int day);
    void setHora(unsigned int hour);
    void setMinutos(unsigned int minutes);

    // Sobrecarga de operadores de comparación
    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    Fecha& operator++();
    void avanzarDia();
    string toString() const;

    ~Fecha();   //destructor
};

#endif // FECHA_H
