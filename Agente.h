#define _USE_MATH_DEFINES 
#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include <vector>
#include <random>
#include <armadillo>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Manejo_mapa.h"



class Agente_Universitario; 

class Agente_Universitario
	{


		private:


			int rol;
			int actividad;
			int facultad;
			int spawn_time;

			float basura_actula;
			float tiempo_actividad;
			bool en_actividad;
			bool en_ruta;
			arma::ivec Ruta;
			sf::Sprite sprite;
			sf::Texture texture;
			double Pos_nodo;
			double Nodo_base;
			double Pos_arista;
			double Vel;
			float scale;
			arma::sp_mat  Mapa;
			arma::ivec  Usables;
			bool verbose;
			arma::mat PosicionNodos;
			double tactividad;
			bool vivo=true;

			// Basura
			float capacidad_basura; // kg/kg Normalizado
			float delta_produccion_basura; // kg/kg*s Normalizado
			float basura_actual; // kg/kg Normalizado

		
		public:
			void inicializar(double rand_rol_un,  double prob_tipo_actividad, double prob_actv_academica, int t_spawn,  float cap_basura, float t_actividad,
					int posicion0,int destino0, double velocidad0, double t,bool verbose, arma::sp_mat  Mapa_0,arma::ivec  Usables_0,arma::mat PosicionNodos_0);
			void asignar_rol(double prob_rol, double prob_actv_academica);
			void Actividad(double prob_tipo_actividad,double t);
			std::tuple<double, double, double> prob_actividad_est(double spawn_time, double t);
			std::tuple<double, double, double> prob_actividad_admin(double t);
			void draw(sf::RenderWindow & window,arma::mat PosNodos, float s,float ratio);

			void Avanzar(arma::sp_mat Madyacencia, double dt, bool verbose);
			int Nodo(void);
			double Arista(arma::mat Madyacencia);
			int Nodo_in_route(void);
			int Next_in_route(void);
			void Print_pos(void);
			arma::vec getNodePosition(arma::mat PosNodos, int nodo);
			arma::vec getAgentPosition(arma::mat PosNodos, float ratio,bool verbose);
			arma::vec getDirection(arma::mat PosNodos);
			arma::ivec getRuta(void){return Ruta;};
			int getRol(void){return rol;};
			float getScale(void){return scale;};
			int getActividad(void){return actividad;};
			int getFacultad(void){return facultad;};
			bool EnRuta(void){return en_ruta;};
			bool EnActividad(void){return en_actividad;};
			void asignar_pos_nodo(double pos_nodo){Pos_nodo=pos_nodo;};
			void asignar_pos_arista(double pos_arista){Pos_arista=pos_arista;};
			void asignar_ruta(int simulationTime,int nodo_i,int nodo_f);
			void hacer_actividad(double t,double dt);
			arma::sp_mat getMapa(void){return Mapa;};  
			arma::mat getPosicionNodos(void){return PosicionNodos;};
			void hacer_actividad(double t,double dt,int nodo_i,int nodo_f,double prob_tipo_actividad);

			// Basura
			bool setBasuraActual(float);
			float getBasuraActual(void);
	
	};


void Agente_Universitario::inicializar(double rand_rol_un,  double prob_tipo_actividad, double prob_actv_academica, int t_spawn,  float cap_basura, float t_actividad,
    int posicion0,int destino0, double velocidad0, double t,bool verbose0, arma::sp_mat  Mapa_0,arma::ivec  Usables_0,arma::mat PosicionNodos_0)
	{
		asignar_rol(rand_rol_un,prob_actv_academica);
		Actividad(prob_tipo_actividad,t);
		spawn_time = t_spawn;
		capacidad_basura = cap_basura;
		tiempo_actividad = t_actividad;
		en_actividad = false;
		en_ruta=true; //se asume que siempre que se inicializa se entra a la U y se toma un camino
		Vel = velocidad0;
		Pos_arista = 0;
		Mapa=Mapa_0;
		Usables=Usables_0;
		verbose= verbose0;
		PosicionNodos = PosicionNodos_0;
		Ruta = Ruta_imagen(posicion0,destino0,Usables,Mapa);
		Pos_nodo = Ruta(0);
		Nodo_base=Ruta(0);
		tactividad=0;

			
		sf::FloatRect spriteBounds = sprite.getLocalBounds();
		sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);

		// inicialización de la basura
		this->basura_actual=0.0;
		
		if(rol==0)
			{this->delta_produccion_basura=0.1;}
		else if(rol==1)
			{this->delta_produccion_basura=0.05;}
		else
			{this->delta_produccion_basura=0.025;}
  
}

void Agente_Universitario::asignar_rol(double prob_rol, double prob_actv_academica)
{
            
  if(prob_rol<0.91)
	{ //0.91
		rol = 0; //Es estudiante
		if (!texture.loadFromFile("./figs/Agente_sprite.png"))
		{std::cout<<" error loading texture\n";}

		sprite.setTexture(texture);
		sf::FloatRect spriteBounds = sprite.getLocalBounds();
		sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
		scale = 0.15f;
	}

  else if(0.91< prob_rol && prob_rol<= 0.95)
	{
		// std::cout<<"rol de admin"<<std::endl;
		rol = 1; //Es administrativo
		if (!texture.loadFromFile("./figs/Adim_sprite.png"))
		{std::cout<<" error loading texture\n";}

		sprite.setTexture(texture);
		sf::FloatRect spriteBounds = sprite.getLocalBounds();
		sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
		scale = 0.2f;
	}
            
  else{
    rol = 2; //Es docente
    if (!texture.loadFromFile("./figs/Profesor_sprite.png"))
      {std::cout<<" error loading texture\n";}

    sprite.setTexture(texture);
    sf::FloatRect spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
    scale = 0.3f;
  } 
                 

            
  //Se desarrolla una actividad academica 
      /*Actividades 
	2 clase ing
	3 clase humanas
	4 clase ciencias
	5 clase Medicina
	6 clase economicas
	7 clase artes
	8 clase derecho
	9 clase veterinaria
	10 clase agrarias 
	11 clase enfermeria
	12 clase odontologia
                    
      */
      if(0<prob_actv_academica && prob_actv_academica<=0.24)
	  {
		facultad = 2;
      }

      else if(0.24<prob_actv_academica && prob_actv_academica<=0.38){
	facultad = 3;
      }

      else if(0.38<prob_actv_academica && prob_actv_academica<=0.52){
	facultad = 4;
      }

      else if(0.52<prob_actv_academica && prob_actv_academica<=0.62){
	facultad = 5;
      }

      else if(0.62<prob_actv_academica && prob_actv_academica<=0.72){
	facultad = 6;
      }

      else if(0.72<prob_actv_academica && prob_actv_academica<=0.81){
	facultad = 7;
      }

      else if(0.81<prob_actv_academica && prob_actv_academica<=0.88){
	facultad = 8;
      }

      else if(0.88<prob_actv_academica && prob_actv_academica<=0.92){
	facultad = 9;
      }

      else if(0.92<prob_actv_academica && prob_actv_academica<=0.95){
	facultad = 10;
      }

      else if(0.95<prob_actv_academica && prob_actv_academica<=0.98){
	facultad = 11;
      }

      else{facultad = 12;}
                    
                
}


void Agente_Universitario::Actividad(double prob_tipo_actividad,double t)
	{
		double P_a;
		double P_o;
		double P_i;


		if(rol==0)//Si es estudiante
			{
				auto result = prob_actividad_est(spawn_time,t);
				P_a = 0.5;//std::get<0>(result);
				P_o = 0.3;//std::get<1>(result);
				P_i= 0.2;//std::get<2>(result);

				if(0<prob_tipo_actividad && prob_tipo_actividad <=P_a)//actividad academica
					{actividad = 1;}
				else if(P_a<prob_tipo_actividad && prob_tipo_actividad <=(P_a+P_o))//ocio o comer
					{actividad = 2;}


				else//irse de la universidad
					{actividad=0; }
			}

		if(rol==1)
			{
				auto result = prob_actividad_admin(t);
				P_a = std::get<0>(result);
				P_o = std::get<1>(result);
				P_i= std::get<2>(result);
							
				// std::cout<<"Probabilidad de irse:" <<P_i<<std::endl;
				if(0<prob_tipo_actividad && prob_tipo_actividad <=P_a)//actividad academica
					{actividad = 1;}
							
				else if(P_a<prob_tipo_actividad && prob_tipo_actividad<=(P_a+P_o))//ocio o comer
					{actividad = 2;}

				else//irse de la universidad
					{actividad=0; }
			}

		if(rol==2)
			{
				auto result = prob_actividad_admin(t);
				P_a = std::get<0>(result);
				P_o = std::get<1>(result);
				P_i= std::get<2>(result);
							
								
						
				if(0<prob_tipo_actividad && prob_tipo_actividad<=P_a){
				actividad = 1; //actividad academica
				}
							
				else if(P_a<prob_tipo_actividad && prob_tipo_actividad<=(P_a+P_o)){
				actividad = 2; //ocio o comer
				}

				else{
				actividad=0; //irse de la universidad
				}
			}


}

std::tuple<double, double, double> Agente_Universitario::prob_actividad_est(double spawn_time, double t)
	{
		double s = 0.4*3600; //desviacion estandar
		double x = t;
		double d = spawn_time; //para ir corriendo la gaussiana segun el spawn time
		double a = exp(-pow((x - spawn_time - 6.0) / (2.0 * pow(4.0, 2.0)), 2.0)) *
			(exp(-pow((x - spawn_time) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - spawn_time - 2.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - spawn_time - 4.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - spawn_time - 7.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - spawn_time - 9.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - spawn_time - 11.0) / (2.0 * pow(s, 2.0)), 2.0)));
		//funcion para la actividad academica

		double o = exp(-pow((x - d - 6.0) / (2.0 * pow(4.0, 2.0)), 2.0)) *
			(exp(-pow((x - d) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - d - 2.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			0.0 * exp(-pow((x - d - 4.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - d - 6.0) / (2.0 * pow(0.8, 2.0)), 2.0)) +
			exp(-pow((x - d - 9.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - d - 11.0) / (2.0 * pow(s, 2.0)), 2.0)));

		//funcion para actividad de ocio o comer

		double i = ((x - d) / 10.0) * exp(-pow((x - d - 3.0) / (2.0 * pow(4.0, 2.0)), 2.0)) *
			(exp(-pow((x - d) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - d - 2.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			0.0 * exp(-pow((x - d - 4.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - d - 6.0) / (2.0 * pow(0.8, 2.0)), 2.0)) +
			exp(-pow((x - d - 9.0) / (2.0 * pow(s, 2.0)), 2.0)) +
			exp(-pow((x - d - 11.0) / (2.0 * pow(s, 2.0)), 2.0))) +
			((x - d) / 18.0);

		//funcion para irse de la universidad

		double tot = a+i+o;

		double P_a = a/tot;

		double P_o = o/tot;

		double P_i = i/tot;
		return std::make_tuple(P_a, P_o, P_i);
	}
std::tuple<double, double, double> Agente_Universitario::prob_actividad_admin(double t)
{
  double inicio_dia = 0;//7*3600
  double inicio_descanso = 3600; //12*3600
  double final_descanso = 4600; //14*3600
  double final_dia = 5600; //17*3600
  double P_a;
  double P_o;
  double P_i;
  double x=t;
  double d=4000; //13*3600
  double s = 0.5*3600; 
  if(inicio_dia<t && t<=inicio_descanso){
    P_a = 0.8;
    P_o = 0.2;
    P_i = 0.0;
    return std::make_tuple(P_a, P_o, P_i);
  }

  else if(inicio_descanso<t && t<=final_descanso){
    double a =  exp(-pow((x - d) / (2.0 * pow(s, 2.0)), 2.0));
    double o = 0.5* exp(-pow((x - d) / (2.0 * pow(s, 2.0)), 2.0));
    double i = 0; 
    double tot = a+o+i;
    P_a = a/tot;
    P_o = o/tot;
    P_i = i/tot;
    return std::make_tuple(P_a, P_o, P_i);               

  }

  else if(final_descanso<t && t<=final_dia){
    P_a = 0.8;
    P_o = 0.2;
    P_i = 0.0;
    return std::make_tuple(P_a, P_o, P_i);  
  }

  else{
    P_a = 0.1;
    P_o = 0.2;
    P_i = 0.7;
    return std::make_tuple(P_a, P_o, P_i);  
  }
} 

void Agente_Universitario::Avanzar(arma::sp_mat Madyacencia, double dt, bool verbose=false)

	{	
		// std::cout<<Pos_arista<<"->"<<Pos_arista	+dt*Vel<<"\t";
		Pos_arista+=dt*Vel;
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		// Ruta.as_row().print();
		// if(verbose)std::cout<<"\t\t"<<Pos_nodo<<"\n";
		// if(verbose)std::cout<<idx.size()<<"\n";
		// if(verbose)std::cout<<idx(0)<<"\n";
		// if(verbose)std::cout<<"Condicion:"<<(idx(0)+1>Ruta.size())<<"\n";
		if(idx(0)+1>=Ruta.size())
			{
				en_ruta=false;
				en_actividad=true;
				return;
			}
		// if(verbose)std::cout<<"----\n";
		double cuadra=Madyacencia(Pos_nodo,Ruta(idx(0)+1));
		// if(verbose) {std::cout<<"cuadra actual: "<<cuadra<<"\nNodo actual:" <<Pos_nodo<<"\n"; }
		if(Pos_arista>=cuadra)
			{	
				Pos_nodo=Ruta(idx(0)+1);
				Pos_arista=0;
			}
		if(verbose) {
			// std::cout<<Pos_arista<<"->"<<Pos_arista	+dt*Vel<<"\t";
			std::cout<<"arista actual: "<<Pos_arista<<"\tEntre nodos:" <<Pos_nodo<<"->"<<Ruta(idx(0)+1)<<"\n"; 
			}


}

int Agente_Universitario::Nodo(void)
	{return Pos_nodo;}
double Agente_Universitario::Arista(arma::mat Madyacencia)
	{	
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		double cuadra=Madyacencia(Pos_nodo,Ruta(idx(0)+1));
		return Pos_arista/cuadra;
	}

int Agente_Universitario::Nodo_in_route(void)
	{
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		return Ruta(idx(0));
	}
int Agente_Universitario::Next_in_route(void)
	{	
			
		arma::uvec idx=arma::find(Ruta == Pos_nodo);
		if (idx(0)+1>=Ruta.size())
			{
				en_ruta=false;
				en_actividad=true;
				return-100;
			}
		return Ruta(idx(0)+1);
	}

void Agente_Universitario::Print_pos(void)
	{std::cout<<Pos_nodo<<" "<<Pos_arista<<"\n";}


arma::vec Agente_Universitario::getNodePosition(arma::mat PosNodos, int nodo)
	{	

		double posx= PosNodos(nodo, 1);
		double posy= PosNodos(nodo, 2);
		arma::vec position = {posx, posy};
				
		return position;
			
	}

arma::vec Agente_Universitario::getAgentPosition(arma::mat PosNodos,float ratio, bool verbose)
	{
		double pos_x;
		double pos_y;
		int nodo_pos=Nodo_in_route();
		int next_pos=Next_in_route();
		arma::vec nodo_siguiente;	
		arma::vec nodo_actual=getNodePosition(PosNodos,nodo_pos);
		
		if(next_pos==-100)
			{nodo_siguiente=nodo_actual;}
		else
			{nodo_siguiente=getNodePosition(PosNodos,next_pos);}


		arma::vec r=nodo_siguiente-nodo_actual;
		arma::vec r2=arma::normalise(r,1);
		pos_x=(ratio*r2(0)*Pos_arista)+nodo_actual(0);
		pos_y=(ratio*r2(1)*Pos_arista)+nodo_actual(1);
		arma::vec pos = {pos_x, pos_y};
		return pos;

	}
arma::vec Agente_Universitario::getDirection(arma::mat PosNodos)
	{	
		int nodo_pos=Nodo_in_route();
		int next_pos=Next_in_route();
		arma::vec nodo_siguiente;	
		arma::vec nodo_actual=getNodePosition(PosNodos,nodo_pos);

		if(next_pos==-100)
			{nodo_siguiente=nodo_actual;}
		else
			{nodo_siguiente=getNodePosition(PosNodos,next_pos);}


		arma::vec r=nodo_siguiente-nodo_actual;
		arma::vec r2=arma::normalise(r,1);
		return r2;
	}


void Agente_Universitario::draw(sf::RenderWindow & window,arma::mat PosNodos, float s,float ratio)
	{	
		double pos_x;
		double pos_y;
		arma::vec pos;
		bool errr=false;
		// std::cout<<true<<"\n";
		
		if(en_ruta)
			{
				pos=getAgentPosition(PosNodos,ratio,false);
				pos_x=pos(0);
				pos_y=pos(1);
				sprite.setScale(scale*s, scale*s);
			}
		
		else if(en_actividad)
			{
				int nodo_pos=Nodo_in_route();
				arma::vec nodo_actual=getNodePosition(PosNodos,nodo_pos);
				pos_x=nodo_actual(0);
				pos_y=nodo_actual(1);
				sprite.setScale(scale*s, scale*s);		
			}
		else	
			{ 
				std::cout<<"Error en draw, no tiene actividad definida "<<en_ruta<<" "<<en_actividad<<"\n";
				if(en_actividad){std::cout<<"En actividad";};			
			}



						
		sprite.setPosition(sf::Vector2f(pos_y+50*s,pos_x+50*s));
		window.draw(sprite);
	}



void Agente_Universitario::asignar_ruta(int simulationTime, int nodo_i,int nodo_f) 
	{
		
		// Check if it's the first assignment
		bool isFirstAssignment = (simulationTime == spawn_time);
		// Actividad = 0 -> Irse /// Actividad = 1 -> actv academica /// Actividad = 2 -> ocio o comer
		// Check if the activity is leisure
		bool isLeisureActivity = (actividad == 2);


			// Assign a route based on different conditions
			if (rol == 1 || rol == 2)//si es profesor o admin solo recibe ruta si es la inicializacion o si es actividad de ocio
				{ 
					if (isLeisureActivity) 
						{	
							// std::cout<<"Actividad ocio "<<actividad <<std::endl; 
							// Calculate the route using ruta_imagen function
							Ruta = Ruta_imagen(nodo_i,nodo_f,Usables,Mapa);
							en_ruta=true;
							en_actividad=false;
						}
					else
						{   
							// std::cout<<"entra en el else"<<std::endl;
							en_ruta=false; //Profesores y administrativos solo cambian de locacion para comer/ocio
							en_actividad=true;
						}
				}
			else 
				{ //los estudiantes siempre cambian de locacion
					Ruta = Ruta_imagen(nodo_i,nodo_f,Usables,Mapa);
					en_ruta=true;
					en_actividad=false;
				}
		
	}


void Agente_Universitario::hacer_actividad(double t,double dt,int nodo_i,int nodo_f,double prob_tipo_actividad)
	{
		tactividad+=dt;
		double tmax_actividad = 50;
		double trestante =  tmax_actividad-tactividad;
		nodo_i=Nodo_in_route();

		if(trestante<=0)
			{
				en_actividad=false;
				tactividad=0;
				Actividad(prob_tipo_actividad,t);

				if(actividad !=0)
					{
						en_ruta=true;
						en_actividad=false;
						asignar_ruta(t,nodo_i, nodo_f);

					}
				if(actividad==0)
					{

					}
				

			} 
		setBasuraActual(2.0);

	}


float Agente_Universitario::getBasuraActual()
	{return this->basura_actual;}

bool Agente_Universitario::setBasuraActual(float basura_actual)
	{

		float auxBasuraActual=this->basura_actual+this->delta_produccion_basura;

		if(basura_actual<=1)// Se le puede colocar un valor directo a la basura actual
			{this->basura_actual = basura_actual;}
		else if(auxBasuraActual<=this->capacidad_basura)
			{
				// Cuando el argumenro basura actual es mayor a uno, se hace ac
				// actualización por incremento.
				// Solo se actualiza cuando no se supera la capacidad de basura
				this->basura_actual=auxBasuraActual;
			}
		else
			{return false;}

		return true;
	}