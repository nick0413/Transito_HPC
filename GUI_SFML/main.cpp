#include <string>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;


bool isMovMyView(sf::Texture& texture,sf::View& view)
{
    // cout<<"view.getCenter().x "<<view.getCenter().x<<endl;
    // cout<<"view.getCenter().y "<<view.getCenter().y<<endl<<endl;
    bool result=true;
    if (view.getCenter().x<=0 || view.getCenter().x>=texture.getSize().x)
    {
        result &= false;
    }
    if(view.getCenter().y<=0 || view.getCenter().y>=texture.getSize().y)
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

    sf::Vector2f viewPricipalPosition(2118.f, 3048.f);
    sf::Vector2f viewPricipalSize(800.f, 600.f);
    sf::View viewPrincipal(viewPricipalPosition, viewPricipalSize);

    // cout<<"Center View x: "<<viewPrincipal.getCenter().x<<endl;
    // cout<<"Center View y: "<<viewPrincipal.getCenter().y<<endl;

    
    float moveViewPrincipalX=100.f;
    float moveViewPrincipalY=50.f;
    float zoomViewPrincipal=(float) 0.5;
    

    // Se crea la textura
    if(!textFondo.loadFromFile(figFondo))
    {
        cout<< "por favor cargue verifique la ruta: "<<figFondo<<endl;
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


    while(window.isOpen()){
       
        sf::Event event;
        //Se mantiene en el loop si algún evento pasa
        while (window.pollEvent(event))
        {
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
                    }

                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    cout<<"Move Right"<<endl;
                    
                    sf::Vector2f posActual=viewPrincipal.getCenter();
                    viewPrincipal.move(moveViewPrincipalX,0);
                    

                    if(!isMovMyView(textFondo,viewPrincipal)){
                        viewPrincipal.setCenter(posActual);
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    cout<<"Move Up"<<endl;
                    

                    sf::Vector2f posActual=viewPrincipal.getCenter();
                    viewPrincipal.move(0,-moveViewPrincipalY);
                    

                    if(!isMovMyView(textFondo,viewPrincipal)){
                        viewPrincipal.setCenter(posActual);
                    }

                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    cout<<"Move Down"<<endl;
                    
                    sf::Vector2f posActual=viewPrincipal.getCenter();
                    viewPrincipal.move(0,moveViewPrincipalY);
                    

                    if(!isMovMyView(textFondo,viewPrincipal)){
                        viewPrincipal.setCenter(posActual);
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
        window.setView(viewPrincipal);
        window.draw(sprFondo);
        window.display();

    }

    return 0;
}