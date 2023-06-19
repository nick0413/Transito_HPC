#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>
#include "Dijkstra.h"



std::ofstream salida;

std::ofstream usab;
void displayProgressBar(float progress, int barWidth = 50, std::string line="=")
	{
		std::cout << "[";
		int pos = static_cast<int>(progress * barWidth);
		for (int i = 0; i < barWidth; ++i) 
			{
				if (i < pos)
					{std::cout << line;}
				else if (i == pos)
					{std::cout << ">";}
				else
					{std::cout << " ";}
			}
		std::cout << "] " << static_cast<int>(progress * 100.0) << "%\r";
		std::cout.flush();
	}


void load_matrix(float * matrix,std::string path)
	{
		arma::mat mat= load_csv_arma(path);

		int n_filas=mat.n_rows;
		int n_cols=mat.n_cols;
		int indice=0;
		
		if(n_filas*n_cols!=(sizeof(matrix)/sizeof(matrix[0])))
			{throw std::logic_error("Error: La imagen no es de la dimensiones especificadas\n"+std::to_string(sizeof(matrix)/sizeof(matrix[0]))+" "+std::to_string(n_filas*n_cols));}
		
		for(int ii=0;ii<n_filas;ii++)
			{for(int jj=0;jj<n_cols;jj++)
				{
					indice=ii*n_cols+jj;
					matrix[indice]=mat(ii,jj);
				}

			}
		
	}
void fill_row(float * mat,int fila, float val)
	{
		int elementos=sizeof(mat)/sizeof(mat[0]);
		int columnas=sqrt(elementos);
		int indice=0;
		for(int columna=0;columna<columnas;columna++)
			{
				indice=fila*columnas+columna;
				mat[indice]=val;
			}
	}

void fill_col(float * mat,int col, float val)
	{
		int elementos=sizeof(mat)/sizeof(mat[0]);
		int filas=sqrt(elementos);
		int indice=0;

		for(int fila=0;fila<filas;fila++)
			{
				indice=fila*filas+col;
				mat[indice]=val;
			}

	}

void fill_zero(float * mat)
	{
		int elementos=sizeof(mat)/sizeof(mat[0]);
		std::cout<<"elementos fill_zero: " <<elementos<<"\n";
		for(int ii=0;ii<elementos;ii++)
			{
				mat[ii]=0;
				// std::cout<<mat[ii]<<" "<<ii<<"\n";
			}

	}


void save_vector(double * vec)
	{
		int size = sizeof(vec) / sizeof(vec);

				for(int ii=0;ii<size;ii++)
			{
				if(ii!=0)
					{usab<<"\n";}
				usab<<vec[ii];	
			}
	}
void save_vector(int * vec)
	{
		int size = sizeof(vec) / sizeof(vec);

				for(int ii=0;ii<size;ii++)
			{
				if(ii!=0)
					{usab<<"\n";}
				usab<<vec[ii];	
			}
	}

void matrix_nn(float * mat ,int n,bool progress=false)
	{
		const int nn=n*n;


		fill_zero(mat);
		int indice=0;
		for(int ii=0; ii<nn;ii++)
			{
				for(int jj=0;jj<nn;jj++)
					{
						indice=ii*nn+jj;
						
						if((ii-n==jj-1 && (jj%n)!=0))
							{mat[indice]=1.41;};

						if((jj-n==ii-1 && (ii%n)!=0))
							{mat[indice]=1.41;}


						if((ii+n==jj-1 && (jj%n)!=0))
							{mat[indice]=1.41;}
						
						if((jj+n==ii-1 && (ii%n)!=0))
							{mat[indice]=1.41;}


						if (ii+n==jj || ii-n==jj)
							{mat[indice]=1;}

						else if (ii+1==jj || ii-1==jj)
							{mat[indice]=1;}

						else if(ii==jj)
							{
								if(ii!=nn-1)
									{
										if(ii%n==0 && ii!=0)
											{
												mat[(ii-1)*nn+jj]=9999;
												mat[(ii)*nn+jj-1]=9999;
											}
									}
							}
					}
			}


	}

void Acceso(arma::mat imagen, float * Madyacencia,bool progress=false)
	{
		int numRows=imagen.n_rows;
		int numCols=imagen.n_cols;
		int element=0;
		int n_elementos=sizeof(Madyacencia)/sizeof(Madyacencia[0]);
		int n_filas=sqrt(n_elementos);
		int n_cols=n_filas;
		int Usables[n_filas];
		
		if(numCols!=numRows)
			{throw std::logic_error("Alerta: imagen no es cuadrada\n");}

		if((numRows*numCols)!=n_filas)
			{throw std::logic_error("Alerta: imagen no es de la dimension apropiada nxn "
									+std::to_string(numRows*numCols)+" "+std::to_string(n_filas));}
		

		for(int ii=0;ii<numRows;ii++)
			{
				if(progress){displayProgressBar(float(ii)/float(numRows))  ;}
				for(int jj=0;jj<numCols;jj++)
					{
						element=ii*numRows+jj;
						if(imagen(ii,jj)==0)
							{
								fill_row(Madyacencia,element,9999);
								fill_col(Madyacencia,element,9999);

							}
						else if(imagen(ii,jj)!=0)
							{Usables[element]=1;}
						else
							{Usables[element]=2;
							throw std::logic_error("Alerta: Error en asigancion de Madyacencia\n");}
					}
			}
		int indice=0;
		for(int ll=0; ll<n_cols;ll++)
			{for(int tt=0; tt<n_filas;tt++)
				{	
					indice=ll*n_filas+tt;
					if(Madyacencia[indice]==0)
						{Madyacencia[indice]=9999;}
				}
			}
		save_vector(Usables);
		
		
	}

void save_matrix(float * mat,bool progress = false)
	{
		int n_elementos=sizeof(mat)/sizeof(mat[0]);
		int n_filas=sqrt(n_elementos);
		int n_cols=n_filas; 
		int indice=0;
		for(int ii=0; ii<n_filas;ii++)
			{	if(progress){displayProgressBar(float(ii)/float(n_filas))  ;}
				for(int jj=0;jj<n_cols;jj++)
				{	
					indice=ii*n_filas+jj;
					salida<< mat[indice];
					if (jj!=n_cols-1)
						{salida<<",";}
					else
						{salida<<"\n";}
				}
			}		

	}


int main()
	{
		int n=10;
		float matriz[n*n*n*n];
		salida.open("./Environment/Matriz_adyacencia_mapa.csv");
		usab.open("./Environment/Usables.csv");
		arma::mat imagen= load_csv_arma("./Environment/Acceso.csv");
		matrix_nn(matriz,n);
		std::cout<<sizeof(matriz)/sizeof(matriz[0])<<"\n";
		Acceso(imagen,matriz);
		save_matrix(matriz);
		salida.close();
		usab.close();
		
		return 0;
	}
