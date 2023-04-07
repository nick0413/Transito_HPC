#include "Tools.h"

int main(){
  

  vector<vector<string>> datosContenedores;
  vector<Contenedor> misContenedores;
  
  string fname="datos_contenedores.txt";

  Tools tools;
  
  tools.datosContenedores(fname,datosContenedores);

  for(int i=0;i<datosContenedores.size();i++){
    for (int j=0;j<datosContenedores[i].size();j++){
      cout<<datosContenedores[i][j]<<" ";
    }
    cout<<endl;
  }

  tools.vectorContenedores(fname,misContenedores);

  for(auto cont :  misContenedores){
    cout<<"Nombre: "<<cont.getName()<<endl;
    cout<<"pos x: "<<cont.getPosition().x<<endl;
    cout<<"pos y: "<<cont.getPosition().y<<endl;
    cout<<"radius: "<<cont.getRadius()<<endl<<endl;
    
  }
  
  return 0;
}

