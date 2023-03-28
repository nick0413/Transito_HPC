#include <string>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>



using namespace std;
int main(){

    string figFondo= "./figs/Unal-CU-Bogota.jpg";

    // Se inicializa una ventana de 800x600 con el título TransitoHPC 
    // El tercer argumento es el estilo que por defecto muestra
    // un título, redimensionamiento y para cerrar.
    sf::RenderWindow window(sf::VideoMode(800,600),"TransitoHPC");

    sf::Texture textFondo;

    // Se crea la textura
    if(!textFondo.loadFromFile(figFondo))
    {
        cout<< "por favor cargue verifique la ruta: "<<figFondo<<endl;
    }

    // Se crea el fondo como Sprite
    sf::Sprite sprFondo;

    //  Sprite
    sprFondo.setTexture(textFondo);


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