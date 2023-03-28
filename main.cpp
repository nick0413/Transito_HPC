#include <iostream>
#include "Nodo.h"
#include "Camion.h"
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
		//cout<<i<<"\n";
		while (true)
			{
				i++;
				conexion=Madyacencia.row(current_node);
				Notfound=true;
				
				while (Notfound)
					{	
						
						//cout<<conexion<<"\n";
						idx = find(conexion==0);
						conexion.elem(idx).fill(999);
						new_node=conexion.index_min();

						if ( !arma::any(visited_nodes == new_node))
							{	
								if(verbose){cout<<"Nodo sumado al camino: "<<new_node<<"\n";}
								Notfound=false;
								visited_nodes=append_vec(visited_nodes,new_node);
								current_node=new_node;
							}
						else	
							{conexion(new_node)=999;};
						

					}
				if(current_node==end){
					//visited_nodes=append_vec(visited_nodes,current_node);
					cout<<"------------------------------" <<current_node<<endl;
					break; 
					
				};
				//if(verbose){if(i==3){break;};};
			}

		return visited_nodes;
	}

int main(int argc, char **argv){
	
	try{if(stoi(argv[1])==1){verbose=true;};}
	catch (...){verbose=false;}

	arma::mat Mapa = load_csv_arma("Mapa.csv");

	int start=4;
	int end=7;
	arma:: uvec Ruta1=Dijkstra(Mapa,start,end);
	cout<<Mapa<<endl;
	cout<<Ruta1<<endl;
	double t=0;
	double tmax=10;
	double dt=0.5;
	Camion cam1;
	cam1.Inicio(start,end,Ruta1,30,0.3);
	cout<<"Nodo incial: "<<cam1.Nodo()<<"\n";
	for(;t<tmax;t+=dt)
		{
			cam1.Avanzar(Mapa,dt,verbose);
			cam1.Print_pos();
		}
	

    return 0;
}