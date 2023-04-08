#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "Contenedor.h"



using namespace std;

class Tools{

 public:
  bool datosContenedores(string &,vector<vector<string>> &);
  bool vectorContenedores(string &, vector<Contenedor> &);
  
};


bool Tools::vectorContenedores(string &fileInfContenedores , vector<Contenedor> & vectorContenedores){

  int contadorInfoContenedores;
  vector<vector<string>> myDatosContenedores;

  
  // Verifica que se carguen los datos
  if(!datosContenedores(fileInfContenedores,myDatosContenedores)){
    return false;
  }

  for(int i=1;i<myDatosContenedores.size();i++){
    contadorInfoContenedores=0;
    Contenedor auxContenedor;

    // Se configura la posición
    float posX=stof(myDatosContenedores[i][contadorInfoContenedores++]);
    float posY=stof(myDatosContenedores[i][contadorInfoContenedores++]);
    auxContenedor.setPosition(posX,posY);

    // Configuración del radio
    float radius=stof(myDatosContenedores[i][contadorInfoContenedores++]);
    auxContenedor.setRadius(radius);

    // Configuración del color  
    if (strcmp(myDatosContenedores[i][contadorInfoContenedores++].c_str(),"red")==0){
      auxContenedor.setFillColor(sf::Color::Red);
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


// https://java2blog.com/read-csv-file-in-cpp/
bool Tools::datosContenedores(string &fname,vector<vector<string>> &content){
  vector <string> row;
  string line,word;
  fstream file (fname,ios::in);	// ios::in es para leer
  if(file.is_open()){

    while(getline(file,line)){

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




