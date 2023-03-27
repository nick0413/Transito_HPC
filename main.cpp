#include <iostream>
#include <vector>
#include "Nodo.h"
#include "Camion.h"
#include <eigen/Eigen/Dense>
#include <fstream>
#include <armadillo/include/armadillo>

using namespace std;
using namespace Eigen;


template <typename M>
M load_csv_arma (const std::string & path) {
    arma::mat X;
    X.load(path, arma::csv_ascii);
    return Eigen::Map<const M>(X.memptr(), X.n_rows, X.n_cols);
}


void print_block(const MatrixXd& x)
{
	int m=sqrt(x.size());
  	for(int ii=0;ii<m;ii++)
		{for(int jj=0;jj<m;jj++)
			{
				cout<<x.coeff(ii,jj)<<" ";
			}
			cout<<"\n";

		}
}
int main(){


	MatrixXd A = load_csv_arma<MatrixXd>("Mapa.csv");
    return 0;
}