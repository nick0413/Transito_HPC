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
	// M.print("Matrix:");
	for(int ii=0; ii<nn;ii++)
		{for(int jj=0;jj<nn;jj++)
			{	
				if (ii+n==jj || ii-n==jj)
					{M(ii,jj)=1;}
				
				if (ii+1==jj || ii-1==jj)
					{M(ii,jj)=1;}

				if(ii==jj)
					{
						if(ii!=nn-1)
							{	
								if(ii%n==0 && ii!=0)
									{	
										M(ii-1,jj)=0;
										M(ii,jj-1)=0;
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
		if(numRows!=numCols) 
			{
				std::cout<<"Alerta: imagen no es cuadrada\n";
				return Adyacencia;
			}
		if((numRows*numCols)!=Adyacencia.n_rows)
			{
				std::cout<<"Alerta: imagen no es del tamaño apropiado nxn\n";
				return Adyacencia;
			}

		for(int ii=0; ii<numRows;ii++)
			{for(int jj=0;jj<numCols;jj++)
				{	
					std::cout<<imagen(ii,jj)<<"\n";
					if(imagen(ii,jj)==0)
						{
							element=ii*numRows+jj;
							Adyacencia.row(element).fill(0);
							Adyacencia.col(element).fill(0);
							Usables(element)=0;
						}
					else
						{Usables(element)=1;};
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
		arma::mat imagen= load_csv_arma("./Environment/Aceso.csv");
		arma::mat Matriz = matriz_nn(n);
		arma::mat Matriz2= Aceso(imagen,Matriz);
		save_matrix(Matriz2);
		salida.close();
		usab.close();


		return 0;
	}


