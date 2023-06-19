#include <iostream>
#include <armadillo>

int main() {
    // Create an Armadillo matrix
    arma::mat matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    // Define the values 'a' and 'b'
    double a = 5.0;
    double b = 10.0;

    // Replace 'a' with 'b'
    matrix.transform([a, b](double val) { return (val == a) ? b : val; });

    // Display the modified matrix
    std::cout << matrix << std::endl;

    return 0;
}
