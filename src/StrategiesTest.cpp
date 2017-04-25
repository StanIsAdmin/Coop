#include "StrategiesTest.hpp"


void testStrategies()
{
	std::cout << "Testing Strategies...";
	
	//Strategies creation
	Strategies strat = Strategies(Payoffs::getPayoffsForGameType("ISD"));
	
	//Empty Network with default cooperation has constant strategies
	for (int i=0; i<STRATEGIES_TEST_SAMPLE_SIZE; ++i) {
		NeuralNetwork nn = NeuralNetwork();
		for (int j=0; j<MAXNODES; ++j) {
			nn.removeNode();
		}
		if (nn()) {
			assert(strat.closestPureStrategy(nn) == STRATEGIES_ALWAYS_COOPERATE);
		} else {
			assert(strat.closestPureStrategy(nn) == STRATEGIES_ALWAYS_DEFECT);
		}
		
	}
	
	std::cout << " done!" << std::endl;
}