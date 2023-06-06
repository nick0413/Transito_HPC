#define _USE_MATH_DEFINES 
#include <iostream>
#include <cmath>
#include <fstream>
#include <exception>
#include <memory>
#include <vector>
#include <random>
#include <armadillo>
using namespace std;


class Agente_Universitario; 

class Agente_Universitario{

    private:
        int rol;
        int actividad;
        int facultad;
        int spawn_time;
        float capacidad_basura;
        float tiempo_actividad;
        bool en_actividad;
        bool en_ruta;
        arma:vec Ruta;
        arma:vec posicion(2);
        arma:vec velocidad(2);
    
    public: 
        void Inicializar(double rand_rol_un,  double prob_tipo_actividad, double prob_actv_academica, int t_spawn,  float cap_basura, float t_actividad,
        bool en_actividad0, bool en_ruta0, arma:vec Ruta0, arma:vec posicion0, arma:vec velocidad0){
            rol = asignar_rol(rand_rol_un);
            actividad(prob_tipo_actividad, prob_actv_academica)
            spawn_time = t_spawn;
            capacidad_basura = cap_basura;
            tiempo_actividad = t_actividad;
            en_actividad = bool(actividad);
            en_ruta = en_ruta0;
            Ruta = Ruta0;
            posicion = posicion0;
            velocidad = velocidad0;
        }

        void asignar_rol(double prob_rol){
            
            if(prob_rol<0.91){
                rol = 0; //Es estudiante
                }

                else if(0.91< prob_rol < 0.95){
                    rol = 1; //Es administrativo
                }
            
                else{
                    rol = 2; //Es docente
                }   
            }

        void actividad(double prob_tipo_actividad, double prob_actv_academica){
            if(rol==0){//Si es estudiante
                auto result = prob_actividad_est(spawn_time,t);
                P_a = std::get<0>(result);
                P_o = std::get<1>(result);
                P_i= std::get<2>(result);

                
                if(t==spawn_time){
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
                   if(0<prob_actv_academica<=0.24){
                    facultad = 2;
                   }

                   else if(0.24<prob_actv_academica<=0.38){
                    facultad = 3;
                   }

                   else if(0.38<prob_actv_academica<=0.52){
                    facultad = 4;
                   }

                   else if(0.52<prob_actv_academica<=0.62){
                    facultad = 5;
                   }

                   else if(0.62<prob_actv_academica<=0.72){
                    facultad = 6;
                   }

                   else if(0.72<prob_actv_academica<=0.81){
                    facultad = 7;
                   }

                   else if(0.81<prob_actv_academica<=0.88){
                    facultad = 8;
                   }

                   else if(0.88<prob_actv_academica<=0.92){
                    facultad = 9;
                   }

                   else if(0.92<prob_actv_academica<=0.95){
                    facultad = 10;
                   }

                   else if(0.95<prob_actv_academica<=0.98){
                    facultad = 11;
                   }

                   else{facultad = 12;}
                    
                }
                if(0<prob_tipo_actividad<=P_a){
                    actividad = 1 //actividad academica
                }
                else if(P_a<prob_tipo_actividad<=(P_a+P_c)){
                    actividad = 2; //ocio o comer
                }

                else{
                    actividad=0; //irse de la universidad
                }
            }

            if(rol==1){
                auto result = prob_actividad_admin(t);
                P_a = std::get<0>(result);
                P_o = std::get<1>(result);
                P_i= std::get<2>(result);
                if(t==spawn_time){
                    //Asignacion facultad
                    /*
                        2 ing
                        3 humanas
                        4 ciencias
                        5 Medicina
                        6 economicas
                        7 artes
                        8 derecho
                        9 veterinaria
                        10 agrarias 
                        11 enfermeria
                        12 odontologia
                    
                    */
                   if(0<prob_actv_academica<=0.24){
                    facultad = 2;
                   }

                   else if(0.24<prob_actv_academica<=0.38){
                    facultad = 3;
                   }

                   else if(0.38<prob_actv_academica<=0.52){
                    facultad = 4;
                   }

                   else if(0.52<prob_actv_academica<=0.62){
                    facultad = 5;
                   }

                   else if(0.62<prob_actv_academica<=0.72){
                    facultad = 6;
                   }

                   else if(0.72<prob_actv_academica<=0.81){
                    facultad = 7;
                   }

                   else if(0.81<prob_actv_academica<=0.88){
                    facultad = 8;
                   }

                   else if(0.88<prob_actv_academica<=0.92){
                    facultad = 9;
                   }

                   else if(0.92<prob_actv_academica<=0.95){
                    facultad = 10;
                   }

                   else if(0.95<prob_actv_academica<=0.98){
                    facultad = 11;
                   }

                   else{facultad = 12;}
                    
                }
            
                if(0<prob_tipo_actividad<=P_a){
                    actividad = 1 //actividad academica
                }
                
                else if(P_a<prob_tipo_actividad<=(P_a+P_c)){
                    actividad = 2; //ocio o comer
                }

                else{
                    actividad=0; //irse de la universidad
                }
            }

            if(rol==2){
                auto result = prob_actividad_admin(t);
                P_a = std::get<0>(result);
                P_o = std::get<1>(result);
                P_i= std::get<2>(result);
                if(t==spawn_time){
                    //Asignacion facultad
                    /*
                        2 ing
                        3 humanas
                        4 ciencias
                        5 Medicina
                        6 economicas
                        7 artes
                        8 derecho
                        9 veterinaria
                        10 agrarias 
                        11 enfermeria
                        12 odontologia
                    
                    */
                   if(0<prob_actv_academica<=0.24){
                    facultad = 2;
                   }

                   else if(0.24<prob_actv_academica<=0.38){
                    facultad = 3;
                   }

                   else if(0.38<prob_actv_academica<=0.52){
                    facultad = 4;
                   }

                   else if(0.52<prob_actv_academica<=0.62){
                    facultad = 5;
                   }

                   else if(0.62<prob_actv_academica<=0.72){
                    facultad = 6;
                   }

                   else if(0.72<prob_actv_academica<=0.81){
                    facultad = 7;
                   }

                   else if(0.81<prob_actv_academica<=0.88){
                    facultad = 8;
                   }

                   else if(0.88<prob_actv_academica<=0.92){
                    facultad = 9;
                   }

                   else if(0.92<prob_actv_academica<=0.95){
                    facultad = 10;
                   }

                   else if(0.95<prob_actv_academica<=0.98){
                    facultad = 11;
                   }

                   else{facultad = 12;}
                    
                }
            
                if(0<prob_tipo_actividad<=P_a){
                    actividad = 1 //actividad academica
                }
                
                else if(P_a<prob_tipo_actividad<=(P_a+P_c)){
                    actividad = 2; //ocio o comer
                }

                else{
                    actividad=0; //irse de la universidad
                }
            }


        }

        std::tuple<double, double, double> prob_actividad_est(double spawn_time, double t){
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

        std::tuple<double, double, double> prob_actividad_admin(double t){
            double inicio_dia = 7*3600;
            double inicio_descanso = 12*3600; 

            double final_dia = 17*3600;
            double P_a;
            double P_o;
            double P_i;
            if(inicio_dia<t<=inicio_descanso){
                P_a = 0.8;
                P_o = 0.2;
                P_i = 0.0;
                return std::make_tuple(P_a, P_o, P_i);
            }

            else if(inicio_descanso<t<=final_descanso){
                double 
                double a =  exp(-pow((x - d) / (2.0 * pow(s, 2.0)), 2.0));
                double o = 0.5* exp(-pow((x - d) / (2.0 * pow(s, 2.0)), 2.0));
                double i = 0; 
                double tot = a+o+i;
                P_a = a/tot;
                P_o = o/tot;
                P_i = i/tot;
                return std::make_tuple(P_a, P_o, P_i);               

            }

            else if(final_descanso<t<=final_dia){
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
}