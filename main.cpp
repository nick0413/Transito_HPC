#include <iostream>
#include "Nodo.h"

using namespace std;

int main(){

    Nodo nodo1(true,100,30,2);

    cout<<nodo1.obtenerInteraccionConCamion()<<endl;
    cout<<nodo1.obtenerCapacidadMaximaBasura()<<endl;
    cout<<nodo1.obtenerCantidadDeBasura()<<endl;
    cout<<nodo1.obtenerTipo()<<endl;
    return 0;
}