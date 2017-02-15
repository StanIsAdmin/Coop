#include "NeuralNetwork.hpp"


numval sigmoidalSquash(numval value, numval threshold)
{
	return 1/(1+std::exp(-value-threshold));
}

/**--------------------InnerNode**/

/*Constructor*/
InnerNode::InnerNode(numval threshold):
	threshold_value(threshold)
	{}
	
void InnerNode::setContext(numval value, numval link_weight)
{
	has_context = true;
	context_value = value;
	context_link_weight = link_weight;
}

void InnerNode::removeContext()
{
	has_context = false;
}
	
bool InnerNode::hasContext()
{
	return has_context;
}

numval InnerNode::operator()(numval input)
{
	if (has_context) {
		input += context_value * context_link_weight;
		input = sigmoidalSquash(input, threshold_value);
		context_value = input;
	}
	else {
		input = sigmoidalSquash(input, threshold_value);
	}
	return input;
}


/**--------------------NeuralNetwork**/

/*Static members*/
std::random_device NeuralNetwork::seed;
std::default_random_engine NeuralNetwork::generator = std::default_random_engine(NeuralNetwork::seed());
std::uniform_int_distribution<int> NeuralNetwork::distribution_bool = std::uniform_int_distribution<int>(0,1);
std::uniform_int_distribution<int> NeuralNetwork::distribution_initial_nodes = std::uniform_int_distribution<int>(0,MAXINITIALNODES);
std::uniform_real_distribution<numval> NeuralNetwork::distribution_real_values = std::uniform_real_distribution<numval>(std::numeric_limits<numval>::lowest(), std::numeric_limits<numval>::max());

/*Default constructor*/
NeuralNetwork::NeuralNetwork():
	default_choice(distribution_bool(generator)), //random bool
	inner_nodes()
{
	//Choose number of initial nodes
	int initial_nodes = distribution_initial_nodes(generator);
	for (int i=0; i<initial_nodes; ++i) {
		addNode();
	}
}

/*Copy constructor*/
NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn):
	default_choice(nn.default_choice),
	inner_nodes()
{
	for (int i=0; i<MAXNODES; ++i) {
		if (nn.inner_nodes[i]) 
			inner_nodes[i] = new InnerNode(*nn.inner_nodes[i]);
	}
}

void NeuralNetwork::addNode()
{
	//Randomly choose if new node is cognitive or context node (unless we can't add context nodes)
	bool isContextNode = false;
	if (context_node_count < cognitive_node_count) isContextNode = distribution_bool(generator);
	
	if (isContextNode){
		//Add context node to one cognitive node (random context value and link weight)
		numval context_value = distribution_real_values(generator);
		numval link_weight = distribution_real_values(generator);
		inner_nodes[context_node_count]->setContext(context_value, link_weight);
		context_node_count++;
	}
	else if (cognitive_node_count < MAXNODES) {
		//Add cognitive node to the network (random threshold)
		numval threshold_value = distribution_real_values(generator);
		inner_nodes[cognitive_node_count] = new InnerNode(threshold_value);
		cognitive_node_count++;
	}
}

void NeuralNetwork::removeNode()
{
	//Randomly choose if deleted node is cognitive or context node (unless we can't remove context nodes)
	bool isContextNode = false;
	if (context_node_count > 0) isContextNode = distribution_bool(generator);
	
	if (isContextNode){
		//Remove context node from one cognitive node
		inner_nodes[context_node_count]->removeContext();
		context_node_count--;
	}
	else if (cognitive_node_count > 0) {
		//Remove cognitive node from the network
		delete inner_nodes[cognitive_node_count];
		inner_nodes[cognitive_node_count] = nullptr;
		cognitive_node_count++;
	}
}

int NeuralNetwork::getInnerNodeCount()
{
	return context_node_count + cognitive_node_count;
}

bool NeuralNetwork::decide(payoff self, payoff other)
{
	return self > other;
}

bool NeuralNetwork::decide()
{
	return default_choice;
}
