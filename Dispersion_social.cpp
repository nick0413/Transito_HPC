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
#include "Camion.h"
#include "Agente.h"
#include "Tools.h"

ofstream rol;
ofstream times;
//#define NUM_THREADS 3
// SFML

// https://www.sfml-dev.org/tutorials/2.5/system-thread.php
sf::Mutex myMutex; // Se usa para la  protección de datos compartidos en thread

// *** Variable globales de los contenedores ***

// *** Global variables to physics calculation ***

// Person number
std::vector<Agente_Universitario>  persons;
std::random_device rd;
std::mt19937 gen(68);
std::uniform_real_distribution<double> real_dist(0.0,1.0);
std::uniform_int_distribution<int> int_dist(0,99); 
arma::mat Mapa;
double dt;
double t;
sf::Clock clockPhysics;
sf::Time timeUpdatePhysics = sf::milliseconds(1); // milliseconds (Int32) or microseconds (Int64)

// *** Functions ***

// void actualizarCapacidadActualContenedores();

// Init persons activities
void init_persons_activities(int t_spawn, float cap_basura,
			     float t_actividad, double vel, bool verbose);

// Manager sfml evets
void events_sfml_manager(sf::RenderWindow &window, sf::Event &event,
			 sf::View &viewPrincipal, sf::Texture &textFondo,
			 Tools &tools, char &opcionesDeFondo,
			 sf::Vector2f &viewPricipalCenter, bool &showInfoContenedores,
			 sf::Sprite &sprFondo,sf::Texture &textFondoNodos ,
			 float dxViewPrincipal, float dyViewPrincipal, float zoomViewPrincipal);
// *** functions in threads
// Persons's movement
void physics();

bool verbose=false;



//-------
int main(int argc, char **argv)
{
  

  rol.open("rol.csv");
  times.open("times.txt", ios:: app);
  //omp_set_num_threads(NUM_THREADS);


  try{if(std::stoi(argv[1])==1){verbose=false;};}
  catch (...){verbose=false;}

 // Person number
  int N=100;
  // Array Person
  persons.resize(N);


  t=0;
  int t_spawn=0; //por ahora todos se crean al tiempo
  float cap_basura=0.2; //ahora mismo no hace nada
  float t_actividad=7200;
  double vel=0.05;
  
  

  rol<< "agente"<< " " << "rol"<<" "<< "prob"<< " "<< "actividad" << std::endl;
  

  // Init persons activities
  init_persons_activities(t_spawn, cap_basura, t_actividad, vel, verbose); 
  
  	
  std::cout<< persons[0].getMapa().n_cols<< "\t"<< persons[0].getMapa().n_rows<<std::endl;
  std::cout<< persons[0].getPosicionNodos().n_cols<< "\t"<< persons[0].getPosicionNodos().n_rows<<std::endl;
  if(verbose) std::cout<<persons[0].getPosicionNodos()<<std::endl;


  // ******* sfml
  arma::mat PosicionNodos=persons[0].getPosicionNodos();
  Mapa=persons[0].getMapa();
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
  /*
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

  */
  // Physics in a thread
  sf::Thread threadPhysics(&physics);
  threadPhysics.launch();

  // ******* sfml


  window.setFramerateLimit(30); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real

  dt = 1;


  while(window.isOpen()){ 
      t+=dt;
      sf::Event event;

      // Event manager
      events_sfml_manager(window, event, viewPrincipal,textFondo ,
			  tools ,opcionesDeFondo ,viewPricipalCenter ,
			  showInfoContenedores ,sprFondo ,textFondoNodos ,
			  dxViewPrincipal, dyViewPrincipal, zoomViewPrincipal);
					      
      // Es una forma de actualizar
      window.clear();

      // Se ajusta la vista
      window.setView(viewPrincipal);

			
      // En esta parte se colocan los objetos
      window.draw(sprFondo);

      // Se actualiza la posición del camión
      // if(cam1.Is_alive()) cam1.draw(window,Mapa,PosicionNodos);


      // Update persons
      for(int jj = 0; jj < N; jj++) {	
	if(persons[jj].getActividad()!=0) 
	  {	
							
	    persons[jj].draw(window,Mapa,PosicionNodos);
	    //rol<<persons[jj].getRol()<<" "<<persons[jj].getScale()<<std::endl;
	  }			       					
      }

      // Se dibujan los contenedores y la información
      /*
      for(auto contenedor : vectorContenedores)
	{
	  window.draw(contenedor);
	  if(showInfoContenedores)
	    {
	      sf::Lock lock(myMutex);
	      window.draw(contenedor.getTextPercentageCurrentlyCapacity());
	    }
	};
      */		

      // Se muestra todo
      window.display();

    }

  // Finish thread physics
  threadPhysics.terminate();

  return 0;
}



// ******* Finalize Main *******


// Persons's movement
void physics(){
  sf::Time elapsed; // Se tiene en cuenta el tiempo de procesamiento  
  int N = persons.size();

  int nodo_inicio;
  int nodo_destino;
  double prob_actv;
  
  int total =0;
  int threads=0;
  FILE *fp;
  fp = fopen("physics.txt","a");
      
    
  while(true){
    // Calculate time to sleep exactly timeUpdatePhysics was defined 
      elapsed = clockPhysics.restart();
    total++;
    if (timeUpdatePhysics>elapsed)
      sf::sleep(timeUpdatePhysics-elapsed); 

    clockPhysics.restart();
    double time;
    auto start = std::chrono::steady_clock::now();

    #pragma omp parallel private(nodo_inicio, nodo_destino, prob_actv)
    {
      int thr_id=omp_get_thread_num();
      int num_thr=omp_get_num_threads();
      if(thr_id==1 && total<3){
        //times<<num_thr<<" ";
	threads=num_thr;
      }
      int Nlocal=N/num_thr;

      int imin=thr_id*Nlocal;
      if(num_thr>1 && thr_id==(num_thr-1) && (N%Nlocal)!=0){
	      Nlocal+=N%num_thr;
      }

      int imax=imin+Nlocal;

      fprintf(stderr,"physics:: imin:%i imax:%i \n",imin,imax);
    
      for (int jj = imin; jj < imax; ++jj){ 	
        //std::cout<<jj<<std::endl;
        if(persons[jj].EnRuta()){	
          persons[jj].Avanzar(Mapa,dt,false);
        }
        if(persons[jj].EnActividad()){
            
          //xy_to_node(inicio, nimagen);	
          nodo_inicio = int_dist(gen);
          nodo_destino = int_dist(gen);
          prob_actv = real_dist(gen);
          //da una nueva ruta si acaba la actividad
          persons[jj].hacer_actividad(t,dt,nodo_inicio,nodo_destino,prob_actv); 
          //std::cout<<"Agente "<< jj<< " " << "Actividad: "<< persons[jj].getActividad()<<std::endl;
        }
        //else{persons[jj].hacer_actividad(t,dt);  }
      }
      

    }
    auto end = std::chrono::steady_clock::now();
    if(total <10 ){
      std::chrono::duration<double> diff = end - start;
      time = diff.count();

      fprintf(fp,"%i %f ",threads, time);
      //times<<time<<" ";
  
      
    }else if(total <11 ){{
	fprintf(fp,"\n");
	fclose(fp);
	//times<<"\n";
    }}
  }

  

  return;
}

void init_persons_activities(int t_spawn, float cap_basura,
			     float t_actividad, double vel, bool verbose){
  int N=persons.size();
  int nodo_inicio;
  int nodo_destino;
  double rand_rol;
  double rand_type_actv;
  double rand_actv_acad;
  double time;

  auto start = std::chrono::steady_clock::now();
  #pragma omp parallel private(nodo_inicio, nodo_destino, rand_rol, rand_type_actv, rand_actv_acad)
  {
    int thr_id=omp_get_thread_num();
    int num_thr=omp_get_num_threads();
    if(thr_id==1){
      times<<num_thr<<" ";
    }
    int Nlocal=N/num_thr;

    int imin=thr_id*Nlocal;

    if(num_thr>1 && thr_id==(num_thr-1) && (N%Nlocal)!=0){
      Nlocal+=N%num_thr;
    }

    int imax=imin+Nlocal;

  
    fprintf(stderr,"init_persons_activities:: imin:%i imax:%i \n",imin,imax);

    for (int jj = imin; jj < imax; ++jj){ 
      // xy_to_node(inicio, nimagen);
      nodo_inicio = int_dist(gen);
      nodo_destino = int_dist(gen); 
      rand_rol = real_dist(gen);
      rand_type_actv = real_dist(gen);
      rand_actv_acad = real_dist(gen);
      persons[jj].inicializar(rand_rol,rand_type_actv,rand_actv_acad,
			      t_spawn,cap_basura,t_actividad,
			      nodo_inicio,nodo_destino,vel,t,verbose);
				
      rol<< jj<< " " << persons[jj].getRol()<<" "<< rand_rol<<" "
	 <<persons[jj].getActividad() << std::endl;
    }
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = end - start;
  time = diff.count();
  times<<time<<" ";
  

  return;

} 

// Events smfl manager
void events_sfml_manager(sf::RenderWindow &window, sf::Event &event,
			 sf::View &viewPrincipal, sf::Texture &textFondo,
			 Tools &tools, char &opcionesDeFondo,
			 sf::Vector2f &viewPricipalCenter, bool &showInfoContenedores,
			 sf::Sprite &sprFondo, sf::Texture &textFondoNodos,
			 float dxViewPrincipal, float dyViewPrincipal,
			 float zoomViewPrincipal){
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

  return;
}


