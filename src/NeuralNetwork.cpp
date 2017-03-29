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
	assert(has_context_node);
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
	assert(input >= 0 and input <= 1);
	
	return input;
}


/**-------------------- NeuralNetwork**/

/*Static members*/
RNG NeuralNetwork::rng = RNG();

/*Default constructor*/
NeuralNetwork::NeuralNetwork():
	cooperate_by_default(rng.getRandomBool()), //random bool
	output_node_threshold(rng.getRandomNumval()), //random real value
	inner_nodes() //nullptr array
{
	//Choose number of initial nodes
	int initial_nodes = rng.getInitialNodeCount();
	for (int i=0; i<initial_nodes; ++i) {
		addNode();
	}
	assert(getInnerNodeCount() == initial_nodes);
	assert(initial_nodes >= 0 and initial_nodes <= MAXINITIALNODES);
}

/*Copy constructor*/
NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn):
	cooperate_by_default(nn.cooperate_by_default),
	output_node_threshold(nn.output_node_threshold),
	inner_nodes(),
	cognitive_node_count(nn.cognitive_node_count),
	context_node_count(nn.context_node_count)
{
	for (int i=0; i<MAXNODES; ++i) {
		link_weights_from_self_payoff[i] = nn.link_weights_from_self_payoff[i];
		link_weights_from_other_payoff[i] = nn.link_weights_from_other_payoff[i];
		link_weights_from_inner_nodes[i] = nn.link_weights_from_inner_nodes[i];
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
		isContextNode = rng.getRandomBool();
	
	if (isContextNode){
		//Add context node to one cognitive node (random context value and link weight)
		numval context_value = rng.getRandomNumval();
		numval link_weight = rng.getRandomNumval();
		inner_nodes[context_node_count]->setContextNode(context_value, link_weight);
		context_node_count++;
	}
	else {
		//Add cognitive node to the network (random threshold)
		numval threshold_value = rng.getRandomNumval();
		inner_nodes[cognitive_node_count] = new InnerNode(threshold_value);
		
		//Initialize link weights to and from node with random values
		link_weights_from_self_payoff[cognitive_node_count] = rng.getRandomNumval();
		link_weights_from_other_payoff[cognitive_node_count] = rng.getRandomNumval();
		link_weights_from_inner_nodes[cognitive_node_count] = rng.getRandomNumval();
		cognitive_node_count++;
	}
}

void NeuralNetwork::removeNode()
{
	if (cognitive_node_count == 0) //there are no nodes to remove
		return;
	
	//Choose if deleted node is cognitive or context node
	bool isContextNode = false;
	if (context_node_count > 0) isContextNode = rng.getRandomBool();
	
	if (isContextNode){
		//Remove context node from one cognitive node
		context_node_count--;
		inner_nodes[context_node_count]->removeContextNode();
	}
	else {
		//If cognitive node has context node, uncount it
		cognitive_node_count--;
		if (inner_nodes[cognitive_node_count]->hasContextNode())
			context_node_count--;
		
		//Remove cognitive node from the network
		delete inner_nodes[cognitive_node_count];
		inner_nodes[cognitive_node_count] = nullptr;
	}
}

void NeuralNetwork::mutate()
{
	///Modify numeric values
	for (int i=0; i<cognitive_node_count; i++) {
		///Link weights
		//From self payoff to inner nodes
		if (rng.getRandomProbability() < value_mutation_prob) {
			link_weights_from_self_payoff[i] += rng.getRandomNumval();
		}
		//From other payoff to inner nodes
		if (rng.getRandomProbability() < value_mutation_prob) {
			link_weights_from_other_payoff[i] += rng.getRandomNumval();
		}
		//From inner nodes to output
		if (rng.getRandomProbability() < value_mutation_prob) {
			link_weights_from_inner_nodes[i] += rng.getRandomNumval();
		}
		//From context nodes to cognitive nodes
		if (rng.getRandomProbability() < value_mutation_prob) {
			inner_nodes[i]->context_link_weight += rng.getRandomNumval();
		}
		
		///Node thresholds
		//Cognitive nodes
		if (rng.getRandomProbability() < value_mutation_prob) {
			inner_nodes[i]->threshold_value += rng.getRandomNumval();
		}
	}
	
	//Output node threshold
	if (rng.getRandomProbability() < value_mutation_prob) {
		output_node_threshold += rng.getRandomNumval();
	}
	
	///Network structure
	if (rng.getRandomProbability() < network_mutation_prob) {
		if (rng.getRandomBool()) addNode();
		else removeNode();
	}
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
	numval cooperate_prob = sigmoidalSquash(output, output_node_threshold);
	
	//If random prob < cooperate prob : cooperate
	return rng.getRandomProbability() < cooperate_prob;
}

/*Returns true if it chooses to cooperate by default, false otherwise*/
bool NeuralNetwork::operator()()
{
	return cooperate_by_default;
}
