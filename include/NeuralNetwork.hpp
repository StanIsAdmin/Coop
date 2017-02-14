#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#define MAXNODES 10

typedef unsigned short payoff;

class NeuralNetwork
{
	private:
		double contextNodes[MAXNODES];
	
	public:
		/**Constructors**/
        NeuralNetwork() = default; //trivial
        NeuralNetwork(const NeuralNetwork&); //copy

        /**Destructors**/
        //No need for destructors (for now)

		/**Methods & Operators**/
		bool decide(payoff self, payoff other); //decide whether to cooperate or defect
};

#endif // NEURALNETWORK_H
