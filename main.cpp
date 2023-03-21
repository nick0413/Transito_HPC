#include <iostream>
#include <vector>

#include "Nodo.h"
#include "Camion.h"
using namespace std;

int main(){

    Camion camion_verde(1,(vector<float>) {2.3,2.3},3.6,3.5);
    cout<<"Tipo de basura"<<endl;
    cout<<camion_verde.obtenerTipoBasura()<<endl;
    cout<<"Posición camión"<<endl;
    for(auto& aux : camion_verde.obtenerPosicionCamion()){
        cout<<aux<<" ";
    }
    cout<<"\nCapacidad Máxima"<<endl;
    cout<<camion_verde.obtenerCapacidadMaxima()<<endl;
    cout<<"Cantidad de basura"<<endl;
    cout<<camion_verde.obtenerCantidadBasura()<<endl;
    
    if (camion_verde.sobrepasoCapacidadMaxima) {
        cout<<"Sobrepaso la capacidad máxima de basura"<<endl;
    }
    

    //Prueba clase Nodo
    Nodo nodo1(true,100,30,2);

    cout<<nodo1.obtenerInteraccionConCamion()<<endl;
    cout<<nodo1.obtenerCapacidadMaximaBasura()<<endl;
    cout<<nodo1.obtenerCantidadDeBasura()<<endl;
    cout<<nodo1.obtenerTipo()<<endl;
    return 0;
    
}