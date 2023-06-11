// Librerias estándar
#include <string>
#include <iostream>
#include <fstream>
#include <random>

// Librerias externas
#include <armadillo>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>	// Para los Thread


// Clases propias
#include "Camion.h"
#include "Agente.h"
#include "Tools.h"

ofstream rol;
// SFML

// https://www.sfml-dev.org/tutorials/2.5/system-thread.php
sf::Mutex myMutex; // Se usa para la  protección de datos compartidos en thread

// *** Variable globales de los contenedores ***

std::vector<Contenedor> vectorContenedores;
sf::Clock clockContenedor;
sf::Time tiempoActualizarCapacidadActualContenedores =sf::seconds(1.f);

// Funciones

void actualizarCapacidadActualContenedores();

// SFML


bool verbose=false;



//-------
int main(int argc, char **argv)
	{
	rol.open("rol.csv");

	try{if(std::stoi(argv[1])==1){verbose=false;};}
	catch (...){verbose=false;}


	const int N=40;

	std::string Mapa_file = "Environment/Matriz_adyacencia_mapa.csv";
	arma::mat Mapa= load_csv_arma(Mapa_file);
	//std::cout<< Mapa.n_cols<< "\t"<< Mapa.n_rows<<std::endl;
	arma::mat PosicionNodos = load_csv_arma("./nodos-finales.csv");
	std::cout<< PosicionNodos.n_cols<< "\t"<< PosicionNodos.n_rows<<std::endl;
	if(verbose) std::cout<<PosicionNodos;
	const int N=200;

	Agente_Universitario Persona[N];
	int start=11;
	int end=0;
	int seed = std::stoi(argv[2]);
	std::random_device rd;

	std::mt19937 gen(68);

	std::uniform_real_distribution<double> real_dist(0.0,1.0);
	std::uniform_int_distribution<int> int_dist(0,99); 
	int nimagen = 10;
	int t_spawn=0; //por ahora todos se crean al tiempo
	float cap_basura=0.2; //ahora mismo no hace nada
	float t_actividad=7200;
	double vel=0.05;
	double t;
	rol<< "agente"<< " " << "rol"<<" "<< "prob"<< " "<< "actividad" << std::endl;

		for (int jj = 0; jj < N; jj++)
			{   
				t=0; //tiempo inicial
				int rand_destino = int_dist(gen);
				//arma::vec destino = coord_edificios.row(rand_destino);
				//arma::vec inicio = {5,5};
				int nodo_inicio = int_dist(gen);//xy_to_node(inicio, nimagen);
				int nodo_destino = int_dist(gen);//xy_to_node(destino, nimagen				
				double rand_rol = real_dist(gen);
				double rand_type_actv = real_dist(gen);
				double rand_actv_acad = real_dist(gen);
				Persona[jj].inicializar(rand_rol,rand_type_actv,rand_actv_acad,t_spawn,cap_basura,t_actividad,nodo_inicio,nodo_destino,vel,t,verbose);
				
				rol<< jj<< " " << Persona[jj].getRol()<<" "<< rand_rol<<" "<<Persona[jj].getActividad() << std::endl;

			}
	
	std::cout<< Persona[0].getMapa().n_cols<< "\t"<< Persona[0].getMapa().n_rows<<std::endl;
	std::cout<< Persona[0].getPosicionNodos().n_cols<< "\t"<< Persona[0].getPosicionNodos().n_rows<<std::endl;
	if(verbose) std::cout<<Persona[0].getPosicionNodos()<<std::endl;
	// ******* sfml

	arma::mat PosicionNodos=Persona[0].getPosicionNodos();
	arma::mat Mapa=Persona[0].getMapa();

	// Zona de declaración de variables
	// Herramientas generales
	Tools tools;

	// Pantalla principal  
	sf::RenderWindow window;

	// *** Fondo ***

	sf::Texture textFondo;
	sf::Texture textFondoNodos;
	// Se crea el fondo como Sprite
	sf::Sprite sprFondo;
	std::string figFondo="./Environment/mapa_v7.png";
	// std::string figFondoNodos="./figs/Contenedores-Residuos-nods.png";
	char opcionesDeFondo=1;

	// Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
	sf::Vector2u textureSize;  //Added to store texture size.

	// *** Vista principal ***

	// //View ([left,top],[width,height])
	sf::Vector2f viewPricipalCenter(500, 500);
	sf::Vector2f viewPricipalSize(1000, 1000);
	sf::View viewPrincipal(viewPricipalCenter, viewPricipalSize);

		
	// Parámetros de vista
	float dxViewPrincipal=100.f;
	float dyViewPrincipal=50.f;
	float zoomViewPrincipal= 0.5;

	// ******* Contenedores *******

	std::string fileDataContenedores="./files/datos_contenedores.txt";
	std::string fileFontInformation="./fonts/DeliusSwashCaps-Regular.ttf";
	sf::Font fontInformation;
	bool showInfoContenedores=false;


	//***  Nodos de la carreteras ***

	std::vector<NodosCarretera> vectorNodosCarretera;
	std::string fileNodosCarretera="./files/nodos_carretera.txt";


	// ******* Zona de finalización de inicialización *******

	// *** Configuración del fondo ***

	// Se crea la textura del fondo
	if(!textFondo.loadFromFile(figFondo))
		{
			std::cout<< "Por favor verifique la ruta: "<<figFondo<<std::endl;
			return 1;
		}


	textureSize = textFondo.getSize(); //Get size of texture.

	window.create(sf::VideoMode(textureSize.x, textureSize.y),"TransitoHPC");

	sprFondo.setTexture(textFondo);



	if(!tools.vectorNodosCarretera(fileNodosCarretera,vectorNodosCarretera))
		{
			std::cout<<"Por favor verifique que el archivo "<<fileNodosCarretera<<" exista"<<std::endl;
			return 1;
		}

	// fuente
	if(!fontInformation.loadFromFile(fileFontInformation))
		{
			std::cout<<"Por favor verifique que el archivo "<<fileFontInformation<<" exista"<<std::endl;
			return 1;
		}

	// Configuración de tamaño, color y estilo del texto de los nodos

	sf::Text auxText;
	for (long unsigned int i=0; i<vectorContenedores.size();i++)
		{
			auxText=vectorContenedores[i].getTextPercentageCurrentlyCapacity();
			
			auxText.setFont(fontInformation);
			auxText.setCharacterSize(40);
			auxText.setFillColor(vectorContenedores[i].getFillColor());
			auxText.setStyle(sf::Text::Bold);

			// Outline text
			auxText.setOutlineThickness(auxText.getCharacterSize()*0.07);
			auxText.setOutlineColor(sf::Color::Black);
			
			// Se agrega al vector de contenedores
			vectorContenedores[i].setTextPercentageCurrentlyCapacity(auxText);

			// Outline shape
			vectorContenedores[i].setOutlineThickness(-vectorContenedores[i].getRadius()*0.07);
			vectorContenedores[i].setOutlineColor(sf::Color::Black);
		}


	// Actualización actual de los contenedores en thread
	sf::Thread threadActualizarCapacidadActualContenedores(&actualizarCapacidadActualContenedores);
	threadActualizarCapacidadActualContenedores.launch();


	// ******* sfml


	window.setFramerateLimit(30); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real

	double dt = 1;


	while(window.isOpen())
		{ 
				t+=dt;
			sf::Event event;

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
												

									if(!tools.isMovMyView(textFondo,viewPrincipal)){
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
								{
									viewPrincipal.setCenter(viewPricipalCenter);
								}

							if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
								{
									// Se encarga de cambiar el fondo
									std::cout<<"Tab is pressed"<<std::endl;
									if(opcionesDeFondo==3)
										{opcionesDeFondo=0;}
									switch(++opcionesDeFondo)
										{
											case 1:
											showInfoContenedores=false;
											sprFondo.setTexture(textFondo);
											break;
											case 2:
											showInfoContenedores=false;
											sprFondo.setTexture(textFondoNodos);
											break;
											case 3:
											showInfoContenedores=true;
											sprFondo.setTexture(textFondo);
											break;
											default:
											showInfoContenedores=false;
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
								std::cout << "Mouse Wheel Scroll delta: " << delta << std::endl;
										
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
					
				
			//Physics
		
			for(int jj = 0; jj < N; jj++)
				{	
					//std::cout<<jj<<std::endl;
					if(Persona[jj].EnRuta()) 
						{	

							Persona[jj].Avanzar(Mapa,dt,false);
						}
					if(Persona[jj].EnActividad()){
						
						int nodo_inicio = int_dist(gen);//xy_to_node(inicio, nimagen);
						int nodo_destino = int_dist(gen);
						double prob_actv = real_dist(gen);
						//
						Persona[jj].hacer_actividad(t,dt,nodo_inicio,nodo_destino,prob_actv); //da una nueva ruta si acaba la actividad
						//std::cout<<"Agente "<< jj<< " " << "Actividad: "<< Persona[jj].getActividad()<<std::endl;
					}
					//else{Persona[jj].hacer_actividad(t,dt);  }
				}
			


			// Es una forma de actualizar
			window.clear();

			// Se ajusta la vista
			window.setView(viewPrincipal);

			
			// En esta parte se colocan los objetos
			window.draw(sprFondo);

			// Se actualiza la posición del camión
			// if(cam1.Is_alive()) cam1.draw(window,Mapa,PosicionNodos);

			for(int jj = 0; jj < N; jj++)
				{	
					if(Persona[jj].getActividad()!=0) 
						{	
							Persona[jj].draw(window,Mapa,PosicionNodos);
							//rol<<Persona[jj].getRol()<<" "<<Persona[jj].getScale()<<std::endl;
						}
					
					
				}

			//std::cout<<"holi"<<std::endl;
			// Se dibujan los contenedores y la información
			for(auto contenedor : vectorContenedores)
				{
					window.draw(contenedor);
					if(showInfoContenedores)
						{
							sf::Lock lock(myMutex);
							window.draw(contenedor.getTextPercentageCurrentlyCapacity());
						}
				};
			
			// Se muestra todo
			window.display();

		}

	// Se termina el Thread de la actualización de la capacidad de los contenedores.
	threadActualizarCapacidadActualContenedores.terminate();

	return 0;
	}

void actualizarCapacidadActualContenedores()
	{
	sf::Time elapsed;
	// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	std::random_device rd; // Semilla de forma aleatoria
	std::mt19937 gen(rd()); // 32-bit Mersenne Twister by Matsumoto and Nishimura, 1998 

		while(true){

			// Se tiene en cuenta el tiempo de procesamiento  
			elapsed = clockContenedor.restart();
			sf::sleep(tiempoActualizarCapacidadActualContenedores-elapsed); 
			clockContenedor.restart();

			// Se actualiza el llenado de cada vectorContenedores[i]enedor
			for(long unsigned int i=0;i<vectorContenedores.size();i++){
			
			{
			sf::Lock lock(myMutex);
			std::uniform_real_distribution <> dis_uni(0,vectorContenedores[i].getMaximumCapacity());
			vectorContenedores[i].setCurrentCapacity(dis_uni(gen));
			}
			
			}

		}
		

	}
