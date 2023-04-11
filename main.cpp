//? Librerias estándar y extenas
#include <string>
#include <iostream>
#include <fstream>
#include <armadillo>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


//? Clases propias
#include "Camion.h"
#include "Dijkstra.h"


using namespace std;
bool verbose=false;

bool isMovMyView(sf::Texture& texture,sf::View& view)
	{
		// Se encarga de enmarcar un movimiento
		bool result=true;
		if (view.getCenter().x<=0+view.getSize().x/2 || view.getCenter().x>=texture.getSize().x - view.getSize().x/2)
			{result &= false;}
		if(view.getCenter().y<=0 +view.getSize().y/2 || view.getCenter().y>=texture.getSize().y - view.getSize().y/2)
			{result &= false;}
		
		return result;
	}



//-------
int main(int argc, char **argv){


	try{if(stoi(argv[1])==1){verbose=true;};}
	catch (...){verbose=false;}


	arma::mat Mapa= load_csv_arma("Madyacencia.txt");
	cout<< Mapa.n_cols<< "\t"<< Mapa.n_rows<<endl;
	arma::mat PosicionNodos;
	PosicionNodos.load("nodos-finales.csv", arma::raw_ascii);
	if(verbose) cout<<PosicionNodos;
	string figFondo= "./figs/Contenedores-Residuos.png";
		
	// un título, redimensionamiento y para cerrar.
	sf::RenderWindow window;
	sf::Texture textFondo;
	// Se crea el fondo como Sprite
	sf::Sprite sprFondo;

	// Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
	sf::Vector2u textureSize;  //Added to store texture size.


	sf::Vector2f viewPricipalCenter(2118.f, 3048.f);
	sf::Vector2f viewPricipalSize(800.f, 600.f);// Se inicializa una ventana de 800x600 con el título TransitoHPC
	sf::View viewPrincipal(viewPricipalCenter, viewPricipalSize);


	float moveViewPrincipalX=100.f;
	float moveViewPrincipalY=50.f;
	float zoomViewPrincipal=(float) 0.5;



	// Se crea la textura
	if(!textFondo.loadFromFile(figFondo))
	{
		cout<< "por favor cargue o verifique la imagen de fondo: "<<figFondo<<endl;
		return 1;
	}
	// Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size

	textureSize = textFondo.getSize(); //Get size of texture.
	window.create(sf::VideoMode(textureSize.x, textureSize.y),"TransitoHPC",sf::Style::Titlebar);

	//  Sprite
	sprFondo.setTexture(textFondo);

	// Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
	// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1CircleShape.php
	// Creación de contenedores
	//---------------Camion------------

	Camion cam1;
	int start=11;
	int end=0;
	arma:: ivec Ruta1=dijkstra_arma(Mapa,start,end);

	cout<< Ruta1 <<endl;
	cam1.Inicio(start,0,Ruta1,100,10);



	window.setFramerateLimit(30); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real

	double dt = 1;

	while(window.isOpen()){
		
		sf::Event event;
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
					

					if(!isMovMyView(textFondo,viewPrincipal)){
						viewPrincipal.setCenter(posActual);
						wasUpdateByKeyboardArrows=false;
					}

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					cout<<"Move Right"<<endl;
					
					sf::Vector2f posActual=viewPrincipal.getCenter();
					viewPrincipal.move(moveViewPrincipalX,0);
					

					if(!isMovMyView(textFondo,viewPrincipal)){
						viewPrincipal.setCenter(posActual);
						wasUpdateByKeyboardArrows=false;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					cout<<"Move Up"<<endl;
					
					

					sf::Vector2f posActual=viewPrincipal.getCenter();
					viewPrincipal.move(0,-moveViewPrincipalY);
					

					if(!isMovMyView(textFondo,viewPrincipal)){
						viewPrincipal.setCenter(posActual);
						wasUpdateByKeyboardArrows=false;
					}

					

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					cout<<"Move Down"<<endl;
					
					sf::Vector2f posActual=viewPrincipal.getCenter();
					viewPrincipal.move(0,moveViewPrincipalY);
					
			
					if(!isMovMyView(textFondo,viewPrincipal)){
						viewPrincipal.setCenter(posActual);
						wasUpdateByKeyboardArrows=false;
					}
				}
				// Al quedarse en un ciclo sin poder actualizar vuelve a un lugar de referencia
				if(!wasUpdateByKeyboardArrows && !isMovMyView(textFondo,viewPrincipal))
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
		//cout<<"---------------------\n";
		window.clear();
		window.setView(viewPrincipal);

		// En esta parte se colocan los objetos
		window.draw(sprFondo);
		if(cam1.Is_alive()) cam1.draw(window,Mapa,PosicionNodos);

		// Dibujan los contenedores

		
		window.display();

	}


	return 0;
}
