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
#include "Dijkstra.h"
#include "Tools.h"
#include "Agente.h"
#include "imagen_pathfind.h"

// SFML

// https://www.sfml-dev.org/tutorials/2.5/system-thread.php
sf::Mutex myMutex; // Se usa para la  protección de datos compartidos en thread


// Funciones


// SFML


bool verbose=false;



//-------
int main(int argc, char **argv)
	{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> real_dist(0.0,1.0);
	std::uniform_int_distribution<int> int_dist(0,99); 
	try{if(std::stoi(argv[1])==1){verbose=true;};}
	catch (...){verbose=false;}
	

	std::string Mapa_file = "Environment/Matriz_adyacencia_mapa.csv";
	arma::mat Mapa= load_csv_arma(Mapa_file);
	//arma::mat Mapa= load_csv_arma("./files/Madyacencia.txt");
	//std::cout<< Mapa.n_cols<< "\t"<< Mapa.n_rows<<std::endl;
	arma::mat PosicionNodos;
	PosicionNodos.load("nodos-finales.csv", arma::raw_ascii);
	if(verbose) std::cout<<PosicionNodos;
	Agente_Universitario Persona;
	int nimagen = 10;
	int nodo_inicio = 6;//xy_to_node(inicio, nimagen);
				
	int nodo_destino = 7;//xy_to_node(destino, nimagen);
	// std::cout << "Inicio: "<< nodo_inicio << "\t" << "Final: " << nodo_destino << std::endl;
	arma::ivec Ruta1 = Ruta_imagen(nodo_inicio,nodo_destino,"Environment/Usables.csv",Mapa_file,false);
	

	std::cout<< Ruta1 <<std::endl;

	//Inicializacion agente
	int t_spawn=0; //por ahora todos se crean al tiempo
	float cap_basura=0.2; //ahora mismo no hace nada
	float t_actividad=7200;
	double vel=0.2;
	double t;
	nodo_inicio=Ruta1(0);				
	double rand_rol = real_dist(gen);
	double rand_type_actv = real_dist(gen);
	double rand_actv_acad = real_dist(gen);
	Persona.inicializar(rand_rol,rand_type_actv,rand_actv_acad,t_spawn,cap_basura,t_actividad,Ruta1,nodo_inicio,vel,t);

	// ******* sfml

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
	
	char opcionesDeFondo=1;

	// Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
	sf::Vector2u textureSize;  //Added to store texture size.

	// *** Vista principal ***

	// //View ([left,top],[width,height])
	sf::Vector2f viewPricipalCenter(2118.f, 3048.f);
	sf::Vector2f viewPricipalSize(800.f, 600.f);
	sf::View viewPrincipal(viewPricipalCenter, viewPricipalSize);

		
	// Parámetros de vista
	float dxViewPrincipal=100.f;
	float dyViewPrincipal=50.f;
	float zoomViewPrincipal= 0.5;

	// ******* Contenedores *******

	std::string fileFontInformation="./fonts/DeliusSwashCaps-Regular.ttf";
	sf::Font fontInformation;
	bool showInfoContenedores=false;


	//***  Nodos de la carreteras ***

	
	// ******* Zona de finalización de inicialización *******

	// *** Configuración del fondo ***

	// Se crea la textura del fondo
	if(!textFondo.loadFromFile(figFondo))
		{
		std::cout<< "Por favor verifique la ruta: "<<figFondo<<std::endl;
		return 1;
		}

	
		
	// Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size

	textureSize = textFondo.getSize(); //Get size of texture.
	// El tercer argumento es el estilo que por defecto muestra
	// un título, redimensionamiento y para cerrar.
	window.create(sf::VideoMode(textureSize.x, textureSize.y),"TransitoHPC");
	// windowSize = window.getSize();             //Get size of window.

	//  Sprite
	sprFondo.setTexture(textFondo);

	// *** Configuración Contenedores ***

	// Creación de contenedores
	// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1CircleShape.php
		
	

	// *** Creación nodos de la carretera ***

	

	// fuente
	if(!fontInformation.loadFromFile(fileFontInformation))
		{
			std::cout<<"Por favor verifique que el archivo "<<fileFontInformation<<" exista"<<std::endl;
			return 1;
		}

	// Configuración de tamaño, color y estilo del texto de los nodos

	sf::Text auxText;
	

	// Actualización actual de los contenedores en thread
	//sf::Thread threadActualizarCapacidadActualContenedores(&actualizarCapacidadActualContenedores);
	//threadActualizarCapacidadActualContenedores.launch();


	// ******* sfml


	window.setFramerateLimit(30); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real

	double dt = 1;


	while(window.isOpen())
		{
				
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

			if(Persona.EnRuta()) Persona.Avanzar(Mapa,dt,verbose);
			//std::cout<<"---------------------\n";

			// Es una forma de actualizar
			window.clear();

			// Se ajusta la vista
			window.setView(viewPrincipal);

			
			// En esta parte se colocan los objetos
			window.draw(sprFondo);

			// Se actualiza la posición del camión
			if(Persona.EnRuta()) Persona.draw(window,Mapa,PosicionNodos);


			window.display();

		}



	return 0;
	}



			




	
