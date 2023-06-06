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
		const double TMAX = stoi(argv[1]);
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
		string name,name2;
		int t_spawn=0; //por ahora todos se crean al tiempo
		float cap_basura=0.2; //ahora mismo no hace nada
		float t_actividad=7200;
		double vel=1;
		double t;
		std::cout << "vivo" << std::endl;
			for (int jj = 0; jj < Pob_max; jj++)
				{   t=0; //tiempo inicial
					int rand_destino = int_dist(gen);
					//arma::vec destino = coord_edificios.row(rand_destino);
					//arma::vec inicio = {5,5};
					int nodo_inicio =  0;//int_dist(gen);//xy_to_node(inicio, nimagen);
					int nodo_destino = 5;//int_dist(gen);//xy_to_node(destino, nimagen);
					std::cout << "Inicio: "<< nodo_inicio << "\t" << "Final: " << nodo_destino << std::endl;
					arma::ivec ruta = Ruta_imagen(nodo_inicio,nodo_destino,"Environment/Usables.csv",Mapa_file);
					
					double rand_rol = real_dist(gen);
					double rand_type_actv = real_dist(gen);
					double rand_actv_acad = real_dist(gen);
					Poblacion_unal[jj].inicializar(rand_rol,rand_type_actv,rand_actv_acad,t_spawn,cap_basura,t_actividad,ruta,nodo_inicio,vel,t);
					std::cout << Poblacion_unal[jj].getRol() << Poblacion_unal[jj].getFacultad() << Poblacion_unal[jj].getActividad() << std::endl;
					
					//std::cout << ruta << std::endl;
				}
			int dt = 2;
			
			for (int t = 0, tdibujo = 0; t < TMAX; t +=dt)
				{if(Poblacion_unal[0].IsEnRuta()) Poblacion_unal[0].Avanzar(Mapa,dt,false);
				
				}
		/*
		
		StartAnimacion(); // Dibujar


		for (int t = 0, tdibujo = 0; t < TMAX; t ++)
		{ 
			//total_bio=Fate.Biomass(food,Bichitos);
			//food_bio=Fate.food_Biomass(food);
			//Bichos_bio=Fate.Bichos_Biomass(Bichitos);
			//cout<<food_bio<<" "<<Bichos_bio<<" "<<Energy_bank<<"\n";
			//cout<<total_bio+Energy_bank<<"\n";

			if(t%100==0)
				{	
					//name="Nodos/Nodos"+to_string(t)+".csv";
					//Nodes.open(name);
					//Fate.Genetic_Nodes(Bichitos);
					//Nodes.close();
					
					//name2="Edges/Edges"+to_string(t)+".csv";
					//Edges.open(name2);
					//Fate.Genetic_Edges(Bichitos, 7);
					//Edges.close();
					
					Fate.Genetic_out(t,Bichitos);

					cout<<"t="<<t<<","<<t/TMAX*100<<"% completado. Poblacion: "<<Nlive<<"\n";
					//cout<<"std dev gen "<<gene<<": "<<Fate.Std_gene(Bichitos,gene)<<"\n";
					Fate.Haldane(t,Bichitos);
				}
			
			for (int ii = 0,nn=0; ii < Ni; ii++)  						//Para todos los bichines vivos
				{	
					if (Bichitos[ii].Alive())  										//Si el bichin esta vivo
					{ nn++;
						
																											
						prob = ran64.r();  //Genere un número aleatorio
						Bichitos[ii].Move(K, prob); //Muevase con ese numero
				

						if (Bichitos[ii].GetE() > Ehijos && Bichitos[ii].GetT() > Thijos &&  int(Bichitos[ii].GetE())%2==0)  //Si el bichin cumple las 2 condiciones para reproducirse
							{
								Blive=false;
								for(qq=0;qq<Ni;qq++)
									{
										Blive=Bichitos[qq].Alive();
										if(!Blive){break;};
									}
									if(Blive){cout<<"Poblacion maxima \n";}

								Fate.Birth(Bichitos[ii], Bichitos[qq], t, ran64);   //Escoga a un nuevo bichin del array como hijo
							}


						for (int jj = 0; jj < Nfood; jj++)
							{   //Para toda la comida, revise si puede alimentarse con ella
								food[jj].Feed(Bichitos[ii]);
							}
						if(Bichitos[ii].GetE()<10)
						{	
						//cout<<Bichitos[ii].GetE()<<"----------"<<"\n";
						}

						if(Bichitos[ii].GetE()==0)
							{Nlive-=1;
							//cout<<"muerte-----------------------------\n";
							}
						
						if(nn==Nlive)
							{break;};
					}
				}

			InicieCuadro(); //Dibuje los bichines vivos y la comida viva

			for (int ii = 0; ii < Nfood; ii++)
				{	
					if (food[ii].GetE() > 0)
					{	
						food[ii].Print();
					}
				}
			
			for (int ii = 0; ii < Ni; ii++)
				{
					if (Bichitos[ii].Alive())
					{	
						live_counter+=1;
						Bichitos[ii].Print();
					}
				}

			grafica<<t<<" "<<Nlive<<"\n";

			TermineCuadro();
			Fate.RechargeFood(food, ran64);

		}
		salida.close();
		grafica.close();
		Hald.close(); */
		return 0;
	}