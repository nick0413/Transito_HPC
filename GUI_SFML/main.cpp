// Librerias estándar y extenas
#include <string>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Clases propias

#include "Tools.h"

using namespace std;


bool isMovMyView(sf::Texture& texture,sf::View& view)
{
    // Se encarga de enmarcar un movimiento
    // cout<<"view.getCenter().x "<<view.getCenter().x<<endl;
    // cout<<"view.getCenter().y "<<view.getCenter().y<<endl<<endl;
    bool result=true;
    if (view.getCenter().x<=0+view.getSize().x/2 || view.getCenter().x>=texture.getSize().x - view.getSize().x/2)
    {
        result &= false;
    }
    if(view.getCenter().y<=0 +view.getSize().y/2 || view.getCenter().y>=texture.getSize().y - view.getSize().y/2)
    {
        result &= false;
    }
    return result;
}


int main(){

    string figFondo= "./figs/Contenedores-Residuos.jpg";
    // Se inicializa una ventana de 800x600 con el título TransitoHPC 
    // El tercer argumento es el estilo que por defecto muestra
    // un título, redimensionamiento y para cerrar.
    sf::RenderWindow window;
    sf::Texture textFondo;
    // Se crea el fondo como Sprite
    sf::Sprite sprFondo;

    // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
    sf::Vector2u textureSize;  //Added to store texture size.
    // sf::Vector2u windowSize;   //Added to store window size.
    // float scaleX ;
    // float scaleY ;     //Calculate scale.
    // //View ([left,top],[width,height])

    sf::Vector2f viewPricipalCenter(2118.f, 3048.f);
    sf::Vector2f viewPricipalSize(800.f, 600.f);
    sf::View viewPrincipal(viewPricipalCenter, viewPricipalSize);

    // cout<<"Center View x: "<<viewPrincipal.getCenter().x<<endl;
    // cout<<"Center View y: "<<viewPrincipal.getCenter().y<<endl;

    
    float moveViewPrincipalX=100.f;
    float moveViewPrincipalY=50.f;
    float zoomViewPrincipal=(float) 0.5;
    
    // Vector de contenedores
    string fileDataContenedores="datos_contenedores.txt";
    vector<Contenedor> vectorContenedores;
    Tools tools;
    
    // Se crea la textura
    if(!textFondo.loadFromFile(figFondo))
    {
        cout<< "por favor cargue verifique la ruta: "<<figFondo<<endl;
	return 1;
    }
    // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size

    textureSize = textFondo.getSize(); //Get size of texture.
    window.create(sf::VideoMode(textureSize.x, textureSize.y),"TransitoHPC");
    // windowSize = window.getSize();             //Get size of window.

    // scaleX = (float) windowSize.x / textureSize.x;
    // scaleY = (float) windowSize.y / textureSize.y;     //Calculate scale.


    //  Sprite
    sprFondo.setTexture(textFondo);

    // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
    //sprFondo.scale(0.5, 0.5);

    // Creación de contenedores
    // https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1CircleShape.php
    
    if(!tools.vectorContenedores(fileDataContenedores,vectorContenedores))
      {
	cout<<"Por favor verifique que el archivo "<<fileDataContenedores<<" exista"<<endl;
	return 1;
      }

    
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

		
        // Es una forma de actualizar
        window.clear();
        window.setView(viewPrincipal);

	// En esta parte se colocan los objetos
        window.draw(sprFondo);

	// Dibujan los contenedores

	for(auto contenedor : vectorContenedores){
	  window.draw(contenedor);
	};
	
	window.display();

    }

    return 0;
}
