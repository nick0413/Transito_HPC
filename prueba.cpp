#include "Agente.h"
#include "imagen_pathfind.h"

int xy_to_node(arma::vec coordenadas, int nimagen){
	
	int x = int(coordenadas(0));
	int y = int(coordenadas(1));
	int nodo = x*nimagen+y;

	return nodo;
}
int main(int argc, char **argv){

		std::string Mapa_file = "Environment/Matriz_adyacencia_mapa.csv";
		arma::mat Mapa= load_csv_arma(Mapa_file);

		const double TMAX=10;
		
		int Pob_max=10; //Poblacion maxima de la universidad
		int Ni = 2; //poblacion inicial
		Agente_Universitario Poblacion_unal[Pob_max]; 							//Array de bichines con numero maximo de bichines
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> real_dist(0.0,1.0);
		arma::mat coord_edificios = {{0.0,0.0},{10.0,0.0},{0.0,10.0},{10.0,10.0}};
		int num_destinos = coord_edificios.n_rows;
		std::uniform_int_distribution<int> int_dist(0,99); 				//Radio de la comida
		int nimagen = 10;
		double prob;  									//variable auxiliar para mover bichines
		int prob1, prob2;  								//variables auxiliares para las mutaciones
		int gene=0;
	
		int total_bio=0,food_bio=0,Bichos_bio=0;

		int qq=0,nn=0,live_counter;
		bool Blive=false;
		// string name,name2;

		int t_spawn=0; //por ahora todos se crean al tiempo
		float cap_basura=0.2; //ahora mismo no hace nada
		float t_actividad=7200;
		double vel=0.2;
		double t;
		std::cout << "vivo" << std::endl;

		for (int jj = 0; jj < Pob_max; jj++)
			{   
				t=0; //tiempo inicial
				int rand_destino = int_dist(gen);
				//arma::vec destino = coord_edificios.row(rand_destino);
				//arma::vec inicio = {5,5};
				int nodo_inicio = 6;//xy_to_node(inicio, nimagen);
				int hhhh=int_dist(gen);
				int nodo_destino = 7;//xy_to_node(destino, nimagen);
				// std::cout << "Inicio: "<< nodo_inicio << "\t" << "Final: " << nodo_destino << std::endl;
				arma::ivec ruta = Ruta_imagen(nodo_inicio,nodo_destino,"Environment/Usables.csv",Mapa_file,false);
				
				nodo_inicio=ruta(0);
				
				double rand_rol = real_dist(gen);
				double rand_type_actv = real_dist(gen);
				double rand_actv_acad = real_dist(gen);
				Poblacion_unal[jj].inicializar(rand_rol,rand_type_actv,rand_actv_acad,t_spawn,cap_basura,t_actividad,ruta,nodo_inicio,vel,t);
				// std::cout << Poblacion_unal[jj].getRol()<<"\t" << Poblacion_unal[jj].getFacultad()<<"\t" << Poblacion_unal[jj].getActividad()<< std::endl;

			}
			int dt = 1;
			for(int t=0;t<TMAX;t+=dt)
				{
					for(int jj = 0; jj < Pob_max; jj++)
						{	
							if(Poblacion_unal[jj].EnRuta()) 
								{	
									// Poblacion_unal[jj].getRuta().as_row().print();

									Poblacion_unal[jj].Avanzar(Mapa,dt,false);
								}
						}
				}


		return 0;
	}