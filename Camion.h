#include <vector>
#include <armadillo>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;


class Camion{
    public:
		void Inicio(int nodo_deposito,int tipoBasura , arma::ivec ruta, double capacidad, double velocidad);
		void Avanzar(arma::mat Madyacencia, double dt,bool verbose);	
		int  Nodo(void);
		int  Nodo_in_route(void);
		int  Next_in_route(void);
		double Arista(arma::mat Madyacencia);
		void Print_pos(void);
		arma::vec getPosition(arma::mat PosNodos, int nodo);

		void draw(sf::RenderWindow & window,arma::mat Mapa,arma::mat PosNodos);

    private:


		int Tipo;
		arma::ivec Ruta;
		sf::Sprite sprite;
		sf::Texture texture;
		double Capacidad;
		double Pos_nodo;
		double Pos_arista;
		double Vel;

};


void Camion::Inicio(int nodo_deposito,int tipoBasura , arma::ivec ruta, double capacidad, double velocidad)
	{
		Tipo=tipoBasura;
		Ruta=ruta;
		Capacidad=capacidad;
		Pos_nodo=nodo_deposito;
		Pos_arista=0;
		Vel=velocidad;

		if (!texture.loadFromFile("./figs/Camion_sprite.png"))
			{cout<<" error loading texture\n";}

		sprite.setTexture(texture);
		sf::FloatRect spriteBounds = sprite.getLocalBounds();
		sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
    
	}
void Camion::Avanzar(arma::mat Madyacencia, double dt, bool verbose)
	{
		Pos_arista+=dt*Vel;
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		double cuadra=Madyacencia(Pos_nodo,Ruta(idx(0)+1));
		if(verbose) {cout<<"cuadra actual: "<<cuadra<<"\nNodo actual:" <<Pos_nodo<<"\n"; cout<<Ruta<<"\n"; }
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
double Camion::Arista(arma::mat Madyacencia)
	{	
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		double cuadra=Madyacencia(Pos_nodo,Ruta(idx(0)+1));
		return Pos_arista/cuadra;
	}

int Camion::Nodo_in_route(void)
	{
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		return Ruta(idx(0));
	}
int Camion::Next_in_route(void)
	{
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		return Ruta(idx(0)+1);
	}

void Camion::Print_pos(void)
	{
		cout<<Pos_nodo<<" "<<Pos_arista<<"\n";
	}
arma::vec Camion::getPosition(arma::mat PosNodos, int nodo)
	{
		double posx= PosNodos(nodo, 1);
		double posy= PosNodos(nodo, 2);
		arma::vec position = {posx, posy};
		return position;
	}


void Camion::draw(sf::RenderWindow & window,arma::mat Mapa,arma::mat PosNodos)
	{	
		
			int nodo_pos=Nodo_in_route();
			int next_pos=Next_in_route();
			arma::vec nodo_actual=getPosition(PosNodos,nodo_pos);
			arma::vec nodo_siguiente=getPosition(PosNodos,next_pos);
			arma::vec r=nodo_siguiente-nodo_actual;

			arma::vec r2=arma::normalise(r,1);
			cout<<"||||||||||||||||||||||\n";
			cout<<r(0)<<"\t"<<r(1)<<"\n";
			cout<<r2(0)<<"\t"<<r2(1)<<"\n";

			if(r(0)<0)
				{sprite.setScale(-0.040f, 0.04f);}
			else
				{sprite.setScale(0.040f, 0.04f);}

			
			double pos_x=r2(0)*Pos_arista+nodo_actual(0);
			double pos_y=r2(1)*Pos_arista+nodo_actual(1);

			// cout<<nodo_pos<<"\t"<<next_pos<<"\n";

			// cout<<pos_x<<"\t"<<pos_y<<"\n";
			Print_pos();
			sprite.setPosition(sf::Vector2f(pos_x,pos_y));
			window.draw(sprite);

	}



