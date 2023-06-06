#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>

std::ofstream salida;




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
		int n=100;
		int nn=n*n;
		salida.open("Matriz_adyacencia_mapa.csv");
		arma::mat Matriz = matriz_nn(n);
		save_matrix(Matriz);
		salida.close();


		return 0;
	}


