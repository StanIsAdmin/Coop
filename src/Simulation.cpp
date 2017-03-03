#include "Simulation.hpp"

/*Static members*/
std::random_device Simulation::generator;

std::negative_binomial_distribution<int> Simulation::distribution_iterations = 
	std::negative_binomial_distribution<int>(ROUND_ITERATIONS_STOP_COUNT, ROUND_ITERATIONS_MEAN_PROB);

Simulation::Simulation(const GamePayoffs& payoffs):
	population(),
	population_payoff_sum(),
	population_game_count(),
	game_payoffs(payoffs)
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
		playerAPayoff = game_payoffs.both_collaborate;
		playerBPayoff = game_payoffs.both_collaborate;
	}
	else if (playerACoops && !playerBCoops) {
		playerAPayoff = game_payoffs.self_collaborates_other_defects;
		playerBPayoff = game_payoffs.self_defects_other_collaborates;
	}
	else if (!playerACoops && playerBCoops) {
		playerAPayoff = game_payoffs.self_defects_other_collaborates;
		playerBPayoff = game_payoffs.self_collaborates_other_defects;
	}
	else {
		playerAPayoff = game_payoffs.both_defect;
		playerBPayoff = game_payoffs.both_defect;
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
	unsigned long long playerA_payoff_sum(0), playerB_payoff_sum(0);
	
	//Play a random number of iterations (mean is 50)
	int round_iterations = distribution_iterations(generator);
	for (int iteration=0; iteration<round_iterations; ++iteration) {
		//Gather payoffs from individual's decisions
		payoffsFromChoices(playerA_cooperates, playerB_cooperates, playerA_payoff, playerB_payoff);
		playerA_payoff_sum += playerA_payoff;
		playerB_payoff_sum += playerB_payoff;
		
		//Play subsequent iterations
		playerA_cooperates = playerA(playerA_payoff, playerB_payoff);
		playerB_cooperates = playerB(playerB_payoff, playerA_payoff);
	}
	population_payoff_sum[playerAIndex] += playerA_payoff_sum;
	population_payoff_sum[playerBIndex] += playerB_payoff_sum;
	population_game_count[playerAIndex] += round_iterations;
	population_game_count[playerBIndex] += round_iterations;
}