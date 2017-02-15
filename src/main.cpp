#include "NeuralNetwork.hpp"

#include <iostream>


int main()
{
    std::cout << "Test:" << std::endl;
	
	NeuralNetwork nn1;
	std::cout << "Node count: " << nn1.getInnerNodeCount() << std::endl;
	if (nn1.decide())
		std::cout << "Collaborates!" << std::endl;
	else
		std::cout << "Defects!" << std::endl;
	
	if (nn1.decide(2,3))
		std::cout << "Collaborates!" << std::endl;
	else
		std::cout << "Defects!" << std::endl;

    return 0;
}
