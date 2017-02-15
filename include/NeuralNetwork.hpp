#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <random>
#include <cmath>
#include <limits>
#include <iostream>

#define MAXNODES 10
#define MAXINITIALNODES 3
#define MAXINITIALNUMVAL 1e10

typedef unsigned short payoff;
typedef double numval;

/* Computes the sigmoidal squash of -value -threshold
 * (maps values from [-inf, inf] to [0, 1]) */
numval sigmoidalSquash(numval value, numval threshold);


class InnerNode
{
	private:
		bool has_context = false;
		numval context_value = 0;
		numval context_link_weight = 0;
		numval threshold_value;		
	
	public:
		InnerNode() = delete;
		InnerNode(const InnerNode&) = default; //copy
		InnerNode(numval threshold); //node without context
		
		//set values for associated context node (create if needed)
		void setContext(numval value, numval link_weight); 
		void removeContext(); //remove associated context node
		bool hasContext(); //check if cognitive node has associated context node
		
		numval operator()(numval input); //get output value from input
};

class NeuralNetwork
{
	private:
		static std::random_device seed;
		static std::default_random_engine generator;
		static std::uniform_int_distribution<int> distribution_bool;
		static std::uniform_int_distribution<int> distribution_initial_nodes;
		static std::uniform_real_distribution<numval> distribution_real_values;
		static std::uniform_real_distribution<numval> distribution_prob_values;
	
		bool default_choice;
		numval output_node_threshold;
		
		InnerNode* inner_nodes[MAXNODES];
		numval link_weights_from_self_payoff[MAXNODES];
		numval link_weights_from_other_payoff[MAXNODES];
		numval link_weights_from_inner_nodes[MAXNODES];
		
		int cognitive_node_count = 0;
		int context_node_count = 0;
		void addNode(); //adds a random node to the structure
		void removeNode(); //removes the top node from the structure
	
	public:
		/**Constructors**/
        NeuralNetwork(); //trivial
        NeuralNetwork(const NeuralNetwork&); //copy

        /**Destructors**/
        //No need for destructors (for now)

		/**Methods & Operators**/
		bool decide(payoff self, payoff other); //decide whether to cooperate or defect
		bool decide(); //default decision (without input)
		
		int getInnerNodeCount(); //returns the number of inner nodes
		int getCognitiveNodeCount();
		int getContextNodeCount();
};

#endif // NEURALNETWORK_H
