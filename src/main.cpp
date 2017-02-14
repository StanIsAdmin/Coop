#include "NeuralNetwork.hpp"

#include <iostream>


int main()
{
    std::cout << "Test:" << std::endl;
	
	NeuralNetwork nn1;
	if (nn1.decide(2,3))
		std::cout << "Collaborates!" << std::endl;
	else
		std::cout << "Defect!" << std::endl;

    return 0;
}
