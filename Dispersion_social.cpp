// Librerias estándar
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <cstdio>
#include <chrono>

// Librerias externas
#include <armadillo>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>	// To threads in SFML
#include <omp.h> 		// To OpenMP

// Clases propias

#include "Agente.h"
#include "Tools.h"

int N=1000;
int resolucion=50; // 10, 50 , 100, 200
float scale=0.2;//200/resolucion;
std::ofstream rol;
//ofstream times;

// *** Global variables to physics calculation ***

// Person number
std::vector<Agente_Universitario>  Personas;
std::random_device Rd;
std::mt19937 Gen(68); // gen(Rd()) 
std::uniform_real_distribution<double> Real_dist(0.0,1.0);
std::uniform_int_distribution<int> Int_dist(0,resolucion*resolucion); 
arma::sp_mat Madyacencia_sp;
arma::ivec Usables_vec;
arma::mat PosicionNodos_0;
double dt_Global;
double t_Global;
sf::Clock ClockPhysics;
sf::Time TimeUpdatePhysics = sf::milliseconds(1); // milliseconds (Int32) or microseconds (Int64)

void init_personas_activities(int t_spawn, float cap_basura, float t_actividad, double vel, bool verbose, arma::sp_mat & Mapa, arma::ivec & Usables,arma::mat PosicionNodos );


void events_sfml_manager(sf::RenderWindow &window, sf::Event &event,
			 sf::View &viewPrincipal, sf::Texture &textFondo,
			 Tools &tools, char &opcionesDeFondo,
			 sf::Vector2f &viewPricipalCenter,
			 sf::Sprite &sprFondo,sf::Texture &textFondoNodos ,
			 float dxViewPrincipal, float dyViewPrincipal, float zoomViewPrincipal);


void physics();

bool verbose=false;

int main(int argc, char **argv)
	{
		try{if(std::stoi(argv[1])==1){verbose=false;};}
		catch (...){verbose=false;}

		Tools tools;

		rol.open("rol.csv");
		dt_Global = 1;
		
		Personas.resize(N);


		t_Global=0;
		int t_spawn=0; //por ahora todos se crean al tiempo
		float cap_basura=1; //ahora mismo no hace nada
		float t_actividad=7200;
		double vel=0.05;

		arma::mat imagen= load_imagen(resolucion);
		int n=imagen.n_cols;
		int nn=n*n;
		std::cout<<"Resolucion: "<<resolucion<<"pixeles \t"<<nn*nn<<" nodos\n";
		std::vector<float> Madyacencia(nn*nn);	
		std::vector<int> Usables(nn);
		matriz_adyacencia(Madyacencia,nn,n);
		Acceso(imagen,Madyacencia,nn,Usables);
		arma::mat Ma_arma;
		Ma_arma = arma::conv_to<arma::mat>::from(Madyacencia);
		Ma_arma.reshape(nn,nn);
		arma::sp_mat M_PlaceHolder(Ma_arma);
		Madyacencia_sp=M_PlaceHolder;
		Usables_vec = arma::conv_to<arma::ivec>::from(Usables);
		PosicionNodos_0=load_csv_arma("./Environment/Nodos_finales/nodos"+std::to_string(resolucion) +".csv");
		std::cout<<"Carga del mapa completada, inicializando angetes\n";
		
		// Madyacencia_sp.print();
		init_personas_activities(t_spawn, cap_basura, t_actividad, vel, verbose,Madyacencia_sp,Usables_vec,PosicionNodos_0); 
		
			

		sf::RenderWindow window;
		sf::Texture textFondo;
		sf::Texture textFondoNodos;
		sf::Sprite sprFondo;
		std::string figFondo="./Environment/Fondos/"+std::to_string(resolucion)+".png";
		char opcionesDeFondo=1;
		sf::Vector2u textureSize; 
		sf::Vector2f viewPricipalCenter(500, 500);
		sf::Vector2f viewPricipalSize(1000, 1000);
		sf::View viewPrincipal(viewPricipalCenter, viewPricipalSize);
				
		// Parámetros de vista
		float dxViewPrincipal=100.f;
		float dyViewPrincipal=50.f;
		float zoomViewPrincipal= 0.5;


		if(!textFondo.loadFromFile(figFondo))
			{throw std::logic_error("Por favor verifique la ruta de la imagen\n");}


		textureSize = textFondo.getSize();
		window.create(sf::VideoMode(textureSize.x, textureSize.y),"TransitoHPC");
		sprFondo.setTexture(textFondo);
		sf::Thread threadPhysics(&physics);
		threadPhysics.launch();  

		std::cout<<"--------------------------\n";


		while(window.isOpen())
			{ 
				t_Global+=dt_Global;
				sf::Event event;

				// Event manager
				events_sfml_manager(window, event, viewPrincipal,textFondo ,tools ,opcionesDeFondo ,viewPricipalCenter ,
						sprFondo ,textFondoNodos ,dxViewPrincipal, dyViewPrincipal, zoomViewPrincipal);
									

				window.clear();

				window.setView(viewPrincipal);


				window.draw(sprFondo);

				for(int jj = 0; jj < N; ++jj) 
				{
				  //fprintf(stderr,"jj:%i, t: %f\n",jj,t_Global);
					if(Personas[jj].getActividad()!=0) 
						{	
						
							Personas[jj].draw(window,PosicionNodos_0,scale);
							// std::cout<<t_Global<<"\n";
							
						}	
							       					
				}

				window.display();

			}

		threadPhysics.terminate();

		return 0;
	}



// ******* Finalize Main *******


// Persons's movement
void physics()
	{
		sf::Time elapsed; // Se tiene en cuenta el tiempo de procesamiento  
		int N = Personas.size();

		int nodo_inicio;
		int nodo_destino;
		double prob_actv;
		
		int total =0;
		int threads=0;
		FILE *fp;
		fp = fopen("./metrics/physics.txt","a");
			
		
		while(true)
			{
				// Calculate time to sleep exactly timeUpdatePhysics was defined 

				elapsed = ClockPhysics.restart();
				total++;
				if (TimeUpdatePhysics>elapsed)
				sf::sleep(TimeUpdatePhysics-elapsed); 

				ClockPhysics.restart();
				double time;
				auto start = std::chrono::steady_clock::now();

				#pragma omp parallel private(nodo_inicio, nodo_destino, prob_actv)
					{
						int thr_id=omp_get_thread_num();
						int num_thr=omp_get_num_threads();
						int Nlocal=N/num_thr;
						int imin=thr_id*Nlocal;
						int imax=imin+Nlocal;


						if(thr_id==0)
							{threads=num_thr;}

						if(num_thr>1 && thr_id==(num_thr-1) && (N%Nlocal)!=0)
							{Nlocal+=N%num_thr;}
											
						for (int jj = imin; jj < imax; ++jj)
							{ 	
								//std::cout<<jj<<std::endl;
								if(Personas[jj].EnRuta())
									{Personas[jj].Avanzar(Madyacencia_sp,dt_Global,false);}

								if(Personas[jj].EnActividad())
									{
										//xy_to_node(inicio, nimagen);	
										nodo_inicio = Int_dist(Gen);
										nodo_destino = Int_dist(Gen);
										prob_actv = Real_dist(Gen);
										//da una nueva ruta si acaba la actividad
										Personas[jj].hacer_actividad(t_Global,dt_Global,nodo_inicio,nodo_destino,prob_actv); 
										std::cout<<jj<<"\n";
									}

								// fprintf(stderr,"Rol: %i, Cap act basura %f\n",Personas[jj].getRol(),Personas[jj].getBasuraActual());
						
							}
					}


				auto end = std::chrono::steady_clock::now();
				if(total <40 )
					{
						std::chrono::duration<double> diff = end - start;
						time = diff.count();
						fprintf(fp,"%i %f ",threads, time);	
					}
				else if(total <41 )
					{
						fprintf(fp,"\n");
						fclose(fp);
					}
			}

		return;
	}

void init_personas_activities(int t_spawn, float cap_basura, float t_actividad, double vel, bool verbose, arma::sp_mat & Mapa, arma::ivec & Usables,arma::mat PosicionNodos )
	{
		int N=Personas.size();
		int nodo_inicio;
		int nodo_destino;
		double rand_rol;
		double rand_type_actv;
		double rand_actv_acad;
		double time;
		FILE *fp;
		fp = fopen("./metrics/inits.txt","a");
		int threads=0;

		auto start = std::chrono::steady_clock::now();
		#pragma omp parallel private(nodo_inicio, nodo_destino, rand_rol, rand_type_actv, rand_actv_acad)
			{
				int thr_id=omp_get_thread_num();
				int num_thr=omp_get_num_threads();
				int Nlocal=N/num_thr;
				int imin=thr_id*Nlocal;
				int imax=imin+Nlocal;

				if(thr_id==0)//times<<num_thr<<" ";
					{threads=num_thr;}


				if(num_thr>1 && thr_id==(num_thr-1) && (N%Nlocal)!=0)
					{Nlocal+=N%num_thr;}

				

				for (int jj = imin; jj < imax; ++jj)// xy_to_node(inicio, nimagen);
					{ 	
						// std::cout<<jj<<"\t"<<imax <<"\n";
					  if(thr_id==(num_thr-1)){
					    displayProgressBar(((float)(jj-imin)/(imax-imin)));    
					  }
					
						nodo_inicio = Int_dist(Gen);
						nodo_destino = Int_dist(Gen); 
						rand_rol = Real_dist(Gen);
						rand_type_actv = Real_dist(Gen);
						rand_actv_acad = Real_dist(Gen);
						Personas[jj].inicializar(rand_rol,rand_type_actv,rand_actv_acad,t_spawn,cap_basura,t_actividad,
									nodo_inicio,nodo_destino,vel,t_Global,false,Mapa,Usables_vec,PosicionNodos);
									
					}
				std::cout<<std::endl;
			}
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = end - start;
		time = diff.count();
		//times<<time<<" ";
		fprintf(fp,"%i %f\n", threads, time);
		fclose(fp);
		return;
	}

// Events smfl manager
void events_sfml_manager(sf::RenderWindow &window, sf::Event &event,
			 sf::View &viewPrincipal, sf::Texture &textFondo,Tools &tools, char &opcionesDeFondo,
			 sf::Vector2f &viewPricipalCenter,sf::Sprite &sprFondo, sf::Texture &textFondoNodos,
			 float dxViewPrincipal, float dyViewPrincipal,float zoomViewPrincipal)
	{
		//Se mantiene en el loop si algún evento pasa
		while (window.pollEvent(event))
			{   
				bool wasUpdateByKeyboardArrows=true;
				switch (event.type)
				{
					case sf::Event::Closed:
						window.close();
						break;
													
					case sf::Event::KeyPressed:

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
							{
								std::cout<<"Move left"<<std::endl;
								sf::Vector2f posActual=viewPrincipal.getCenter();
								viewPrincipal.move(-dxViewPrincipal,0);
																		

								if(!tools.isMovMyView(textFondo,viewPrincipal))
									{
										viewPrincipal.setCenter(posActual);
										wasUpdateByKeyboardArrows=false;
									}
							}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
							{
								std::cout<<"Move Right"<<std::endl;
																		
								sf::Vector2f posActual=viewPrincipal.getCenter();
								viewPrincipal.move(dxViewPrincipal,0);
																		

								if(!tools.isMovMyView(textFondo,viewPrincipal))
									{
									viewPrincipal.setCenter(posActual);
									wasUpdateByKeyboardArrows=false;
									}
							}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
							{
								std::cout<<"Move Up"<<std::endl;
																		
								sf::Vector2f posActual=viewPrincipal.getCenter();
								viewPrincipal.move(0,-dyViewPrincipal);
																		
								if(!tools.isMovMyView(textFondo,viewPrincipal))
									{
									viewPrincipal.setCenter(posActual);
									wasUpdateByKeyboardArrows=false;
									}
																
							}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
							{
								std::cout<<"Move Down"<<std::endl;
																		
								sf::Vector2f posActual=viewPrincipal.getCenter();
								viewPrincipal.move(0,dyViewPrincipal);
																		
																
									if(!tools.isMovMyView(textFondo,viewPrincipal))
									{
									viewPrincipal.setCenter(posActual);
									wasUpdateByKeyboardArrows=false;
									}
							}
													
						// Al quedarse en un ciclo sin poder actualizar vuelve a un lugar de referencia
						if(!wasUpdateByKeyboardArrows && !tools.isMovMyView(textFondo,viewPrincipal))
							{viewPrincipal.setCenter(viewPricipalCenter);}

						if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
							{
								// Se encarga de cambiar el fondo
								std::cout<<"Tab is pressed"<<std::endl;
								if(opcionesDeFondo==3)
								{opcionesDeFondo=0;}
								switch(++opcionesDeFondo)
									{
										case 1:
											sprFondo.setTexture(textFondo);
											break;

										case 2:
											sprFondo.setTexture(textFondoNodos);
											break;

										case 3:
											sprFondo.setTexture(textFondo);
											break;
											
										default:
											sprFondo.setTexture(textFondo);
											break;	  
									}		
							}
															
						break;
														
						case sf::Event::MouseWheelScrolled:

						if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
							{
								// delta>o Up and delta <0 Down
								int delta=event.mouseWheelScroll.delta;
								// https://stackoverflow.com/questions/30961071/check-mouse-wheel-state-sfml
								// std::cout << "Mouse Wheel Scroll delta: " << delta << std::endl;
																
								//    viewPrincipal.setSize(textureSize.x, textureSize.y);

								//Zoom de forma relativa
								if (delta>0)// Más grande
									{viewPrincipal.zoom(zoomViewPrincipal);}
								else// Más pequeño
									{viewPrincipal.zoom(1/zoomViewPrincipal);}    
															
							}
													
						break;

					default:
						break;
				}
			}

		return;
	}
