#include "NeuralNetwork.hpp"

#include <iostream>
#include <cassert>


int main()
{
	NeuralNetwork nn1;
	int cogNodes = nn1.getCognitiveNodeCount();
	int conNodes = nn1.getContextNodeCount();
	int innNodes = nn1.getInnerNodeCount();
	
	assert(cogNodes + conNodes == innNodes);
	assert(innNodes <= MAXINITIALNODES);
	assert(conNodes <= cogNodes);
	
	
	if (nn1())
		std::cout << "Collaborates!" << std::endl;
	else
		std::cout << "Defects!" << std::endl;
	
	if (nn1(2,3))
		std::cout << "Collaborates!" << std::endl;
	else
		std::cout << "Defects!" << std::endl;

    return 0;
}
