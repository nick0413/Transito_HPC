/* Externas  */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class NodosCarretera{
 public:
  explicit NodosCarretera(int,sf::Vector2f);  
  explicit NodosCarretera();
  bool setId(int);
  int getId();

  bool setPosition(sf::Vector2f);
  sf::Vector2f getPosition();
 private:
  int id;
  sf::Vector2f position;
};

NodosCarretera::NodosCarretera(int id, sf::Vector2f position){
  setId(id);
  setPosition(position);
}

NodosCarretera::NodosCarretera():NodosCarretera(0,sf::Vector2f(0,0)){}

bool NodosCarretera::setId(int id){
  this->id=id;
  return true;
}
int NodosCarretera::getId(){
  return this->id;
}

bool NodosCarretera::setPosition(sf::Vector2f position){
  this->position=position;
  return true;
}
sf::Vector2f NodosCarretera::getPosition(){
  return this->position;
}




