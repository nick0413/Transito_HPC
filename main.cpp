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
arma::vec getPosition(arma::mat PosNodos,arma::uvec Ruta, int counter){
    int nodo = Ruta(counter);
    double posx= PosNodos(nodo, 1);
    double posy= PosNodos(nodo, 2);
    arma::vec position = {posx, posy};
    return position;

}
void update(arma::mat PosNodos, arma::mat Madyacencia, arma::uvec Ruta, double & xVelocity, double & yVelocity, double V, double dt, arma::vec & nodo_old, arma::vec & nodo_new, arma::vec & r, 
double & distancia, double & pasos, int i, int j, sf::Sprite &rect){
    nodo_old = getPosition(PosNodos, Ruta, i);
    nodo_new = getPosition(PosNodos, Ruta, j);
    r = nodo_new-nodo_old;
    distancia = arma::norm(r);
    pasos = distancia/V;
    xVelocity = dt*r(0)/pasos;
    yVelocity = dt*r(1)/pasos;
    rect.setScale(0.040f, 0.04f);
    if(xVelocity<0){
        rect.setScale(-0.040f, 0.04f);
    }
    else{
        rect.setScale(0.040f, 0.04f);
    }
    cout<<distancia<<"\t"<<xVelocity<<"\t"<<yVelocity<<"\t"<<nodo_old(0)<<"\t"<<nodo_old(1)<<"\t"<<Ruta(i)<<endl;
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
    arma::mat Mapa;// = load_csv_arma("Mapa.csv");
    Mapa.load("Madyacencia1.csv", arma::raw_ascii);
    cout<< Mapa.n_cols<< "\t"<< Mapa.n_rows<<endl;
    arma::mat PosicionNodos;
    PosicionNodos.load("nodos-finales.csv", arma::raw_ascii);

    arma:: uvec Ruta1=Dijkstra(Mapa,start,end);
    //arma::uvec x_sort_indices = arma::sort_index(Ruta1);
    //PosicionNodos=PosicionNodos.rows(x_sort_indices ); //Organiza los nodos en el orden de Ruta 1 

    cout<< Ruta1 <<endl;

    string figFondo= "./figs/Contenedores-Residuos-nods.png";
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
     sf::Texture spriteCamion;
    if (!spriteCamion.loadFromFile("./figs/Camion_sprite.png"))
    {
        // error loading texture
        return 1;
    }

    // create a sprite and set its texture
    sf::Sprite rect(spriteCamion);


    window.setFramerateLimit(30); //si esta en 1 se mueve en tiempo real, es cuantos pasos van a pasar por cada segundo real
 
    
    int i = 0;
    int j = 1;
    double V=11.66;
    double dt = 1;
    arma::vec pos_nodo_old;
    arma::vec pos_nodo_new;
    arma::vec r;
    double distancia;
    double pasos;
    double xVelocity;
    double yVelocity;
    update(PosicionNodos, Mapa, Ruta1, xVelocity, yVelocity, V, dt, pos_nodo_old, pos_nodo_new, r, distancia, pasos, i, j,rect);
    sf::Vector2f rectanglePosition(pos_nodo_old(0), pos_nodo_old(1));
    double distrecorrida = 0;
    rect.setPosition(rectanglePosition);
    
    //rect.setOutlineColor(sf::Color::Red);
    //rect.setOutlineThickness(5);
    cout<<xVelocity<<"\t"<<yVelocity<<endl;

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

    rectanglePosition.x += dt*xVelocity;
    rectanglePosition.y += dt*yVelocity;
    distrecorrida += sqrt(xVelocity*xVelocity+yVelocity*yVelocity);
    rect.setPosition(rectanglePosition);
    if(distrecorrida>distancia){
        i+=1;
        j+=1;
        cout<<i<<"\t"<<j<<endl;
        if(Ruta1(j)==end){break;}
        update(PosicionNodos, Mapa, Ruta1, xVelocity, yVelocity, V, dt, pos_nodo_old, pos_nodo_new, r, distancia, pasos, i, j, rect);
        distrecorrida=0;
        
    }
		
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
