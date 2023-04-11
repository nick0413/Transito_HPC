// Librerias estándar
#include <string>
#include <iostream>
#include <random>

// Librerias externas
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>	// Para los Thread


// Clases propias
#include "Tools.h"


using namespace std;

// https://www.sfml-dev.org/tutorials/2.5/system-thread.php
sf::Mutex mutex; // Se usa para la  protección de datos compartidos en thread

// *** Variable globales del camión ***

sf::Vector2f posicionActualDelCamion;
sf::Clock clockCamion; // Medición de tiempos del movimiento del camión
vector<sf::Vector2f> movimientoCamion; // Movimientos que debe hacer el camión
sf::Time tiempoMovimientoCamion =sf::seconds(1.f); // Tiempo real en el que se debe mover el camión


// *** Variable globales de los contenedores ***

vector<Contenedor> vectorContenedores;
sf::Clock clockContenedor;
sf::Time tiempoActualizarCapacidadActualContenedores =sf::seconds(1.f);


// Funciones

void movimientoDelCamion();
void actualizarCapacidadActualContenedores();

int main(){
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
  string figFondo="./figs/Contenedores-Residuos.jpg";
  string figFondoNodos="./figs/Contenedores-Residuos-nods.png";
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
  
  string fileDataContenedores="./files/datos_contenedores.txt";
  string fileFontInformation="./fonts/DeliusSwashCaps-Regular.ttf";
  sf::Font fontInformation;
  bool showInfoContenedores=false;

  // ****** Camión ******
  
  sf::Texture textCamion;
  sf::Sprite sprCamion;
  string fileCamion="./figs/trashmaster.png";
  sf::Vector2f defaultPositionCamion(1188,1722);
  
  
  //***  Nodos de la carreteras ***
  
  vector<NodosCarretera> vectorNodosCarretera;
  string fileNodosCarretera="./files/nodos_carretera.txt";
  

  // ******* Zona de finalización de inicialización *******

  // *** Configuración del fondo ***
  
  // Se crea la textura del fondo
  if(!textFondo.loadFromFile(figFondo))
    {
      cout<< "Por favor verifique la ruta: "<<figFondo<<endl;
      return 1;
    }

  if(!textFondoNodos.loadFromFile(figFondoNodos)){
    cout<< "Por favor verifique la ruta: "<<figFondoNodos<<endl;
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
      cout<<"Por favor verifique que el archivo "<<fileDataContenedores<<" exista"<<endl;
      return 1;
    }
 
  // *** Creación nodos de la carretera ***

  if(!tools.vectorNodosCarretera(fileNodosCarretera,vectorNodosCarretera)){
    cout<<"Por favor verifique que el archivo "<<fileNodosCarretera<<" exista"<<endl;
    return 1;
  }

  // fuente
  if(!fontInformation.loadFromFile(fileFontInformation)){
       cout<<"Por favor verifique que el archivo "<<fileFontInformation<<" exista"<<endl;
    return 1;
  }

 
  // Configuración de tamaño, color y estilo del texto de los nodos

  sf::Text auxText;
  for (int i=0;i< vectorContenedores.size();i++){
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
  
  // *** Configuración Camión ***
  
  // Se crea la textura del camion
  if(!textCamion.loadFromFile(fileCamion))
    {
      cout<< "por favor cargue verifique la ruta: "<<fileCamion<<endl;
      return 1;
    }
  
  // Configuración de la textura al sprite del camión
  defaultPositionCamion=vectorNodosCarretera[0].getPosition();
  sprCamion.setTexture(textCamion);
  // Origen del camión en el centro
  sprCamion.setOrigin(textCamion.getSize().x/2,textCamion.getSize().y/2);
  sprCamion.setPosition(defaultPositionCamion);
  posicionActualDelCamion=defaultPositionCamion;

  
  // Se cargar los recorridos que debe hacer el camión
  for (auto nodCar : vectorNodosCarretera){
    movimientoCamion.push_back(nodCar.getPosition());
  }
  
  // En camión se mueve en un Thread
  sf::Thread threadMovimientoCamion(&movimientoDelCamion);
  threadMovimientoCamion.launch();

  // Actualización actual de los contenedores en thread
  sf::Thread threadActualizarCapacidadActualContenedores(&actualizarCapacidadActualContenedores);
  threadActualizarCapacidadActualContenedores.launch();

  
  
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
		cout<<"Move left"<<endl;
		sf::Vector2f posActual=viewPrincipal.getCenter();
		viewPrincipal.move(-dxViewPrincipal,0);
                    

		if(!tools.isMovMyView(textFondo,viewPrincipal)){
		  viewPrincipal.setCenter(posActual);
		  wasUpdateByKeyboardArrows=false;
		}

	      }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	      {
		cout<<"Move Right"<<endl;
                    
		sf::Vector2f posActual=viewPrincipal.getCenter();
		viewPrincipal.move(dxViewPrincipal,0);
                    

		if(!tools.isMovMyView(textFondo,viewPrincipal)){
		  viewPrincipal.setCenter(posActual);
		  wasUpdateByKeyboardArrows=false;
		}
	      }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	      {
		cout<<"Move Up"<<endl;
                    
		sf::Vector2f posActual=viewPrincipal.getCenter();
		viewPrincipal.move(0,-dyViewPrincipal);
                    
		if(!tools.isMovMyView(textFondo,viewPrincipal)){
		  viewPrincipal.setCenter(posActual);
		  wasUpdateByKeyboardArrows=false;
		}
                    
	      }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	      {
		cout<<"Move Down"<<endl;
                    
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
	      cout<<"Tab is pressed"<<endl;

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
		cout << "Mouse Wheel Scroll delta: " << delta << endl;
                   
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
      
		
    // Es una forma de actualizar
    window.clear();

    // Se ajusta la vista
    window.setView(viewPrincipal);

    // Se coloca el fondo de pantalla
    window.draw(sprFondo);

    // Se dibujan los contenedores y la información
    for(auto contenedor : vectorContenedores){

      window.draw(contenedor);

      if(showInfoContenedores)
	{
	  sf::Lock lock(mutex);
	  window.draw(contenedor.getTextPercentageCurrentlyCapacity());
	}

    };
	

    // Se actualiza la posición del camión y se dibuja.
    {
      // Bloquea mientras se ejecuta y libera ante un error
      // https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Lock.php
      sf::Lock lock(mutex);
      sprCamion.setPosition(posicionActualDelCamion);
    } // Se desbloquea
	
    window.draw(sprCamion);	
	
    // Se muestra todo
    window.display();
		
  }
    
  // Se termina el Thread del camión
  threadMovimientoCamion.terminate();

  // Se termina el Thread de la actualización de la capacidad de los contenedores.
  threadActualizarCapacidadActualContenedores.terminate();
  return 0;
}


// Función del thread del camión
void movimientoDelCamion(){
  sf::Time elapsed;
  while(true){
    cout<<"camión en movimiento"<<endl;
    for(auto movCam : movimientoCamion){

      // Calcular el tiempo "exacto" en el que se ejecuta el thread
      elapsed = clockCamion.restart();
      sf::sleep(tiempoMovimientoCamion-elapsed);

      // cout << "Tiempo real camión: " << (clockCamion.getElapsedTime()+elapsed).asSeconds() <<endl;

      clockCamion.restart();
      
      {
	// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Lock.php
	// Bloquea mientras se ejecuta y libera ante un error
	sf::Lock lock(mutex); // mutex.lock()
	posicionActualDelCamion=movCam;
      } //Se desbloquea
      
      
    }
  }
}

void actualizarCapacidadActualContenedores(){
  sf::Time elapsed;
  // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
  random_device rd; // Semilla de forma aleatoria
  mt19937 gen(rd()); // 32-bit Mersenne Twister by Matsumoto and Nishimura, 1998 

  while(true){

    // Se tiene en cuenta el tiempo de procesamiento  
    elapsed = clockContenedor.restart();
    sf::sleep(tiempoActualizarCapacidadActualContenedores-elapsed); 
    clockContenedor.restart();

    // Se actualiza el llenado de cada vectorContenedores[i]enedor
    for(int i=0;i<vectorContenedores.size();i++){
    
      {
	sf::Lock lock(mutex);
	uniform_real_distribution <> dis_uni(0,vectorContenedores[i].getMaximumCapacity());
	vectorContenedores[i].setCurrentCapacity(dis_uni(gen));
      }
      
    }
  
    
 
  
  }
      
  
}
