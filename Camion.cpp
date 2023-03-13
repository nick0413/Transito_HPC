#include "Camion.h"

Camion::Camion(int tipoBasura,vector<float> posicionCamion,float capacidadMaxima,float cantidadDeBasura){
    establecerTipoBasura(tipoBasura);
    establecerPosicionCamion(posicionCamion);
    establecerCapacidadMaxima(capacidadMaxima);
    establecerCantidadBasura(cantidadDeBasura);
}

int Camion::obtenerTipoBasura(){
    return this->tipoBasura;
}
void Camion::establecerTipoBasura(int tipoBasura){
    this->tipoBasura = tipoBasura;
    return;
}

vector<float> Camion::obtenerPosicionCamion(){
    return this->posicionCamion;
}
void Camion::establecerPosicionCamion(vector<float> posicionCamion){
    this->posicionCamion = posicionCamion;
}

float Camion::obtenerCapacidadMaxima(){
    return this->capacidadMaxima;
}
void Camion::establecerCapacidadMaxima(float capacidadMaxima){
    this->capacidadMaxima = capacidadMaxima;
}

float Camion::obtenerCantidadBasura(){
    return this-> cantidadBasura;
}
void Camion::establecerCantidadBasura(float cantidadBasura){
    if (cantidadBasura > this-> capacidadMaxima){
        this-> cantidadBasura = 0.0;
        sobrepasoCapacidadMaxima=true;
    }
    else {
        this-> cantidadBasura =  cantidadBasura;
        sobrepasoCapacidadMaxima=false;
    } 
    return;
}

