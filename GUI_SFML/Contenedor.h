#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class Contenedor : public sf::CircleShape{
 public:
  explicit Contenedor(string);
  string nombre;
  void setName(string);
  string getName();
 private:
  string name;
  
};

Contenedor::Contenedor(string name){
  setName(name);
}

void Contenedor::setName(string name){
  this->name = name;
}

string Contenedor::getName(){
  return this->name;
}


