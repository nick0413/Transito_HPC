#include "Nodo.h"

// Constructor
Nodo::Nodo(bool interaccionConCamion, float capacidadMaximaBasura,float cantidadDeBasura, int tipo){
	establecerInterracionConCamion(interaccionConCamion);
	establecerCapacidadMaximaBasura(capacidadMaximaBasura);
	establecerCantidadDeBasura(cantidadDeBasura);
	establecerTipo(tipo);	
}

bool Nodo::obtenerInteraccionConCamion(){
	return interaccionConCamion;
}
void Nodo::establecerInteraccionConCamion(bool interaccionConCamion){
	this->interaccionConCamion=interaccionConCamion;
	return;
}

float Nodo::obtenerCapacidadMaximaBasura(){
	return capacidadMaximaBasura;
}
void Nodo::establecerCapacidadMaximaBasura(float capacidadMaximaBasura){
	this->capacidadMaximaBasura=capacidadMaximaBasura;
	return;
}

float Nodo::obtenerCantidadDeBasura(){
	return cantidadDeBasura;
}
void Nodo:: establecerCantidadDeBasura(float cantidadDeBasura){
	this->cantidadDeBasura=cantidadDeBasura;
	return;
}

int Nodo::obtenerTipo(){
	return tipo;
}
void Nodo::establecerTipo(int tipo){
	this->tipo=tipo;
	return;
}

	


