#include <string>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>



using namespace std;
int main(){

    string figFondo= "./figs/Contenedores-Residuos.jpg";
    // Se inicializa una ventana de 800x600 con el título TransitoHPC 
    // El tercer argumento es el estilo que por defecto muestra
    // un título, redimensionamiento y para cerrar.
    sf::RenderWindow window(sf::VideoMode(800,600),"TransitoHPC");
    sf::Texture textFondo;
    // Se crea el fondo como Sprite
    sf::Sprite sprFondo;

    // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
    sf::Vector2u textureSize;  //Added to store texture size.
    sf::Vector2u windowSize;   //Added to store window size.

    

    // Se crea la textura
    if(!textFondo.loadFromFile(figFondo))
    {
        cout<< "por favor cargue verifique la ruta: "<<figFondo<<endl;
    }
    // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size

    textureSize = textFondo.getSize(); //Get size of texture.
    windowSize = window.getSize();             //Get size of window.

    float scaleX = (float) windowSize.x / textureSize.x;
    float scaleY = (float) windowSize.y / textureSize.y;     //Calculate scale.


    //  Sprite
    sprFondo.setTexture(textFondo);

    // Tomado de: https://stackoverflow.com/questions/36448101/2-3-1-set-scale-of-background-texture-to-renderwindow-size
    sprFondo.setScale(scaleX, scaleY);


    while(window.isOpen()){
       
        sf::Event event;
        //Se mantiene en el loop si algún evento pasa
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        // Es una forma de actualizar
        window.clear();
        
        window.draw(sprFondo);

        window.display();

    }

    return 0;
}