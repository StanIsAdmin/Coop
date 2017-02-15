#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <random>
#include <cmath>
#include <limits>
#include <iostream>

#define MAXNODES 10
#define MAXINITIALNODES 3
#define NUMVAL_MEAN 0
#define NUMVAL_STDDEV 0.5

typedef unsigned short payoff;
typedef double numval;


/*Squashing function used by cognitive and output nodes*/
numval sigmoidalSquash(numval value, numval threshold);


/* Represents a cognitive node that may or may not be attached to a context node */
class InnerNode
{
	private:
		bool has_context_node = false; //is a context node attached ?
		numval context_value = 0; //the context value acts as a memory
		numval context_link_weight = 0; //multiplicator for input from context node
		
		numval threshold_value;	//used by the squashing function of the cognitive node	
	
	public:
		///Constructors
		InnerNode() = delete; //threshold needs to be provided
		InnerNode(numval threshold); //creates a cognitive node without a context node
		InnerNode(const InnerNode&) = default;
		
		//set values for associated context node (creates context node if needed)
		void setContextNode(numval value, numval link_weight); 
		void removeContextNode(); //remove associated context node
		bool hasContextNode(); //does the cognitive node has associated context node
		
		numval operator()(numval input); //get output value given the provided input
};


/* Represents a complete neural network with up to 10 cognitive and 10 context nodes*/
class NeuralNetwork
{
	private:
		///Randomness
		static std::random_device generator; //random number generator TODO: check efficiency
		static std::uniform_int_distribution<int> distribution_bool; //0 or 1
		static std::uniform_int_distribution<int> distribution_initial_nodes; //0 to MAXINITIALNODES
		//normal distribution (mean = NUMVAL_MEAN, std deviation = NUMVAL_STDDEV)
		static std::normal_distribution<numval> distribution_real_values; 
		static std::uniform_real_distribution<numval> distribution_prob_values; //0 to 1
	
		///Neural network structure
		bool collaborate_by_default; //used for decision-making in first round
		numval output_node_threshold; //same use as inner nodes thresholds
		
		InnerNode* inner_nodes[MAXNODES]; //Neural network's hidden layer nodes
		numval link_weights_from_self_payoff[MAXNODES]; //link weights between first input and nodes
		numval link_weights_from_other_payoff[MAXNODES]; //...between second input and nodes
		numval link_weights_from_inner_nodes[MAXNODES]; //...between nodes and output
		
		int cognitive_node_count = 0;
		int context_node_count = 0;
	
	public:
		///Constructors
        NeuralNetwork(); //trivial
        NeuralNetwork(const NeuralNetwork&); //copy

        ///Destructors
        ~NeuralNetwork(); //deletes InnerNode objects

		/**Methods & Operators**/
		void addNode(); //adds a node to the structure if possible
		void removeNode(); //removes a node from the structure if possible
		
		int getInnerNodeCount();
		int getCognitiveNodeCount();
		int getContextNodeCount();
		
		bool operator()(payoff self, payoff other); //decide whether to cooperate or defect
		bool operator()(); //default decision (without input)
};

#endif // NEURALNETWORK_H
