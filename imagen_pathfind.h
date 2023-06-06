#include "Dijkstra.h"
#include <armadillo>
#include <iostream>
#include <string>

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
		if (Usables(start)==1){
			if(Usables(end)==1)
				{std::cout<<"Si\n";}
			else
				{std::cout<<end<<" No es un nodo accesible\n"; exit;}
		}
		else
			{std::cout<<start<<" No es un nodo accesible\n"; exit;}
		arma:: ivec Ruta1=dijkstra_arma(Mapa,start,end);
		std::cout<<"_\n";
		Ruta1.print();
		return Ruta1;
	}

