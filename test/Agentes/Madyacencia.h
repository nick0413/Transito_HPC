#include <iostream>
#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>
#include "Dijkstra.h"

std::ofstream salida;
std::ofstream usab;

//void fill_row(std::vector<float>& mat, int cols, int fila, float val)
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

void save_vector(std::vector<int> & vec, int size)
	{
		for(int ii=0;ii<size;ii++)
			{
				if(ii!=0)
					{usab<<"\n";}
				usab<<vec[ii];	
			}
	}

void save_vector(std::vector<float> & vec, int size)
	{
		for(int ii=0;ii<size;ii++)
			{
				if(ii!=0)
					{usab<<"\n";}
				usab<<vec[ii];	
			}
	}


void fill_vec(std::vector<float>& vec, int size, float val)
	{
		for (int ii = 0; ii < size; ii++)
			{vec[ii] = val;}
	}
void fill_vec(std::vector<int>& vec, int size, float val)
	{
		for (int ii = 0; ii < size; ii++)
			{vec[ii] = val;}
	}

void fill_row(std::vector<float>& mat, int cols, int fila, float val)
	{
		int columnas = cols;
		int indice = 0;
		for (int columna = 0; columna < columnas; columna++)
			{
				indice = fila * columnas + columna;
				mat[indice] = val;
			}
	}
void fill_col(std::vector<float>& mat, int filas,int col,float val)
	{
		int indice=0;

		for(int fila=0;fila<filas;fila++)
			{
				indice=fila*filas+col;
				mat[indice]=val;
			}

	}
void fill_zero(std::vector<float>& mat,int elementos,bool verbose=false)
	{
		if(verbose)std::cout<<"elementos fill_zero: " <<elementos<<"\n";

		for(int ii=0;ii<elementos;ii++)
			{
				mat[ii]=0;
				// std::cout<<mat[ii]<<" "<<ii<<"\n";
			}

	}

void matriz_adyacencia(std::vector<float>& mat, int filas,int n,bool progress=false)
	{	
		// ; n Es la cantidad de nodos de la red
		// ; filas es la cantidad de filas/columnas que tiene la matriz de adyacencia, es decir n^2.
		fill_zero(mat,filas*filas);
		int indice=0;
		int nn=n*n;

		for(int ii=0; ii<filas;ii++)
			{
				for(int jj=0;jj<filas;jj++)
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
												mat[(ii-1)*nn+jj]=0;
												mat[(ii)*nn+jj-1]=0;
											}
									}
							}					
					}
			}
	}
void Acceso(arma::mat imagen, std::vector<float>& Madyacencia, int n_filas,std::vector<int>& Usables,bool progress=false)
	{
		// El vector usables debe ser de dimension n_filas
		int numRows=imagen.n_rows;
		int numCols=imagen.n_cols;		
		int element=0;
		int n_cols=n_filas; 
		fill_vec(Usables,n_filas,3);

		if(numCols!=numRows)
			{throw std::logic_error("Alerta: imagen no es cuadrada\n");}

		if((numRows*numCols)!=n_filas)
			{throw std::logic_error("Alerta: imagen no es de la dimension apropiada nxn "+std::to_string(numRows*numCols)+" "+std::to_string(n_filas));}

		for(int ii=0;ii<numRows;ii++)
			{
				if(progress){displayProgressBar(float(ii)/float(numRows));}
				for(int jj=0;jj<numCols;jj++)
					{
						element=ii*numRows+jj;
						if(imagen(ii,jj)==0)
							{
								
								// fill_row(Madyacencia,element,n_filas,9999);
								fill_row(Madyacencia,n_cols,element,0);
								fill_col(Madyacencia,n_filas,element,0);
								Usables[element]=0;

							}
						else if(imagen(ii,jj)!=0)
							{Usables[element]=1;}
						else
							{Usables[element]=2;
							throw std::logic_error("Alerta: Error en asigancion de Madyacencia\n");}

						// std::cout<<imagen(ii,jj)<<"\t||"<<ii<<"\t"<<jj<<"\t"<<element<<"\t"<<Usables[element]<<"\n";
					}
			}


		int indice=0;
		for(int ll=0; ll<n_cols;ll++)
			{for(int tt=0; tt<n_filas;tt++)
				{	
					indice=ll*n_filas+tt;
					if(Madyacencia[indice]==0)
						{Madyacencia[indice]=0;}
				}
			}

		save_vector(Usables, n_filas);
		
	}

void save_matrix(std::vector<float>& mat,int n_filas,bool progress = false)
	{
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

		salida.open("./Environment/Matriz_adyacencia_mapa.csv");
		usab.open("./Environment/Usables.csv");
		arma::mat imagen= load_csv_arma("./Environment/Acceso.csv");

		int n=imagen.n_cols;
		int nn=n*n;	
		std::cout<<nn*nn<<"\n";
		std::vector<float> Madyacencia(nn*nn);	
		std::vector<int> Usables(nn);
		
		matriz_adyacencia(Madyacencia,nn,n);
		Acceso(imagen,Madyacencia,nn,Usables);
		// arma::mat armaMat = arma::mat(Madyacencia.data(), nn, nn, false);

		arma::mat Ma_arma;
		Ma_arma = arma::conv_to<arma::mat>::from(Madyacencia);
		Ma_arma.reshape(nn,nn);
		arma::sp_mat Ma_sp(Ma_arma);
		Ma_sp.print();
		
	

		return 0;
	}	