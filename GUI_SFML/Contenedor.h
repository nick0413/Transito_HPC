#include <math.h> 
#include <string>
// Precisión
#include <iomanip>
#include <sstream>

#include <SFML/Graphics.hpp>


using namespace std;

class Contenedor : public sf::CircleShape{
 public:
  explicit Contenedor(string,int,float,float);
  explicit Contenedor(string,int,float);
  explicit Contenedor(string,int);
  explicit Contenedor(string);
  explicit Contenedor();

  bool  setInformation(string);
  string getInformation();

  bool setId(int);
  int getId();

  bool setMaximumCapacity(float);
  float getMaximumCapacity();

  bool setCurrentCapacity(float);
  float getCurrentCapacity();

  bool setTextPercentageCurrentlyCapacity(sf::Text);
  sf::Text getTextPercentageCurrentlyCapacity();
  
 private:
  string information;
  int id;
  float maximumCapacity;
  float currentCapacity;
  sf::Text textPercentageCurrentlyCapacity;
};

// https://stackoverflow.com/questions/34396115/multiple-constructors-in-c
// https://www.wordreference.com/es/en/translation.asp?spen=capacidad

Contenedor::Contenedor(string information,int id, float maximumCapacity, float currentCapacity){
  setInformation(information);
  setId(id);
  setMaximumCapacity(maximumCapacity);
  setCurrentCapacity(currentCapacity);

}

Contenedor::Contenedor(string information,int id, float maximumCapacity):Contenedor(information,id, maximumCapacity, 0.0){

}

Contenedor::Contenedor(string information,int id):Contenedor(information,id, 0.0, 0.0){

}

Contenedor::Contenedor(string information):Contenedor(information,0, 0.0, 0.0){
}

Contenedor::Contenedor():Contenedor("",0, 0.0, 0.0){
}


bool Contenedor::setInformation(string information){
  this->information = information;
  return true;
}
string Contenedor::getInformation(){
  return this->information;
}

bool Contenedor::setId(int id){
  this->id=id;
  return true;
}
int Contenedor::getId(){
  return this->id;
}

bool Contenedor::setMaximumCapacity(float maximumCapacity){
  this->maximumCapacity=maximumCapacity;
  return true;
}
float Contenedor::getMaximumCapacity(){
  return this->maximumCapacity;
}

bool Contenedor::setCurrentCapacity(float currentCapacity){
  if(currentCapacity<=maximumCapacity){
    this->currentCapacity=currentCapacity;

    auto auxPercentage=currentCapacity*100/maximumCapacity;

    // Redondeo a dos cifras decimales
    auxPercentage=round(auxPercentage*100)/100;
    // https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified

    stringstream stream;
    stream << fixed << setprecision(2) << auxPercentage <<" %";
    this->textPercentageCurrentlyCapacity.setString(stream.str());
    
    return true;
  }
  else{
    this->currentCapacity=0;
    return false;
  }
}
float Contenedor::getCurrentCapacity(){
  return this->currentCapacity;
}

bool Contenedor::setTextPercentageCurrentlyCapacity(sf::Text text){
  this->textPercentageCurrentlyCapacity=text;
  return true;
}
sf::Text Contenedor::getTextPercentageCurrentlyCapacity(){
  return this->textPercentageCurrentlyCapacity;
}





