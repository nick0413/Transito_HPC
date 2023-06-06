#include "Dijkstra.h"
#include <armadillo>
#include <iostream>
#include <string>

int next_node(arma::vec Usables, int nodo)
	{   int ii;
		if(nodo-2<0){
			ii=0;
		}
		else{
			ii=nodo-2;
		}
		for( ; ii<Usables.size(); ii++)
			{
				if (Usables(ii)==1)
					{return ii;}
			}
		std::cout<< "No hay nodos accesibles"<<std::endl;
		return 0;
	}

arma::ivec Ruta_imagen(int start, int end, std::string Usables_file, std::string Mapa_file)
	{
		arma::mat Usables_mat= load_csv_arma(Usables_file);
		arma::vec Usables = Usables_mat.as_col();
		arma::mat Mapa= load_csv_arma(Mapa_file);


		if (start>Usables.size() || end>Usables.size())
			{
				std::cout<<start<<"->" <<end<<" Este es un mapa mas pequeño, cambiar start-end\n";
				return 0;
			}

		if(Usables(end)==1)
				{std::cout<<"Si end\n";}
			else

				{
					std::cout<<end<<" No es un nodo accesible"; 
					end=next_node(Usables,end);
					std::cout<<", Se usara "<<end; 
				}
		if(Usables(start)==1)
				{std::cout<<"Si start\n";}
			else
				{
					std::cout<<start<<" No es un nodo accesible"; 
					end=next_node(Usables,start);
					std::cout<<", Se usara "<< start; 
				}
		arma:: ivec Ruta1=dijkstra_arma(Mapa,start,end);
		std::cout<<"_\n";
		//Ruta1.print();
		return Ruta1;
	}

