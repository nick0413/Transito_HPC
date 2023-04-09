#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "Contenedor.h"
#include "NodosCarretera.h"


using namespace std;

class Tools{

 public:
  
  bool datosStringFile(string &,vector<vector<string>> &);
  
  bool vectorContenedores(string &, vector<Contenedor> &);

  bool vectorNodosCarretera(string &,vector<NodosCarretera> &);

  bool isMovMyView(sf::Texture &, sf::View &);
};


bool Tools::vectorContenedores(string &fileInfContenedores , vector<Contenedor> & vectorContenedores){

  int contadorInfoContenedores;
  vector<vector<string>> myDatosContenedores;

  
  // Verifica que se carguen los datos
  if(!datosStringFile(fileInfContenedores,myDatosContenedores)){
    return false;
  }

  for(int i=1;i<myDatosContenedores.size();i++){
    contadorInfoContenedores=0;
    Contenedor auxContenedor;

    // Configuración del radio
    float radius=stof(myDatosContenedores[i][contadorInfoContenedores++]);
    auxContenedor.setRadius(radius);

    
    // Se configura la posición
    float posX=stof(myDatosContenedores[i][contadorInfoContenedores++])-radius; // Se resta el radio para que quede centrado
    float posY=stof(myDatosContenedores[i][contadorInfoContenedores++])-radius; 
    auxContenedor.setPosition(posX,posY);


    // Configuración del color
    string colorStr=myDatosContenedores[i][contadorInfoContenedores++];
    boost::algorithm::to_lower(colorStr);

    sf::Color color(0,0,0); 
    
    if (strcmp(colorStr.c_str(),"red")==0){
      auxContenedor.setFillColor(sf::Color::Red);
    }
    else if (strcmp(colorStr.c_str(),"yellow")==0){
      auxContenedor.setFillColor(sf::Color::Yellow);
    }
    else if (strcmp(colorStr.c_str(),"orange")==0){
      color.r=255;
      color.g=165;
      color.b=0;
      auxContenedor.setFillColor(color);
    }
    else if (strcmp(colorStr.c_str(),"blue")==0){
      auxContenedor.setFillColor(sf::Color::Blue);
    }
    else{
      auxContenedor.setFillColor(sf::Color::Green);
    }

    // Se le coloca la información
    auxContenedor.setInformation(myDatosContenedores[i][contadorInfoContenedores++]);

    // Identificación
    auxContenedor.setId(atoi(myDatosContenedores[i][contadorInfoContenedores++].c_str()));

    // Capacidad Máxima
    auxContenedor.setMaximumCapacity(stof(myDatosContenedores[i][contadorInfoContenedores++]));

    // Capacidad actual
    auxContenedor.setCurrentCapacity(stof(myDatosContenedores[i][contadorInfoContenedores++]));
    
    // Se coloca en el vector
    vectorContenedores.push_back(auxContenedor);      
  
  
    
  }
  
  return true;
}

bool Tools::vectorNodosCarretera(string &fileNodosCarretera, vector<NodosCarretera> &myVectorNodosCarretera){

  vector<vector<string>> dataFileNodosCarretera;
  int contador;
  
  if(!datosStringFile(fileNodosCarretera,dataFileNodosCarretera)){
    return false;
  }
  

  for(int i=1;i<dataFileNodosCarretera.size();i++){
    contador=0;
    int id=atoi(dataFileNodosCarretera[i][contador++].c_str());
    float posx=stof(dataFileNodosCarretera[i][contador++]);

    float posy=stof(dataFileNodosCarretera[i][contador++]);
    
    NodosCarretera auxNodoCarretera(id,sf::Vector2f(posx,posy));


    myVectorNodosCarretera.push_back(auxNodoCarretera);
    
  }
  return true;
  
}


// https://java2blog.com/read-csv-file-in-cpp/
bool Tools::datosStringFile(string &fname,vector<vector<string>> &content){
  vector <string> row;
  string line,word;
  fstream file (fname,ios::in);	// ios::in es para leer
  if(file.is_open()){

    while(getline(file,line)){
      // https://www.geeksforgeeks.org/how-to-use-getline-in-c-when-there-are-black-lines-in-input/
      if(line.length()==0)
	continue;
      
      row.clear();
      
      stringstream str(line); 	// Para extraer cada campo

      while(getline(str,word,',')){
	// https://www.geeksforgeeks.org/boosttrim-in-cpp-library/
	boost::algorithm::trim(word);
	row.push_back(word);
      }
      content.push_back(row);
    }
  }
  else {
    return false;
  }

  return true;
  
}

bool Tools::isMovMyView(sf::Texture& texture,sf::View& view)
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

