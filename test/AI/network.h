#include <iostream>
#include <armadillo>
#include <vector>
#include <utility>
#include <tuple>
#include <memory>
#include <sstream>
#include <list>
#include <random>
#include <cmath>
#include <string>

using namespace std;

double epsilon =0.01;

class IA
	{
		public:
			IA(const tuple<int,int> &, const tuple<int,int>, const string&, const double&, const double &);


		private:
			shared_ptr<arma::Mat<double>> layer_inputs;
			shared_ptr<arma::Mat<double>> layer_weights;
			shared_ptr<arma::Mat<double>> layer_activations;
			shared_ptr<arma::Mat<double>> layer_sigmas;
			shared_ptr<arma::Mat<double>> layer_gradients;

			arma::Mat<double> output;
			double lambda;
			double alpha;
			string activation;
	};


IA::IA(const tuple<int,int> & input_size, const tuple<int,int> output_size, const string& activation="relu", const double& lambda=0.5, const double & alpha)
	{
		
	}