
#include <iostream>

// #include "Nodo.h"
#include "Camion.h"
#include <fstream>
#include <armadillo>

using namespace std;
int V;
int cost[100][100];

arma::mat load_csv_arma (const std::string & path) {
    arma::mat X;
    X.load(path, arma::csv_ascii);
    return X;
}

arma::vec in_dist(int size, int value, int source)
	{
		arma::vec dist;
		dist.resize(size);
		for (int i=0; i<size;i++)
			{dist(2)=value;}
		return dist;
	}

int main(){

	arma::mat Mapa = load_csv_arma("Madyacencia.txt");
	int V=Mapa.n_rows;
	int source=3;

	arma::vec dist=in_dist(V,999,source);

	



}