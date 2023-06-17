#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>
#include "Dijkstra.h"

std::ofstream salida;

std::ofstream usab;

void save_vector(arma::ivec vector)
	{
		int size=vector.size();
		for(int ii=0;ii<size;ii++)
			{
				if(ii!=0)
					{usab<<"\n";}
				usab<<vector(ii);
				
			}
	}

arma::mat matriz_nn(int n){
	int nn=n*n;
	arma::mat M;

	M.zeros(n*n, n*n);

	for(int ii=0; ii<nn;ii++)
		{for(int jj=0;jj<nn;jj++)
			{	


				// if(ii==)	
				// 	{
				// 		std::cout<<ii<<" "<<jj<<"\n";
				// 		M(ii,jj)=1;
				// 	}(ii-n==jj-1 &&(jj%n)!=0)





				if((ii-n==jj-1 && (jj%n)!=0))
					{	
						if(ii==20 || jj==20){std::cout<<ii<<" "<<jj<<"\t1\n";}
						M(ii,jj)=1;
					};
				if((jj-n==ii-1 && (ii%n)!=0))
					{	
						if(ii==20 || jj==20){std::cout<<ii<<" "<<jj<<"\t2\n";}
						// std::cout<<ii<<" "<<jj<<"\n";
						M(ii,jj)=1;
					};


				if((ii+n==jj-1 && (jj%n)!=0))
					{	
						if(ii==20 || jj==20){std::cout<<ii<<" "<<jj<<"\t3\n";}
						// std::cout<<ii<<" "<<jj<<"\n";
						M(ii,jj)=1;
					};

				if((jj+n==ii-1 && (ii%n)!=0))
					{	
						if(ii==20 || jj==20){std::cout<<ii<<" "<<jj<<"\t4\n";}
						// std::cout<<ii<<" "<<jj<<"\n";
						M(ii,jj)=1;
					};


				if (ii+n==jj || ii-n==jj)
					{M(ii,jj)=1;}
				
				else if (ii+1==jj || ii-1==jj)
					{M(ii,jj)=1;}
				

				else if(ii==jj)
					{
						if(ii!=nn-1)
							{	
								if(ii%n==0 && ii!=0)
									{	
										// std::cout<<ii<<" "<<jj<<"\n";
										M(ii-1,jj)=9999;
										M(ii,jj-1)=9999;
									}
								
							}
					}
			}
		}
	// M.print("Matrix:");

	return M;
}

arma::mat Aceso(arma::mat imagen, arma::mat Adyacencia)
	{
		int numRows = imagen.n_rows;
		int numCols= imagen.n_cols;
		int element=0;
		arma::ivec Usables(Adyacencia.n_rows);
		Usables.fill(3);
		if(numRows!=numCols) 
			{
				std::cout<<"Alerta: imagen no es cuadrada\n";
				return Adyacencia;
			}
		// std::cout<<"Alerta: imagen no es del tamaño apropiado nxn "<<numRows*numCols<<" "<<Adyacencia.n_rows << "\n";
		if((numRows*numCols)!=Adyacencia.n_rows)
			{
				std::cout<<"Alerta: imagen no es del tamaño apropiado nxn "<<numRows*numCols<<" "<<Adyacencia.n_rows << "\n";
				return Adyacencia;
			}
		//std::cout<<0<<"\t"<<4<<"\t"<<Adyacencia(0,4) <<"----\n";
		for(int ii=0; ii<numRows;ii++)
			{for(int jj=0;jj<numCols;jj++)
				{	
					// std::cout<<imagen(ii,jj)<<"\n";
					element=ii*numRows+jj;
					if(imagen(ii,jj)==0)
						{
							
							
							Adyacencia.row(element).fill(9999);
							Adyacencia.col(element).fill(9999);


							Usables(element)=0;
							// std::cout<<element<<"\t"<<ii<<"\t"<<jj<< "\t" <<Usables(element) <<"\n";
						}
					else if(imagen(ii,jj)!=0)
						{Usables(element)=1;}
					else
						{Usables(element)=2;
						std::cout<<"Alerta: Error en asigancion de Madyacencia\n";}
				}
			}


		// std::cout<<0<<"\t"<<4<<"\t"<<Adyacencia(0,4) <<"----\n";
		for(int ll=0;ll<Adyacencia.n_cols;ll++)
			{for(int tt=0; tt<Adyacencia.n_rows;tt++)
				{
				if(Adyacencia(ll,tt)==0)
					{Adyacencia(ll,tt)=9999;}
				}
			}
		save_vector(Usables);
		return Adyacencia;

	}



void save_matrix(arma::mat Matrix)
	{
		int numRows = Matrix.n_rows;
		int numCols = Matrix.n_cols;

		for(int ii=0; ii<numRows;ii++)
			{for(int jj=0;jj<numCols;jj++)
				{
					salida<< Matrix(ii,jj);
					if (jj!=numCols-1)
						{salida<<",";}
					else
						{salida<<"\n";}
				}
			}
		
	}


int main()
	{	
		int n=10;
		salida.open("./Environment/Matriz_adyacencia_mapa.csv");
		usab.open("./Environment/Usables.csv");
		arma::mat imagen= load_csv_arma("./Environment/Acceso.csv");
		arma::mat Matriz = matriz_nn(n);
		// std::cout<<0<<"\t"<<4<<"\t"<<Matriz(0,4) <<"\n";
		arma::mat Matriz2= Aceso(imagen,Matriz);
		save_matrix(Matriz2);
		salida.close();
		usab.close();


		return 0;
	}


