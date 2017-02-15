#include "NeuralNetwork.hpp"

#include <iostream>
#include <cassert>


int main()
{
	std::cout << "Running tests..." << std::endl;
	
	///Construction & initial nodes
	NeuralNetwork nn;
	int cogNodes = nn.getCognitiveNodeCount();
	int conNodes = nn.getContextNodeCount();
	int innNodes = nn.getInnerNodeCount();
	
	assert(cogNodes + conNodes == innNodes);
	assert(innNodes <= MAXINITIALNODES);
	assert(conNodes <= cogNodes);

	///Node addition and removal
	for (int i=0; i<MAXNODES*2-innNodes; ++i) {
		nn.addNode();
	}
	
	assert(nn.getCognitiveNodeCount() == MAXNODES);
	assert(nn.getContextNodeCount() == MAXNODES);
	assert(nn.getInnerNodeCount() == 2*MAXNODES);
	
	for (int i=0; i<19; ++i) {
		nn.removeNode();
	}
	
	assert(nn.getCognitiveNodeCount() == 0);
	assert(nn.getContextNodeCount() == 0);
	
	///Randomness of collaborate/defect
	int defaultCollab = 0;
	int otherCollab = 0;
	for (int i=0; i<1000; ++i) {
		NeuralNetwork nni;
		if (nni())
			defaultCollab++;
		
		if (nni(1,1))
			otherCollab++;
	}
	//There's one chance in a million that 1000 coin tosses result in 575 or more heads/tails
	assert(defaultCollab < 575); 
	assert(otherCollab < 575);
	
	std::cout << "All tests passed!" << std::endl;

    return 0;
}
