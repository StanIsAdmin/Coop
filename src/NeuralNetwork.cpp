#include "NeuralNetwork.hpp"

/**--------------------CONSTRUCTORS**/

/*Static members*/
std::random_device NeuralNetwork::seed;
std::default_random_engine NeuralNetwork::generator = std::default_random_engine(NeuralNetwork::seed());
std::uniform_int_distribution<int> NeuralNetwork::distribution_bool = std::uniform_int_distribution<int>(0,1);

/*Default constructor*/
NeuralNetwork::NeuralNetwork():
	defaultChoice(distribution_bool(generator)) //random bool
	{std::cout << defaultChoice << std::endl;}

/*Copy constructor*/
NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn):
	defaultChoice(nn.defaultChoice)
{
	for (int i=0; i<MAXNODES; ++i) {
		contextNodes[i] = nn.contextNodes[i];
	}
}


/**--------------------METHODS & OPERATORS**/

bool NeuralNetwork::decide(payoff self, payoff other)
{
	return self > other;
}

bool NeuralNetwork::decide()
{
	return defaultChoice;
}
