// Librerias estándar y extenas
#include <string>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
// #include "Nodo.h"
#include "Camion.h"
#include <fstream>
#include <armadillo>

// Clases propias

// #include "Tools.h"

using namespace std;
bool verbose=false;


arma::vec append_vec(arma::vec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}
arma::uvec append_vec(arma::uvec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}
arma::mat load_csv_arma (const std::string & path) {
    arma::mat X;
    X.load(path, arma::csv_ascii);
    return X;
}

arma::uvec Dijkstra(arma::mat Madyacencia, int start, int end)
	{
		int i=0,j=0;
		int new_node;
		bool Notfound=true;
		arma::vec Dis_nodes= -1*(arma::ones<arma::vec>(Madyacencia.n_cols));
		arma::rowvec conexion=arma::vectorise(Madyacencia.row(start),1);
		Dis_nodes(start)=0;
		int current_node=start;
		double current_distance=0;
		arma::uvec visited_nodes;
		arma::uvec idx;
		visited_nodes=append_vec(visited_nodes,current_node);
		//cout<<i<<"\n";
		while (true)
			{
				i++;
				conexion=Madyacencia.row(current_node);
				Notfound=true;
				
				while (Notfound)
					{	
						
						//cout<<conexion<<"\n";
						idx = find(conexion==0);
						conexion.elem(idx).fill(999);
						new_node=conexion.index_min();

						if ( !arma::any(visited_nodes == new_node))
							{	
								if(verbose){cout<<"Nodo sumado al camino: "<<new_node<<"\n";}
								Notfound=false;
								visited_nodes=append_vec(visited_nodes,new_node);
								current_node=new_node;
							}
						else	
							{conexion(new_node)=999;};
						

					}
				if(current_node==end){
					//visited_nodes=append_vec(visited_nodes,current_node);
					cout<<"------------------------------" <<current_node<<endl;
					break; 
					
				};
				//if(verbose){if(i==3){break;};};
			}

		return visited_nodes;
	}



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



//-------
int main(int argc, char **argv){
    cout<<"corre hasta aqui0"<<endl;


	try{if(stoi(argv[1])==1){verbose=true;};}
	catch (...){verbose=false;}



	int start=4;
	int end=7;
    arma::mat Mapa = load_csv_arma("Mapa.csv");
    cout<<"corre hasta aqui2mak"<<endl;

    arma:: uvec Ruta1=Dijkstra(Mapa,start,end);
    cout<<"corre hasta aqui1"<<endl;


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
    // string fileDataContenedores="datos_contenedores.txt";
    // vector<Contenedor> vectorContenedores;
    // Tools tools;
    
    // Se crea la textura
    if(!textFondo.loadFromFile(figFondo))
    {
        cout<< "por favor cargue o verifique la ruta: "<<figFondo<<endl;
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
    
    // if(!tools.vectorContenedores(fileDataContenedores,vectorContenedores))
    //   {
	// cout<<"Por favor verifique que el archivo "<<fileDataContenedores<<" exista"<<endl;
	// return 1;
    //   }
    cout<<"corre hasta aqui"<<endl;
    //---------------Camion------------
    window.setFramerateLimit(1); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real
 
    sf::RectangleShape rect;
    
    sf::Vector2f rectanglePosition(0, 4100);
 
    rect.setPosition(rectanglePosition);
    rect.setSize(sf::Vector2f(50, 50));
    rect.setOutlineColor(sf::Color::Red);
    double xVelocity = 11.6666;

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
    //Physics
    rectanglePosition.x += xVelocity;
    //rectanglePosition.y += yVelocity;
    rect.setPosition(rectanglePosition);
		
        // Es una forma de actualizar
    window.clear();
    window.setView(viewPrincipal);

	// En esta parte se colocan los objetos
    window.draw(sprFondo);
    window.draw(rect);

	// Dibujan los contenedores

	// for(auto contenedor : vectorContenedores){
	//   window.draw(contenedor);
	// };
	
	window.display();

    }

    return 0;
}
