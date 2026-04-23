#include "fecha.h"

Fecha::Fecha() {
    mes=0;
    dia=0;
    anio=0;
}

Fecha::Fecha(unsigned int year,unsigned int month,unsigned int day) {
    mes=month;
    dia=day;
    anio=year;
}


//++++++++++++++++++++++++++++++++|GETTERS|++++++++++++++++++++++++++++++++++++++
unsigned int Fecha::getMes()const{
    return mes;
}
unsigned int Fecha::getAnio()const{
    return anio;
}

unsigned int Fecha::getDia()const{
    return dia;
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++| SETTERS|+++++++++++++++++++++++++++++++++++
void Fecha::setMes(unsigned int month){
    mes=month;
}
void Fecha::setAnio(unsigned int year){
    anio=year;
}

void Fecha::setDia(unsigned int day){
    dia=day;
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}
bool Fecha::operator==(const Fecha& otra) const {
    return (this->dia == otra.dia &&
            this->mes == otra.mes &&
            this->anio == otra.anio);
}

string Fecha::toString() const {
    return to_string(dia) + "/" + to_string(mes) + "/" + to_string(anio);
}

Fecha& Fecha::operator++(){
    this->dia++;
    return *this;
}

void Fecha::avanzarDia() {

    if(dia>29){
        dia=1;
        mes+=1;}
    else{dia++;}
    // Si tu clase ya maneja desbordamiento de mes, perfecto.
    // Si no, por ahora un simple dia++ funciona para la simulacion.
}


