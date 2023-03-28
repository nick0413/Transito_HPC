#include <iostream>
#include <vector>
#include "Nodo.h"
#include "Camion.h"
#include <eigen/Eigen/Dense>
#include <fstream>
#include <armadillo>

using namespace std;
bool verbose=false;
arma::vec append_vec(arma::vec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}
arma::uvec append_vec(arma::uvec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}
arma::mat load_csv_arma (const std::string & path) {
    arma::mat X;
    X.load(path, arma::csv_ascii);
    return X;
}

arma::uvec Dijkstra(arma::mat Madyacencia, int start, int end)
	{
		int i=0,j=0;
		int new_node;
		bool Notfound=true;
		arma::vec Dis_nodes= -1*(arma::ones<arma::vec>(Madyacencia.n_cols));
		arma::rowvec conexion=arma::vectorise(Madyacencia.row(start),1);
		Dis_nodes(start)=0;
		int current_node=start;
		double current_distance=0;
		arma::uvec visited_nodes;
		arma::uvec idx;
		visited_nodes=append_vec(visited_nodes,current_node);
		cout<<i<<"\n";
		while (true)
			{
				i++;
				conexion=Madyacencia.row(current_node);
				Notfound=true;
				
				while (Notfound)
					{	
						cout<<conexion<<"\n";
						idx = find(conexion==0);
						conexion.elem(idx).fill(999);
						new_node=conexion.index_min();

						if ( !arma::any(visited_nodes == new_node))
							{
								Notfound=false;
								visited_nodes=append_vec(visited_nodes,new_node);
								current_node=new_node;
							}
						else if (j==3)
							{break;}
						else	
							{conexion(new_node)=999;};
						

					}
				if(current_node==end){break;};
				if(verbose){if(i==3){break;};};
			}

		return visited_nodes;
	}

int main(int argc, char **argv){
	
	try
		{if(stoi(argv[1])==1){verbose=true;};}
	catch (...)
		{verbose=false;}

	arma::mat A = load_csv_arma("Mapa.csv");

	arma::mat B = {
					{0,3,0,2,0},
					{3,0,3,4,0},
					{0,3,0,1,2},
					{2,4,1,0,0},
					{0,0,2,0,0},
	              };
	arma::uvec path = Dijkstra(B,0,4);
	cout<<path<<endl;
    return 0;
}