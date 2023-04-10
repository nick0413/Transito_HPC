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
			{dist(i)=value;}
		dist(source)=0;
		return dist;
	}

int get_nearest(int V,arma::vec dist, bool  *visited){
	int min_value=999;
	int min_node=0;

	for(int ii=0; ii<V-1; ii++)
		{
			if( !visited[ii] && dist(ii)<min_value)
				{
					min_value=dist(ii);
					min_node = ii;
				}
		}
	return min_node;

}

void dijkstra(int V,int source)
	{	
		bool visited[V]={0};
		arma::vec dist=in_dist(V,999,source);
		for(int ii=0; ii<V-1; ii++)
			{
				int neares_unvisited= get_nearest(V,dist,visited)
			}
	}

int main(){

	arma::mat Mapa = load_csv_arma("Madyacencia.txt");
	int V=Mapa.n_rows;
	int source=3;

	

	



}