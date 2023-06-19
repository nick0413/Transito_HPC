#include <iostream>
#include <vector>
#include <armadillo>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <cstdio>
#include "Dijkstra.h"


arma::mat rd_fill(arma::mat Matriz,float val)
	{
		std::mt19937 gen(50);
		std::uniform_real_distribution<double> real_dist(0.0,1.0);
		int cols=Matriz.n_cols;
		int filas=Matriz.n_rows;
		float prob=0;
		for(int ii=0;ii<filas;ii++)
			{for(int jj=0;jj<cols;jj++)
				{	
					prob=real_dist(gen);
					std::cout<<prob<<"\n";
					if(prob<0.2)
						{Matriz(ii,jj)=val;}
				}

			}
		return Matriz;
	}


// int main()
// 	{	

// 		arma::mat M(100,100);

// 		M=rd_fill(M,1);

// 		M.print();
// 		arma::sp_mat M2(M);

// 		M2.print();


// 		return 0;
// 	}


// int main() {
//     double data[] = {1.0, 2.0, 3.0, 4.0, 5.0};  // 1D array

//     arma::mat matrix(data, 1, 5);  // Construct a 1x5 matrix from the 1D array

//     std::cout << "Matrix:\n" << matrix << std::endl;

//     return 0;
// }

// int main()
// {
//     std::vector<float> vec = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
//     int rows = 2;
//     int cols = 3;

//     // Convert std::vector<float> to arma::mat
//     arma::mat armaMat = arma::mat(vec.data(), rows, cols, false);

//     // Print the resulting Armadillo matrix
//     armaMat.print();

//     return 0;
// }

#include <iostream>
#include <vector>
#include <armadillo>

int main() {
    std::vector<double> vec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};  // Example vector
    int rows = 2;  // Number of rows
    int cols = 3;  // Number of columns

    arma::mat matrix;
    matrix = arma::conv_to<arma::mat>::from(vec);  // Convert vector to matrix
    matrix.reshape(rows, cols);  // Reshape the matrix to the specified dimensions

    // Print the resulting matrix
    std::cout << "Matrix:\n" << matrix << std::endl;

    return 0;
}