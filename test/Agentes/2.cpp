#include <iostream>
#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include "Dijkstra.h"
std::ofstream salida; 

arma::mat fill(int n)
	{
		arma::mat M(n,n);

		for(int i=0;i<n;i++)
			{for(int j=0;j<n;j++)
				{
					M(i,j)=3;
				}

			}
		return M;
	}

int main()
	{
		int n[]={10,20,50,80,100,120,150,180,200,1000,2000,5000,10000,40000};
		int runs=sizeof(n)/sizeof(n[0]);
		std::chrono::high_resolution_clock::time_point startTime, endTime;
		salida.open("tiempo_fill_arma.txt");
		for(int ii=0;ii<runs;ii++)
			{	
				startTime=std::chrono::high_resolution_clock::now();
				arma::mat Matriz=fill(n[ii]);
				endTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
				salida << n[ii]<<"\t" << duration.count() << " seconds" << std::endl;
			}
		salida.close();
		return 0;
	}