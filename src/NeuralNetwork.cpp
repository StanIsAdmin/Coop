#include "NeuralNetwork.hpp"


/**-------------------- Out of class**/

/* Computes the sigmoidal squash of -value -threshold (maps values from [-inf, inf] to [0, 1]) */
numval sigmoidalSquash(numval value, numval threshold)
{
	return 1/(1+std::exp(-value-threshold));
}

/**-------------------- InnerNode**/

/*Regular constructor with provided threshold value*/
InnerNode::InnerNode(numval threshold):
	threshold_value(threshold)
	{}

void InnerNode::setContextNode(numval value, numval link_weight)
{
	has_context_node = true;
	context_value = value;
	context_link_weight = link_weight;
}

void InnerNode::removeContextNode()
{
	has_context_node = false;
}
	
bool InnerNode::hasContextNode()
{
	return has_context_node;
}

/* Returns the node output given the provided input*/
numval InnerNode::operator()(numval input)
{
	if (has_context_node) {
		input += context_value * context_link_weight; //add weighted context to input
		input = sigmoidalSquash(input, threshold_value);
		context_value = input; //store result in context node
	}
	else {
		input = sigmoidalSquash(input, threshold_value);
	}
	return input;
}


/**-------------------- NeuralNetwork**/

/*Static members*/
std::random_device NeuralNetwork::generator;

std::uniform_int_distribution<int> NeuralNetwork::distribution_bool = 
	std::uniform_int_distribution<int>(0,1);
std::uniform_int_distribution<int> NeuralNetwork::distribution_initial_nodes = 
	std::uniform_int_distribution<int>(0,MAXINITIALNODES);
std::normal_distribution<numval> NeuralNetwork::distribution_real_values = 
	std::normal_distribution<numval>(NUMVAL_MEAN, NUMVAL_STDDEV);
std::uniform_real_distribution<numval> NeuralNetwork::distribution_prob_values = 
	std::uniform_real_distribution<numval>(0, 1);

/*Default constructor*/
NeuralNetwork::NeuralNetwork():
	collaborate_by_default(distribution_bool(generator)), //random bool
	output_node_threshold(distribution_real_values(generator)), //random real value
	inner_nodes() //nullptr array
{
	//Choose number of initial nodes
	int initial_nodes = distribution_initial_nodes(generator);
	for (int i=0; i<initial_nodes; ++i) {
		addNode();
	}
}

/*Copy constructor*/
NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn):
	collaborate_by_default(nn.collaborate_by_default),
	output_node_threshold(nn.output_node_threshold),
	inner_nodes()
{
	for (int i=0; i<MAXNODES; ++i) {
		if (nn.inner_nodes[i]) 
			inner_nodes[i] = new InnerNode(*nn.inner_nodes[i]);
	}
}

/*Destructor*/
NeuralNetwork::~NeuralNetwork()
{
	for (int i=0; i<cognitive_node_count; ++i) {
		delete inner_nodes[i];
	}
}

/*If possible, adds a node to the network.
  Choice between context and cognitive nodes is random if both choices are allowed*/
void NeuralNetwork::addNode()
{
	if (cognitive_node_count + context_node_count == MAXNODES*2) //Cannot add new nodes
		return;
	
	//Choose if new node is cognitive or context node
	bool isContextNode;
	if (cognitive_node_count == MAXNODES) //Can't add cognitive node
		isContextNode = true;
	else if (context_node_count == cognitive_node_count) //Can't add context node
		isContextNode = false;
	else
		isContextNode = distribution_bool(generator);
	
	if (isContextNode){
		//Add context node to one cognitive node (random context value and link weight)
		numval context_value = distribution_real_values(generator);
		numval link_weight = distribution_real_values(generator);
		inner_nodes[context_node_count]->setContextNode(context_value, link_weight);
		context_node_count++;
	}
	else {
		//Add cognitive node to the network (random threshold)
		numval threshold_value = distribution_real_values(generator);
		inner_nodes[cognitive_node_count] = new InnerNode(threshold_value);
		
		//Initialize link weights to and from node with random values
		link_weights_from_self_payoff[cognitive_node_count] = distribution_real_values(generator);
		link_weights_from_other_payoff[cognitive_node_count] = distribution_real_values(generator);
		link_weights_from_inner_nodes[cognitive_node_count] = distribution_real_values(generator);
		cognitive_node_count++;
	}
}

void NeuralNetwork::removeNode()
{
	std::cout << "A" << std::endl;
	if (cognitive_node_count == 0) //there are no nodes to remove
		return;
	std::cout << "B" << std::endl;
	//Choose if deleted node is cognitive or context node
	bool isContextNode = false;
	if (context_node_count > 0) isContextNode = distribution_bool(generator);
	std::cout << "C" << std::endl;
	if (isContextNode){
		std::cout << "D1" << std::endl;
		//Remove context node from one cognitive node
		std::cout << inner_nodes[context_node_count] << std::endl;
		inner_nodes[context_node_count]->removeContextNode();
		std::cout << "D1" << std::endl;
		context_node_count--;
		std::cout << "D1" << std::endl;
	}
	else {
		std::cout << "D2" << std::endl;
		//Remove cognitive node from the network
		if (inner_nodes[cognitive_node_count]->hasContextNode())
			context_node_count--;
		
		std::cout << inner_nodes[cognitive_node_count] << std::endl;
		delete inner_nodes[cognitive_node_count];
		std::cout << "D2" << std::endl;
		inner_nodes[cognitive_node_count] = nullptr;
		std::cout << "D2" << std::endl;
		cognitive_node_count--;
		std::cout << "D2" << std::endl;
	}
	std::cout << "E" << std::endl;
}

int NeuralNetwork::getInnerNodeCount()
{
	return context_node_count + cognitive_node_count;
}
int NeuralNetwork::getCognitiveNodeCount()
{
	return cognitive_node_count;
}
int NeuralNetwork::getContextNodeCount()
{
	return context_node_count;
}

/*Returns true if it chooses to cooperate based on the input, false otherwise*/
bool NeuralNetwork::operator()(payoff self, payoff other)
{
	//If there are no cognitive nodes, use default choice
	if (cognitive_node_count == 0) return (*this)();
	
	//Use inner nodes to compute output
	numval output = 0;
	for (int i=0; i<cognitive_node_count; ++i) {
		numval selfInput = self * link_weights_from_self_payoff[i];
		numval otherInput = other * link_weights_from_other_payoff[i];
		output += (*inner_nodes[i])(selfInput + otherInput) * link_weights_from_inner_nodes[i];
	}
	//Squash output into collaboration probability
	numval collaborate_prob = sigmoidalSquash(output, output_node_threshold);
	
	//If random prob < collaborate prob : collaborate
	return distribution_prob_values(generator) < collaborate_prob;
}

/*Returns true if it chooses to cooperate by default, false otherwise*/
bool NeuralNetwork::operator()()
{
	return collaborate_by_default;
}
