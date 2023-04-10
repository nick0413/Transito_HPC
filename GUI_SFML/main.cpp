// Librerias estándar
#include <string>
#include <iostream>

// Librerias externas
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>	// Para los Thread


// Clases propias
#include "Tools.h"


using namespace std;

// https://www.sfml-dev.org/tutorials/2.5/system-thread.php
sf::Mutex mutex; // Se usa para la  protección de datos compartidos en thread
sf::Vector2f posicionActualDelCamion;

void movimientoDelCamion(vector<sf::Vector2f> &,sf::Time &);


int main(){
  // Zona de declaración de variables

  // Herramientas generales
  Tools tools;
  
  // Pantalla principal  
  sf::RenderWindow window;

  // Fondo
  sf::Texture textFondo;
  // Se crea el fondo como Sprite
  sf::Sprite sprFondo;
  string figFondo="./figs/Contenedores-Residuos.jpg";
  
  // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
  sf::Vector2u textureSize;  //Added to store texture size.
  // sf::Vector2u windowSize;   //Added to store window size.
  // float scaleX ;
  // float scaleY ;     //Calculate scale.

  // Vista principal
  // //View ([left,top],[width,height])
  sf::Vector2f viewPricipalCenter(2118.f, 3048.f);
  sf::Vector2f viewPricipalSize(800.f, 600.f);
  sf::View viewPrincipal(viewPricipalCenter, viewPricipalSize);

  // cout<<"Center View x: "<<viewPrincipal.getCenter().x<<endl;
  // cout<<"Center View y: "<<viewPrincipal.getCenter().y<<endl;

    
  // Parámetros de vista
  float moveViewPrincipalX=100.f;
  float moveViewPrincipalY=50.f;
  float zoomViewPrincipal=(float) 0.5;
  
  // ******* Contenedores *******
  
  string fileDataContenedores="./files/datos_contenedores.txt";
  vector<Contenedor> vectorContenedores;
  string fileFontInformation="./fonts/DeliusSwashCaps-Regular.ttf";
  sf::Font fontInformation;  

  // ****** Camión ****** 
  sf::Texture textCamion;
  sf::Sprite sprCamion;
  string fileCamion="./figs/trashmaster.png";
  sf::Vector2f defaultPositionCamion(1188,1722);
  vector<sf::Vector2f> movimientoCamion;
  sf::Time tiempoMovimientoCamion =sf::seconds(10.f);
  
  
  // Nodos de la carreteras
  vector<NodosCarretera> vectorNodosCarretera;
  string fileNodosCarretera="./files/nodos_carretera.txt";
  
  // Zona de finalización de inicialización
    
  // Se crea la textura del fondo
  if(!textFondo.loadFromFile(figFondo))
    {
      cout<< "por favor cargue verifique la ruta: "<<figFondo<<endl;
      return 1;
    }
  // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size

  textureSize = textFondo.getSize(); //Get size of texture.
  // El tercer argumento es el estilo que por defecto muestra
  // un título, redimensionamiento y para cerrar.
  window.create(sf::VideoMode(textureSize.x, textureSize.y),"TransitoHPC");
  // windowSize = window.getSize();             //Get size of window.

  // scaleX = (float) windowSize.x / textureSize.x;
  // scaleY = (float) windowSize.y / textureSize.y;     //Calculate scale.


  //  Sprite
  sprFondo.setTexture(textFondo);

  // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
  //sprFondo.scale(0.5, 0.5);

   // ******* Configuración Contenedores *******
  
  // Creación de contenedores
  // https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1CircleShape.php
    
  if(!tools.vectorContenedores(fileDataContenedores,vectorContenedores))
    {
      cout<<"Por favor verifique que el archivo "<<fileDataContenedores<<" exista"<<endl;
      return 1;
    }
 
  // Creación nodos de la carretera

  if(!tools.vectorNodosCarretera(fileNodosCarretera,vectorNodosCarretera)){
    cout<<"Por favor verifique que el archivo "<<fileNodosCarretera<<" exista"<<endl;
    return 1;
  }

  // fuente
  if(!fontInformation.loadFromFile(fileFontInformation)){
       cout<<"Por favor verifique que el archivo "<<fileFontInformation<<" exista"<<endl;
    return 1;
  }

 
  // Configuración de tamaño, color y estilo del texto

  sf::Text auxText;
  for (int i=0;i< vectorContenedores.size();i++){
    auxText=vectorContenedores[i].getTextPercentageCurrentlyCapacity();
    
    auxText.setFont(fontInformation);
    auxText.setCharacterSize(40);
    auxText.setFillColor(vectorContenedores[i].getFillColor());
    auxText.setStyle(sf::Text::Bold);
    // auxText.setPosition(vectorContenedores[i].getPosition()*1.01f);
    vectorContenedores[i].setTextPercentageCurrentlyCapacity(auxText);
  }
  
  // ********** Configuración Camión ************
  
  // Se crea la textura del camion
  if(!textCamion.loadFromFile(fileCamion))
    {
      cout<< "por favor cargue verifique la ruta: "<<fileCamion<<endl;
      return 1;
    }
  // Configuración de la textura al sprite del camión
  defaultPositionCamion=vectorNodosCarretera[0].getPosition();
  sprCamion.setTexture(textCamion);
  sprCamion.setOrigin(textCamion.getSize().x/2,textCamion.getSize().y/2); // origen en el centro
  sprCamion.setPosition(defaultPositionCamion);
  posicionActualDelCamion=defaultPositionCamion;

  
  // Se cargar los recorridos que debe hacer el camión
  for (auto nodCar : vectorNodosCarretera){
    movimientoCamion.push_back(nodCar.getPosition());
  }
  
  // En camión se mueve en un Thread
  sf::Thread threadMovimientoCamion(bind(&movimientoDelCamion,movimientoCamion,tiempoMovimientoCamion));
  threadMovimientoCamion.launch();


  // cout<<"Origen x: "<<sprCamion.getOrigin().x<<endl;
  // cout<<"Origen y: "<<sprCamion.getOrigin().y<<endl;
  
  
    
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
		  viewPrincipal.move(-moveViewPrincipalX,0);
                    

		  if(!tools.isMovMyView(textFondo,viewPrincipal)){
		    viewPrincipal.setCenter(posActual);
		    wasUpdateByKeyboardArrows=false;
		  }

                }
	      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
		  cout<<"Move Right"<<endl;
                    
		  sf::Vector2f posActual=viewPrincipal.getCenter();
		  viewPrincipal.move(moveViewPrincipalX,0);
                    

		  if(!tools.isMovMyView(textFondo,viewPrincipal)){
		    viewPrincipal.setCenter(posActual);
		    wasUpdateByKeyboardArrows=false;
		  }
                }
	      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
		  cout<<"Move Up"<<endl;
                    
                    

		  sf::Vector2f posActual=viewPrincipal.getCenter();
		  viewPrincipal.move(0,-moveViewPrincipalY);
                    

		  if(!tools.isMovMyView(textFondo,viewPrincipal)){
		    viewPrincipal.setCenter(posActual);
		    wasUpdateByKeyboardArrows=false;
		  }

                    

                }
	      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
		  cout<<"Move Down"<<endl;
                    
		  sf::Vector2f posActual=viewPrincipal.getCenter();
		  viewPrincipal.move(0,moveViewPrincipalY);
                    
		    
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
		      // cout<<"pos x : "<<sprCamion.getPosition().x<<endl;
		      // cout<<"pos y : "<<sprCamion.getPosition().y<<endl<<endl;
			
		    }
		  else{
		    // Más pequeño
		    viewPrincipal.zoom(1/zoomViewPrincipal);
		    // cout<<"pos x : "<<sprCamion.getPosition().x<<endl;
		    // cout<<"pos y : "<<sprCamion.getPosition().y<<endl<<endl;
		  }

                   
                
                }
        
	      break;

            default:
	      break;
            }
        }
      
		
        // Es una forma de actualizar
        window.clear();
        window.setView(viewPrincipal);

	// Se coloca el fondo de pantalla
        window.draw(sprFondo);

	// Dibujan los contenedores y la información

	for(auto contenedor : vectorContenedores){

	  window.draw(contenedor);
	  window.draw(contenedor.getTextPercentageCurrentlyCapacity());

	};
	

	// Se actualiza la posición del camión y se dibuja.
	{
	  // Bloquea mientras se ejecuta y libera ante un error
	  // https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Lock.php
	  sf::Lock lock(mutex);
	  sprCamion.setPosition(posicionActualDelCamion);
	} // Se debloquea
	
	window.draw(sprCamion);	
	
	// Se muestra todo
	window.display();

		
    }
    
    // Se termina el Thread del camión
    threadMovimientoCamion.terminate();

    return 0;
}

void movimientoDelCamion(vector<sf::Vector2f> & movimientoCamion,sf::Time & tiempoMovimientoCamion){
  while(true){
    cout<<"camión en movimiento"<<endl;
    for(auto movCam : movimientoCamion){

  
      {
	// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Lock.php
	// Bloquea mientras se ejecuta y libera ante un error
	sf::Lock lock(mutex); // mutex.lock()
	posicionActualDelCamion=movCam;
      } //Se desbloquea
      
      // cout<<"pos x : "<<sprCamion.getPosition().x<<endl;
      // cout<<"pos y : "<<sprCamion.getPosition().y<<endl<<endl;
     
      // cout<<"pos x: "<<movCam.x<<endl;
      // cout<<"pos y: "<<movCam.y<<endl<<endl;
      // Espera
      sf::sleep(tiempoMovimientoCamion);
    }
  }
}
