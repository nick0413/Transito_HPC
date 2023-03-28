#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

int main()
{
    // Create a matrix with 3 rows and 4 columns
    mat A = randu<mat>(3, 4);
    
    // Extract the second row of the matrix as a vector
    rowvec row_vec = A.row(1);
    
    // Print the original matrix and the extracted row vector
    cout << "Original matrix:\n" << A << endl;
    cout << "Extracted row vector:\n" << row_vec << endl;
    
    return 0;
}