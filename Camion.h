#include <vector>
#include <armadillo>
using namespace std;


class Camion{
    public:
		void Inicio(int nodo_deposito,int tipoBasura , arma::uvec ruta, double capacidad, double velocidad);
		void Avanzar(arma::mat Madyacencia, double dt,bool verbose);	
		int  Nodo(void);
		double Arista(void);
		void Print_pos(void);
    private:

		int Tipo;
		arma::uvec Ruta;
		double Capacidad;
		double Pos_nodo;
		double Pos_arista;
		double Vel;

};


void Camion::Inicio(int nodo_deposito,int tipoBasura , arma::uvec ruta, double capacidad, double velocidad)
	{
		Tipo=tipoBasura;
		Ruta=ruta;
		Capacidad=capacidad;
		Pos_nodo=nodo_deposito;
		Pos_arista=0;
		Vel=velocidad;
	}
void Camion::Avanzar(arma::mat Madyacencia, double dt, bool verbose)
	{
		Pos_arista+=dt*Vel;
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		double cuadra=Madyacencia(Pos_nodo,idx(0)+1);
		if(verbose) cout<<"cuadra actual: "<<cuadra<<"\nNodo actual:" <<Pos_nodo<<"\n";
		if(Pos_arista>=cuadra)
			{
				Pos_nodo=Ruta(idx(0)+1);
				Pos_arista=0;
			}
	}

int Camion::Nodo(void)
	{
		return Pos_nodo;
	}
double Camion::Arista(void)
	{
		return Pos_arista;
	}

void Camion::Print_pos(void)
	{
		cout<<Pos_nodo<<" "<<Pos_arista<<"\n";
	}