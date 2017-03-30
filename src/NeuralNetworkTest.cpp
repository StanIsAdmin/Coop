#include "NeuralNetworkTest.hpp"

void testNeuralNetwork()
{
	std::cout << "Running tests..." << std::endl;
	
	///InnerNode class construction
	InnerNode node(0.0);
	assert(node(0.0) == 0.5);
	
	///InnerNode copy
	InnerNode node2(node);
	assert(node2(0.0) == 0.5);
	
	///InnerNode context addition
	node.addContextNode(1.0, -3.0);
	assert(node(3.0) == 0.5);
	
	///InnerNode context removal
	node.removeContextNode();
	assert(node(0.0) == 0.5);
	
	///InnerNode squashing function
	numval max = std::numeric_limits<numval>::max();
	numval min = std::numeric_limits<numval>::min();
	numval low = std::numeric_limits<numval>::lowest();
	
	InnerNode node3(max);
	node3.addContextNode(max, max);
	numval result = node3(max);
	assert(result >= 0 and  result <= 1);
	
	node3.removeContextNode();
	node3.addContextNode(min, low);
	result = node3(max);
	assert(result >= 0 and  result <= 1);
	
	///Construction & initial nodes
	NeuralNetwork nn;
	int cogNodes = nn.getCognitiveNodeCount();
	int conNodes = nn.getContextNodeCount();
	int innNodes = nn.getInnerNodeCount();
	
	assert(cogNodes + conNodes == innNodes);
	assert(innNodes <= MAXINITIALNODES);
	assert(conNodes <= cogNodes);

	///Node addition
	for (int i=0; i< (MAXNODES*2) - innNodes; ++i) {
		nn.addNode();
		assert(nn.getInnerNodeCount() == innNodes + i + 1);
	}
	
	assert(nn.getCognitiveNodeCount() == MAXNODES);
	assert(nn.getContextNodeCount() == MAXNODES);
	assert(nn.getInnerNodeCount() == 2*MAXNODES);
	
	///Copy constructor
	NeuralNetwork nn2(nn);
	assert(nn == nn2);
	
	///Node removal
	innNodes = MAXNODES*2;
	for (int i=0; i<MAXNODES*2 and innNodes>0; ++i) {
		nn.removeNode();
		
		assert(innNodes == nn.getInnerNodeCount() + 1 or innNodes == nn.getInnerNodeCount() + 2);
		assert(nn.getCognitiveNodeCount() >= nn.getContextNodeCount());
		
		innNodes = nn.getInnerNodeCount();
	}
	assert(nn.getCognitiveNodeCount() == 0);
	assert(nn.getContextNodeCount() == 0);
	assert(nn.getInnerNodeCount() == 0);
	
	///Node mutation
	NeuralNetwork nn3(nn2);
	for (int i=0; i<20; ++i) {
		nn3.mutate();
		nn2.mutate();
	}
	assert(nn3 != nn2);
	
	///Randomness of cooperate/defect
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
}
