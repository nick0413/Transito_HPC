#include <armadillo>
using namespace std;

arma::vec append_vec(arma::vec & vector, double value)
	{
		vector.resize(vector.n_elem+1);
		vector(vector.n_elem-1)=value;
		return vector;
	}


int main()
{
    // Create a vector of size 3


    return 0;
}