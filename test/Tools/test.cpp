#include "Tools.h"

int main(){
  

  vector<vector<string>> datosContenedores;
  vector<Contenedor> misContenedores;
  
  string fname="datos_contenedores.txt";

  Tools tools;
  
  tools.datosStringFile(fname,datosContenedores);

  tools.vectorContenedores(fname,misContenedores);

  // for(auto cont : misContenedores){
  // cout<<"pos x: "<<cont.getPosition().x<<endl;
  //   cout<<"pos y: "<<cont.getPosition().y<<endl;
  //   cout<<"radius: "<<cont.getRadius()<<endl<<endl;
  // }
  
  string file="nodos_carretera.txt";
  vector<NodosCarretera> nodosCarretera;

  tools.vectorNodosCarretera(file,nodosCarretera);

  

  
  return 0;
}

