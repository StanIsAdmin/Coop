#include "NeuralNetwork.hpp"

#include <random>

/**--------------------CONSTRUCTORS**/
	
NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn)
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
