#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>

#include "Rng.hpp"
#include "Payoffs.hpp"

#define MAXNODES 10
#define NETWORK_VALUE_MUTATION_PROB 0.1
#define NETWORK_STRUCTURE_MUTATION_PROB 0.02

typedef double numval;

/*Squashing function used by cognitive and output nodes*/
numval sigmoidalSquash(numval value, numval threshold);


/*Represents a cognitive node that may or may not be attached to a context node*/
class InnerNode
{
	friend class NeuralNetwork;
	
	private:
		numval threshold_value;	//used by the squashing function of the cognitive node	
		
		bool has_context_node = false; //is a context node attached ?
		numval context_value = 0; //the context value acts as a memory
		numval context_link_weight = 0; //multiplicator for input from context node
	
	public:
		///Constructors
		InnerNode() = delete; //threshold needs to be provided
		InnerNode(numval threshold); //creates a cognitive node without a context node
		InnerNode(const InnerNode& in); //copy constructor
		
		//set values for associated context node (creates context node if needed)
		bool hasContextNode(); //does the cognitive node has associated context node
		void addContextNode(numval value, numval link_weight); 
		void removeContextNode(); //remove associated context node
		
		numval operator()(numval input); //get output value given the provided input
		
		bool operator==(const InnerNode& in);
};


/* Represents a complete neural network with up to 10 cognitive and 10 context nodes*/
class NeuralNetwork
{	
	private:
		///Neural network structure
		bool cooperate_by_default; //used for decision-making in first round
		numval output_node_threshold; //same use as inner nodes thresholds
		
		int context_node_count = 0; //number of context nodes
		std::vector<InnerNode*> inner_nodes; //Neural network's hidden layer nodes
		std::vector<numval> link_weights_from_self_payoff; //link weights between first input and nodes
		std::vector<numval> link_weights_from_other_payoff; //...between second input and nodes
		std::vector<numval> link_weights_from_inner_nodes; //...between nodes and output
		
		int getRandomCognitiveNode(bool withContext);
	
	public:
		///Constructors
        NeuralNetwork(); //trivial ctr
        NeuralNetwork(const NeuralNetwork&); //copy
		NeuralNetwork(NeuralNetwork&&) = default; //move
		
		///Assignments
		NeuralNetwork& operator=(const NeuralNetwork&) = delete; 
		NeuralNetwork& operator=(NeuralNetwork&&) = delete; 

        ///Destructors
        ~NeuralNetwork(); //deletes InnerNode objects

		/**Methods & Operators**/
		void addNode(); //adds a node to the structure if possible
		void addContextNode(); //adds a context node to the structure (place must be available)
		void addCognitiveNode(); //adds a cognitive node to the structure (place must be available)
		
		void removeNode(); //removes a node from the structure if possible
		void removeContextNode();
		void removeCognitiveNode();
		
		void mutate(); //implements all specified mutations with given random probabilities
		
		int getInnerNodeCount()const;
		int getCognitiveNodeCount() const;
		int getContextNodeCount() const;
		
		bool operator()(payoff self_payoff, payoff other_payoff); //decide whether to cooperate or defect
		bool operator()(); //default decision (without input)
		
		bool operator==(const NeuralNetwork& nn);
		bool operator!=(const NeuralNetwork& nn);
};

#endif // NEURALNETWORK_H
