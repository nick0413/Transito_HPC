#include "Dijkstra.h"
#include <armadillo>
#include <iostream>
#include <string>

int next_node(arma::vec Usables, int nodo)
	{  
		for( int ii=nodo; ii<Usables.size(); ii++)
			{
				if (Usables(ii)==1)
					{return ii;}
			}
		std::cout<< "No hay nodos accesibles"<<std::endl;
		return 0;
	}

arma::ivec Ruta_imagen(int start, int end, std::string Usables_file, std::string Mapa_file,bool verbose)
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
				{ if(verbose)std::cout<<"Si end\n";}
			else

				{
					if(verbose)std::cout<<end<<" No es un nodo accesible"; 
					end=next_node(Usables,end);
					if(verbose)std::cout<<", Se usara "<<end; 
				}
		if(Usables(start)==1)
				{if(verbose)std::cout<<"Si start\n";}
			else
				{
					if(verbose)std::cout<<start<<" No es un nodo accesible"; 
					start=next_node(Usables,start);
					if(verbose)std::cout<<", Se usara "<< start<<"\n"; 
				}
		arma:: ivec Ruta1=dijkstra_arma(Mapa,start,end);

		//Ruta1.print();
		return Ruta1;
	}

