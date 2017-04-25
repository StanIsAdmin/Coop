#include "NeuralNetworkTest.hpp"

void testInnerNodes();
void testNetwork();

void testNeuralNetwork()
{
	std::cout << "Testing NeuralNetwork...";
	
	testInnerNodes();
	testNetwork();
	
	std::cout << " done!" << std::endl;
}	
	
void testInnerNodes()
{
	///InnerNode class construction
	numval input = RNG::getRandomNumval();
	numval threshold = RNG::getRandomNumval();
	InnerNode node(threshold);
	assert(node(input) == sigmoidalSquash(input, threshold));
	
	///InnerNode context addition
	numval context_value = RNG::getRandomNumval();
	numval link_weight = RNG::getRandomNumval();
	node.addContextNode(context_value, link_weight);
	assert(node(input) == sigmoidalSquash(input + (context_value * link_weight), threshold));
	
	///InnerNode copy
	InnerNode node2(node);
	assert(node2 == node);
	
	///InnerNode context removal
	node.removeContextNode();
	assert(node(input) == sigmoidalSquash(input, threshold));
	
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
}

void testNetwork()
{
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
	payoff input_self(7), input_other(6);
	int nn_cooperations(0), nn2_cooperations(0);
	
	for (int i=0; i<100; ++i) {
		if (nn(input_self, input_other)) nn_cooperations ++;
		if (nn2(input_self, input_other)) nn2_cooperations ++;
	}
	assert(abs(nn2_cooperations - nn_cooperations) < 55); //prob. of different results is weak
	
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
	
	int structure_mutations_count = 0, inner_node_count;
	for (int i=0; i<100; ++i) {
		inner_node_count = nn2.getInnerNodeCount();
		nn2.mutate();
		if (nn2.getInnerNodeCount() != inner_node_count) structure_mutations_count++;
	}
	assert(structure_mutations_count < 10);
	
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
}
