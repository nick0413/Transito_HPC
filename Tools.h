// Librerias estándar
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Librerias externas
#include <boost/algorithm/string.hpp>




class Tools{

public:
  
  bool datosStringFile(std::string &,std::vector<std::vector<std::string>> &);
  

  bool isMovMyView(sf::Texture &, sf::View &);
};



// https://java2blog.com/read-csv-file-in-cpp/
bool Tools::datosStringFile(std::string &fname,std::vector<std::vector<std::string>> &content){
  std::vector <std::string> row;
  std::string line,word;
  std::fstream file (fname, std::ios::in);	// ios::in es para leer
  if(file.is_open()){

    while(getline(file,line)){
      // https://www.geeksforgeeks.org/how-to-use-getline-in-c-when-there-are-black-lines-in-input/
      if(line.length()==0)
	continue;
      
      row.clear();
      
      std::stringstream str(line); 	// Para extraer cada campo

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
  // std::cout<<"view.getCenter().x "<<view.getCenter().x<<endl;
  // std::cout<<"view.getCenter().y "<<view.getCenter().y<<endl<<endl;
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

