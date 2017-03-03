#include "Simulation.hpp"


Simulation::Simulation(const GamePayoffs& payoffs):
	population(),
	payoffs(payoffs)
{
	for (int i=0; i<POPULATION_SIZE; ++i) {
		population[i] = new NeuralNetwork();
	}
}

void Simulation::run(unsigned int generations)
{
	std::cout << "Running simulation..." << std::endl;
	for (unsigned int gen_count=0; gen_count<generations; ++gen_count) {
		playGeneration();
	}
	std::cout << "Simulation finished!" << std::endl;
}

void Simulation::payoffsFromChoices(bool playerACoops, bool playerBCoops, payoff& playerAPayoff, payoff& playerBPayoff)
{
	if (playerACoops && playerBCoops)  {
		playerAPayoff = payoffs.both_collaborate;
		playerBPayoff = payoffs.both_collaborate;
	}
	else if (playerACoops && !playerBCoops) {
		playerAPayoff = payoffs.self_collaborates_other_defects;
		playerBPayoff = payoffs.self_defects_other_collaborates;
	}
	else if (!playerACoops && playerBCoops) {
		playerAPayoff = payoffs.self_defects_other_collaborates;
		playerBPayoff = payoffs.self_collaborates_other_defects;
	}
	else {
		playerAPayoff = payoffs.both_defect;
		playerBPayoff = payoffs.both_defect;
	}
}

void Simulation::playGeneration()
{
	//Iterate over every possible pair of players from the population
	for (int playerAIndex=0; playerAIndex<POPULATION_SIZE-1; ++playerAIndex) {
		for (int playerBIndex=playerAIndex+1; playerBIndex<POPULATION_SIZE; ++playerBIndex) {
			playRound(playerAIndex, playerBIndex);
		}
	}
}

void Simulation::playRound(int playerAIndex, int playerBIndex)
{
	NeuralNetwork& playerA(*population[playerAIndex]);
	NeuralNetwork& playerB(*population[playerBIndex]);
	
	//Play initial iteration (no input)
	bool playerA_cooperates = playerA();
	bool playerB_cooperates = playerB();
	
	payoff playerA_payoff, playerB_payoff;
	for (int iteration=0; iteration<50; ++iteration) {
		//Gather payoffs from individual's decisions
		payoffsFromChoices(playerA_cooperates, playerB_cooperates, playerA_payoff, playerB_payoff);
		
		//Play subsequent iterations
		playerA_cooperates = playerA(playerA_payoff, playerB_payoff);
		playerB_cooperates = playerB(playerB_payoff, playerA_payoff);
	}
}