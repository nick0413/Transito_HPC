#include <SFML/Window.hpp>

int main(){
    
    // Se inicializa una ventana de 800x600 con el título TransitoHPC 
    // El tercer argumento es el estilo que por defecto muestra
    // un título, redimensionamiento y para cerrar.
    sf::Window window(sf::VideoMode(800,600),"TransitoHPC");

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
    }

    return 0;
}