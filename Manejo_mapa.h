#include <iostream>
#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include "Dijkstra.h"

std::ofstream salida;
std::ofstream usab;

//void fill_row(std::vector<float>& mat, int cols, int fila, float val)
std::string to_string_with_one_decimal(float value)
{
    // Convert the float to a string using std::to_string
    std::string str = std::to_string(value);

    // Find the position of the decimal point
    size_t decimalPos = str.find('.');

    if (decimalPos != std::string::npos)
    {
        // Check if there is more than one decimal digit
        if (str.size() - decimalPos > 2)
        {
            // Erase all digits after the first decimal digit
            str.erase(decimalPos + 2);
        }
    }

    return str;
}
float secondsToHours(float seconds) {
    float hours = seconds / 3600.0;  // Divide seconds by the number of seconds in an hour (3600)
    return hours;
}

float hoursToSeconds(float hours) {
    float seconds = hours * 3600.0;  // Divide seconds by the number of seconds in an hour (3600)
    return seconds;
}


double gaussian(double x, double mean, double stddev) {
    double exponent = -0.5 * pow((x - mean) / stddev, 2);
    double coefficient = 1.0 / (stddev * sqrt(2 * M_PI));
    return coefficient * exp(exponent);
}
int next_node(arma::ivec Usables, int nodo)
	{  
		for( int ii=nodo; ii<Usables.size(); ii++)
			{
				if (Usables(ii)==1)
					{return ii;}
			}
		std::cout<< "No hay nodos accesibles"<<std::endl;
		return 0;
	}

arma::ivec Ruta_imagen(int start, int end, arma::ivec & Usables, arma::sp_mat & Mapa,bool verbose=false)
	{	

		// std::cout<<"Llama a Load_csv_arma---\n";

		// std::cout<<"termino los llamados---\n";
		// std::cout<<Usables.size()<<"\n";
		// std::cout<<Mapa.size()<<"\n";
		// std::cout<<"\n";
		if (start>Usables.size() || end>Usables.size())
			{
				std::cout<<start<<"->" <<end<<" Este es un mapa mas pequeño, cambiar start-end\n";
				return 0;
			}

		if(Usables(end)==1)
				{ if(verbose)std::cout<<"Si end\n";}
		else

			{
				if(verbose)std::cout<<end<<" No es un nodo accesible"; 
				end=next_node(Usables,end);
				if(verbose)std::cout<<", Se usara "<<end<<"\n"; 
			}
		if(Usables(start)==1)
			{if(verbose)std::cout<<"Si start\n";}
		else
			{
				if(verbose)std::cout<<start<<" No es un nodo accesible"; 
				start=next_node(Usables,start);
				if(verbose)std::cout<<", Se usara "<< start<<"\n"; 
			}
		
		// std::cout<<"dijkstra usa "<<start<<" "<<end<<"\n";
		arma:: ivec Ruta1=dijkstra_arma_sp(Mapa,start,end);

		//Ruta1.print();
		return Ruta1;
	}

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
		std::cout << "] " << static_cast<int>(progress * 100.0)<< "%\r";
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
		int indice_inverso=0;
		int nn=n*n;

		for(int ii=0; ii<filas;ii++)
			{
				for(int jj=0;jj<filas;jj++)
					{
						indice=ii*nn+jj;
						// indice_inverso=jj*nn+ii;
						
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
								// std::cout<<imagen(ii,jj)<<"\n";
								// if(element==2869)std::cout<<imagen(ii,jj)<<"????\n";
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

		// std::cout<<"Acceso\n";
		// print_connections(Ma_sp2,2868);


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

arma::mat load_imagen(int res)
	{
		arma::mat imagen= load_csv_arma("./Environment/Acceso/Acceso_"+std::to_string(res)+".csv");
		return imagen;
	}

// int main()
// 	{	

// 		std::chrono::high_resolution_clock::time_point startTime, endTime;
// 		startTime=std::chrono::high_resolution_clock::now();
// 		arma::mat imagen= load_csv_arma("./Environment/Acceso.csv");

// 		int n=imagen.n_cols;
// 		int nn=n*n;	
// 		std::cout<<nn*nn<<"\n";
// 		std::vector<float> Madyacencia(nn*nn);	
// 		std::vector<int> Usables(nn);
		
// 		matriz_adyacencia(Madyacencia,nn,n);
// 		Acceso(imagen,Madyacencia,nn,Usables);
// 		// arma::mat armaMat = arma::mat(Madyacencia.data(), nn, nn, false);

// 		arma::mat Ma_arma;
// 		arma::vec Usables_vec;
// 		Ma_arma = arma::conv_to<arma::mat>::from(Madyacencia);
// 		Usables_vec = arma::conv_to<arma::vec>::from(Usables);
// 		Ma_arma.reshape(nn,nn);
// 		arma::sp_mat Ma_sp(Ma_arma);
// 		// Ma_sp.print();
// 		endTime = std::chrono::high_resolution_clock::now();
// 		std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
// 		std::cout<< n<<"\t" << duration.count() << " seconds" << std::endl;

// 		return 0;
// 	}	