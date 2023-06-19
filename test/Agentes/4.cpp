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