#include <armadillo>
#include <iostream>

using namespace std;

int main(void){
    arma::mat A = arma::mat("0.0 0.1 0.2 ; 1.0 1.1 1.2 ; 2.0 2.1 2.2");
    arma::vec x = arma::vec("20. 10. 30.");
    cout<<A<<endl;
    cout<<x<<endl;

    arma::uvec x_sort_indices = arma::sort_index(x); // Get index ordering that sorts x.
    cout<<x<<endl;
    
    x = x(x_sort_indices); // Sort x according to ordering in x_sort_indices.
    A = A.rows(x_sort_indices); // Sort columns of A according to ordering in x_sort_indices.

    cout<<A<<endl;
    return 0;
}