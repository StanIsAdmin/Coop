#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <random>
#include <iostream>

#define MAXNODES 10

typedef unsigned short payoff;


class NeuralNetwork
{
	private:
		static std::random_device seed;
		static std::default_random_engine generator;
		static std::uniform_int_distribution<int> distribution_bool;
	
		bool defaultChoice;
		double contextNodes[MAXNODES];
	
	public:
		/**Constructors**/
        NeuralNetwork(); //trivial
        NeuralNetwork(const NeuralNetwork&); //copy

        /**Destructors**/
        //No need for destructors (for now)

		/**Methods & Operators**/
		bool decide(payoff self, payoff other); //decide whether to cooperate or defect
		bool decide(); //default decision (without input)
};

#endif // NEURALNETWORK_H
