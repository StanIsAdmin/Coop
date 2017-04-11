#include "NeuralNetwork.hpp"


/**---------- Out of class ----------**/

/* Computes the sigmoidal squash of -value -threshold (maps values from [-inf, inf] to [0, 1]) */
numval sigmoidalSquash(numval value, numval threshold)
{
	return 1/(1+std::exp(-value-threshold));
}


/**---------- InnerNode ----------**/

/*Regular constructor with provided threshold value*/
InnerNode::InnerNode(numval threshold):
	threshold_value(threshold)
	{}
	
bool InnerNode::hasContextNode()
{
	return has_context_node;
}

/*Adds a context node to the cognitive node*/
void InnerNode::addContextNode(numval value, numval link_weight)
{
	assert(not hasContextNode());
	has_context_node = true;
	context_value = value;
	context_link_weight = link_weight;
}

/*Removes the context node from the cognitive node*/
void InnerNode::removeContextNode()
{
	assert(hasContextNode());
	has_context_node = false;
	context_value = 0;
	context_link_weight = 0;
}

/*Returns the node output given the provided input*/
numval InnerNode::operator()(numval input)
{
	assert(not std::isnan(input)); //verify numval is a regular numeric value
	if (has_context_node) {
		input += context_value * context_link_weight; //add weighted context to input
		input = sigmoidalSquash(input, threshold_value);
		context_value = input; //store result in context node
	}
	else {
		input = sigmoidalSquash(input, threshold_value);
	}
	assert(input >= 0 and input <= 1); //verify the squashing function worked
	
	return input;
}

bool InnerNode::operator==(const InnerNode& in)
{
	return has_context_node == in.has_context_node
		and context_value == in.context_value
		and context_link_weight == in.context_link_weight
		and threshold_value == in.threshold_value;
}


/**---------- NeuralNetwork ----------**/

/*Static members*/
RNG NeuralNetwork::rng = RNG();

/*Default constructor*/
NeuralNetwork::NeuralNetwork():
	cooperate_by_default(rng.getRandomBool()), //random bool
	output_node_threshold(rng.getRandomNumval()) //random real value
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
	cognitive_node_count(nn.cognitive_node_count),
	context_node_count(nn.context_node_count),
	link_weights_from_self_payoff(nn.link_weights_from_self_payoff),
	link_weights_from_other_payoff(nn.link_weights_from_other_payoff),
	link_weights_from_inner_nodes(nn.link_weights_from_inner_nodes)
{
	//Create new inner nodes by copying nn's inner nodes
	for (int i=0; i<nn.cognitive_node_count; ++i) {
		inner_nodes.push_back(new InnerNode(*nn.inner_nodes[i]));
	}
	assert(getInnerNodeCount() >= 0 and getInnerNodeCount() <= MAXNODES*2);
}

/*Destructor*/
NeuralNetwork::~NeuralNetwork()
{
	for (int i=0; i<cognitive_node_count; ++i) {
		delete inner_nodes[i];
	}
}

int NeuralNetwork::getRandomCognitiveNode(bool withContext)
{
	//Find which cognitive nodes have or do not have a context (depending on withContext)
	std::vector<int> nodeSelection(0);
	for (int i=0; i<cognitive_node_count; ++i) {
		if (inner_nodes[i]->hasContextNode() == withContext)
			nodeSelection.push_back(i);
	}
	
	if (withContext)
		assert(context_node_count == static_cast<int>(nodeSelection.size()));
	else
		assert(cognitive_node_count - context_node_count == static_cast<int>(nodeSelection.size()));
	
	//Choose random cognitive node from the context-free list
	unsigned int chosen_context_node = nodeSelection[rng.getRandomInt(0, static_cast<int>(nodeSelection.size())-1)];
	
	assert(inner_nodes[chosen_context_node]->hasContextNode() == withContext);
	
	return chosen_context_node;
}

/*If possible, adds a node to the network. 
Choice between context and cognitive nodes is random if both choices are allowed*/
void NeuralNetwork::addNode()
{
	if (getInnerNodeCount() == MAXNODES*2) //Cannot add new nodes
		return;
	
	//Choose if new node is cognitive or context node
	bool isContextNode;
	if (cognitive_node_count == MAXNODES) 					//Can't add cognitive node
		isContextNode = true;
	else if (context_node_count == cognitive_node_count)	//Can't add context node
		isContextNode = false;
	else 													//Can add either, choose randomly
		isContextNode = rng.getRandomBool();
	
	if (isContextNode) addContextNode();
	else addCognitiveNode();
}

/*Adds a context node to a randomly chose, context-free cognitive node.
If such a cognitive nodes does not exist, assertion fails.*/
void NeuralNetwork::addContextNode() 
{
	assert(context_node_count < cognitive_node_count);
	
	//Choose random cognitive node that does NOT have context
	unsigned int chosen_context_node = getRandomCognitiveNode(false);
	
	//Add context node to one cognitive node (random context value and link weight)
	numval context_value = rng.getRandomNumval();
	numval link_weight = rng.getRandomNumval();
	inner_nodes[chosen_context_node]->addContextNode(context_value, link_weight);
	
	context_node_count++;
}

/*Adds a cognitive node to the network.
If the maximum amount of cognitive nodes is already reached, assertion fails.*/
void NeuralNetwork::addCognitiveNode()
{
	assert(cognitive_node_count < MAXNODES);
	assert(cognitive_node_count == static_cast<int>(inner_nodes.size()));
	
	//Add cognitive node to the network (random threshold)
	numval threshold_value = rng.getRandomNumval();
	inner_nodes.push_back(new InnerNode(threshold_value));
	
	//Initialize link weights to and from node with random values
	link_weights_from_self_payoff.push_back(rng.getRandomNumval());
	link_weights_from_other_payoff.push_back(rng.getRandomNumval());
	link_weights_from_inner_nodes.push_back(rng.getRandomNumval());
	
	cognitive_node_count++;
}

/*If possible, removes a randomly chosen, context or cognitive node from the network. 
Choice between context and cognitive nodes is random if both choices are allowed*/
void NeuralNetwork::removeNode()
{
	if (cognitive_node_count == 0) //there are no nodes to remove
		return;
	
	//Choose if deleted node is cognitive or context node
	bool isContextNode = false;
	if (context_node_count > 0) isContextNode = rng.getRandomBool();
	
	if (isContextNode)
		removeContextNode();
	else 
		removeCognitiveNode();
}

void NeuralNetwork::removeContextNode() 
{
	assert(context_node_count > 0 and cognitive_node_count >= context_node_count);
	
	//Get random cognitive node that DOES have a context
	int chosen_context_node = getRandomCognitiveNode(true);
	
	//Remove context node from one cognitive node
	inner_nodes[chosen_context_node]->removeContextNode();
	context_node_count--;
}

void NeuralNetwork::removeCognitiveNode()
{
	assert(cognitive_node_count > 0 and cognitive_node_count == static_cast<int>(inner_nodes.size()));
	
	//Choose random cognitive node
	int chosen_cognitive_node = rng.getRandomInt(0, cognitive_node_count-1);
	
	//If cognitive node has context node, uncount it
	cognitive_node_count--;
	if (inner_nodes[chosen_cognitive_node]->hasContextNode())
		context_node_count--;
	
	//Remove cognitive node from the network
	delete inner_nodes[chosen_cognitive_node];
	inner_nodes.erase(inner_nodes.begin() + chosen_cognitive_node);
	
	//Remove its link weights
	link_weights_from_self_payoff.erase(link_weights_from_self_payoff.begin() + chosen_cognitive_node);
	link_weights_from_other_payoff.erase(link_weights_from_other_payoff.begin() + chosen_cognitive_node);
	link_weights_from_inner_nodes.erase(link_weights_from_inner_nodes.begin() + chosen_cognitive_node);
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

/*Returns true if both neuralnetworks have the exact same structures and values, false otherwise*/
bool NeuralNetwork::operator==(const NeuralNetwork& nn)
{
	return context_node_count == nn.context_node_count
		and cognitive_node_count == nn.cognitive_node_count
		and cooperate_by_default == nn.cooperate_by_default
		and output_node_threshold == nn.output_node_threshold
		and link_weights_from_self_payoff == nn.link_weights_from_self_payoff
		and link_weights_from_other_payoff == nn.link_weights_from_other_payoff
		and link_weights_from_inner_nodes == nn.link_weights_from_inner_nodes
		and std::equal(inner_nodes.begin(), inner_nodes.end(), nn.inner_nodes.begin(), 
               [](InnerNode* left, InnerNode* right){ return *left == *right; });
}

bool NeuralNetwork::operator!=(const NeuralNetwork& nn)
{
	return not operator==(nn);
}
