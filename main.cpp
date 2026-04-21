#include <iostream>
#include "selecciones.h"
#include "partido.h"
using namespace std;

int main()
{
    srand(time(NULL));
    Selecciones españa=Selecciones("España","Escaloni",2);
    Selecciones colombia=Selecciones("Colombia","lorenzo",6);

    Partido partido(&españa,&colombia,"21/06/2026","10:10");
    partido.simular(false);

    españa.showstats();
    partido.showstats();

    cout<<"Hola mundo"<<endl;
    // cout<<"problema"<<endl;
    return 0;
}
