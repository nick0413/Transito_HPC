// Librerias estándar
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>

// Librerias externas
#include <omp.h>
#include <armadillo>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>	// Para los Thread


// Clases propias
#include "Camion.h"
#include "Dijkstra.h"
#include "Tools.h"

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
int main(int argc, char **argv){


  try{if(std::stoi(argv[1])==1){verbose=true;};}
  catch (...){verbose=false;}


  arma::mat Mapa= load_csv_arma("./files/Madyacencia.txt");
  std::cout<< Mapa.n_cols<< "\t"<< Mapa.n_rows<<std::endl;
  arma::mat PosicionNodos;
  PosicionNodos.load("./files/nodos-finales.csv", arma::raw_ascii);
  if(verbose) std::cout<<PosicionNodos;

  Camion cam1;
  int start=11;
  int end=0;
  arma:: ivec Ruta1=dijkstra_arma(Mapa,start,end);

  std::cout<< Ruta1 <<std::endl;
  cam1.Inicio(start,0,Ruta1,100,10);

  // ******* sfml
  
  // Zona de declaración de variables

  // Aux variables
  long unsigned int sizeVectorContendores;

  // Herramientas generales
  Tools tools;
  
  // Pantalla principal  
  sf::RenderWindow window;

  // *** Fondo ***

  sf::Texture textFondo;
  sf::Texture textFondoNodos;
  // Se crea el fondo como Sprite
  sf::Sprite sprFondo;
  std::string figFondo="./figs/Contenedores-Residuos.jpg";
  std::string figFondoNodos="./figs/Contenedores-Residuos-nods.png";
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

  if(!textFondoNodos.loadFromFile(figFondoNodos)){
    std::cout<< "Por favor verifique la ruta: "<<figFondoNodos<<std::endl;
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
    
  if(!tools.vectorContenedores(fileDataContenedores,vectorContenedores))
    {
      std::cout<<"Por favor verifique que el archivo "<<fileDataContenedores<<" exista"<<std::endl;
      return 1;
    }
 
  // *** Creación nodos de la carretera ***

  if(!tools.vectorNodosCarretera(fileNodosCarretera,vectorNodosCarretera)){
    std::cout<<"Por favor verifique que el archivo "<<fileNodosCarretera<<" exista"<<std::endl;
    return 1;
  }

  // fuente
  if(!fontInformation.loadFromFile(fileFontInformation)){
    std::cout<<"Por favor verifique que el archivo "<<fileFontInformation<<" exista"<<std::endl;
    return 1;
  }

  // Configuración de tamaño, color y estilo del texto de los nodos

  sizeVectorContendores=vectorContenedores.size();
  sf::Text auxText;

  
  
  #pragma omp parallel
  {
    int thr_id=omp_get_thread_num();
    int num_thr=omp_get_num_threads();
    int Nlocal=std::ceil((float)sizeVectorContendores/num_thr);
    long unsigned int imin=thr_id*Nlocal;
    long unsigned int imax=imin+Nlocal;
    if(num_thr>1 && thr_id==(num_thr-1)){
      imax=imin+sizeVectorContendores%Nlocal;
    }
    
    for (long unsigned int i=imin; i<imax;i++){
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
  
  }
  
  
  // Actualización actual de los contenedores en thread
  sf::Thread threadActualizarCapacidadActualContenedores(&actualizarCapacidadActualContenedores);
  threadActualizarCapacidadActualContenedores.launch();


  // ******* sfml

  

  
  window.setFramerateLimit(30); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real

  double dt = 1;

  
  while(window.isOpen()){
		
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
                    

		if(!tools.isMovMyView(textFondo,viewPrincipal)){
		  viewPrincipal.setCenter(posActual);
		  wasUpdateByKeyboardArrows=false;
		}
	      }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	      {
		std::cout<<"Move Up"<<std::endl;
                    
		sf::Vector2f posActual=viewPrincipal.getCenter();
		viewPrincipal.move(0,-dyViewPrincipal);
                    
		if(!tools.isMovMyView(textFondo,viewPrincipal)){
		  viewPrincipal.setCenter(posActual);
		  wasUpdateByKeyboardArrows=false;
		}
                    
	      }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	      {
		std::cout<<"Move Down"<<std::endl;
                    
		sf::Vector2f posActual=viewPrincipal.getCenter();
		viewPrincipal.move(0,dyViewPrincipal);
                    
		    
		if(!tools.isMovMyView(textFondo,viewPrincipal)){
		  viewPrincipal.setCenter(posActual);
		  wasUpdateByKeyboardArrows=false;
		}
	      }
	    
	    // Al quedarse en un ciclo sin poder actualizar vuelve a un lugar de referencia
	    if(!wasUpdateByKeyboardArrows && !tools.isMovMyView(textFondo,viewPrincipal))
	      {
		viewPrincipal.setCenter(viewPricipalCenter);
	      }

	    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
	      // Se encarga de cambiar el fondo
	      std::cout<<"Tab is pressed"<<std::endl;

	      if(opcionesDeFondo==3){
		opcionesDeFondo=0;
	      }
		
	      switch(++opcionesDeFondo){
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
		if (delta>0)
		  {
		    // Más grande
		    viewPrincipal.zoom(zoomViewPrincipal);			
		  }
		else{
		  // Más pequeño
		  viewPrincipal.zoom(1/zoomViewPrincipal);
		}    
                
	      }
        
	    break;

	  default:
	    break;
	  }
      }
    		
		
    //Physics

    if(cam1.Is_alive()) cam1.Avanzar(Mapa,dt,verbose);
    //std::cout<<"---------------------\n";

    // Es una forma de actualizar
    window.clear();

    // Se ajusta la vista
    window.setView(viewPrincipal);

    
    // En esta parte se colocan los objetos
    window.draw(sprFondo);

    // Se actualiza la posición del camión
    if(cam1.Is_alive()) cam1.draw(window,Mapa,PosicionNodos);

    // Se dibujan los contenedores y la información
    for(auto contenedor : vectorContenedores){

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

void actualizarCapacidadActualContenedores(){
  sf::Time elapsed;
  // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
  std::random_device rd; // Semilla de forma aleatoria
  std::mt19937 gen(rd()); // 32-bit Mersenne Twister by Matsumoto and Nishimura, 1998 

  while(true){

    // Se tiene en cuenta el tiempo de procesamiento  
    elapsed = clockContenedor.restart();
    sf::sleep(tiempoActualizarCapacidadActualContenedores-elapsed); 
    clockContenedor.restart();

    // Se actualiza el llenado de cada vectorContenedor
    for(long unsigned int i=0;i<vectorContenedores.size();i++){
    
      {
	sf::Lock lock(myMutex);
	std::uniform_real_distribution <> dis_uni(0,vectorContenedores[i].getMaximumCapacity());
	vectorContenedores[i].setCurrentCapacity(dis_uni(gen));
      }
      
    }
  
    
 
  
  }
      
  
}
