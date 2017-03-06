#include "Simulation.hpp"


/*Static members*/
RNG Simulation::rng = RNG();

Simulation::Simulation(const GamePayoffs& payoffs):
	population(), //nullptr array
	population_payoff_sum(), //array of 0s
	population_game_count(), //array of 0s
	game_payoffs(payoffs) //use provided payoffs
{
	//create initial population of NeuralNetworks
	for (int i=0; i<POPULATION_SIZE; ++i) {
		//NNs are initialized with random structures (as specified)
		population[i] = new NeuralNetwork();
	}
}

/*Executes one complete simulation with a certain number of generations*/
void Simulation::run(unsigned int generations)
{
	std::cout << "Running simulation..." << std::endl;
	for (unsigned int gen_count=0; gen_count<generations; ++gen_count) {
		playGeneration();
	}
	std::cout << "Simulation finished!" << std::endl;
}

/*Assigns payoffs to both players depending on their choices and the game rules*/
void Simulation::payoffsFromChoices(bool playerACoops, bool playerBCoops, payoff& playerAPayoff, payoff& playerBPayoff)
{
	//both cooperate
	if (playerACoops && playerBCoops)  {
		playerAPayoff = game_payoffs.both_cooperate;
		playerBPayoff = game_payoffs.both_cooperate;
	}
	//A cooperates, B defects
	else if (playerACoops && !playerBCoops) {
		playerAPayoff = game_payoffs.self_cooperates_other_defects;
		playerBPayoff = game_payoffs.self_defects_other_cooperates;
	}
	//A defects, B cooperates
	else if (!playerACoops && playerBCoops) {
		playerAPayoff = game_payoffs.self_defects_other_cooperates;
		playerBPayoff = game_payoffs.self_cooperates_other_defects;
	}
	//both defect
	else {
		playerAPayoff = game_payoffs.both_defect;
		playerBPayoff = game_payoffs.both_defect;
	}
}

/*Plays all individuals from this generation against each other*/
void Simulation::playGeneration()
{
	//Iterate over every possible pair of players from the population
	for (int playerAIndex=0; playerAIndex<POPULATION_SIZE-1; ++playerAIndex) {
		for (int playerBIndex=playerAIndex+1; playerBIndex<POPULATION_SIZE; ++playerBIndex) {
			playEachOther(playerAIndex, playerBIndex);
		}
	}
}

/*Plays two individuals against each other for a number of iterations (or "rounds")*/
void Simulation::playEachOther(int playerAIndex, int playerBIndex)
{
	NeuralNetwork& playerA(*population[playerAIndex]);
	NeuralNetwork& playerB(*population[playerBIndex]);
	
	payoff playerA_payoff, playerB_payoff; //results of each game iteration
	unsigned long long playerA_payoff_sum(0), playerB_payoff_sum(0); //sum of all payoffs
	
	//Play initial iteration (no input)
	bool playerA_cooperates = playerA();
	bool playerB_cooperates = playerB();
	
	//Play a random number of iterations (mean is 50)
	int round_iterations = rng.getIterationCount();
	
	for (int iteration=0; iteration<round_iterations; ++iteration) {
		//Gather payoffs from individual's decisions
		payoffsFromChoices(playerA_cooperates, playerB_cooperates, playerA_payoff, playerB_payoff);
		playerA_payoff_sum += playerA_payoff;
		playerB_payoff_sum += playerB_payoff;
		
		//Play subsequent iterations
		playerA_cooperates = playerA(playerA_payoff, playerB_payoff);
		playerB_cooperates = playerB(playerB_payoff, playerA_payoff);
	}
	
	//Modify the player's stats accordingly
	population_payoff_sum[playerAIndex] += playerA_payoff_sum;
	population_payoff_sum[playerBIndex] += playerB_payoff_sum;
	population_game_count[playerAIndex] += round_iterations;
	population_game_count[playerBIndex] += round_iterations;
}

/*Creates a new population by selection based on fitness followed by mutation*/
void Simulation::nextGeneration()
{
	std::array<double, POPULATION_SIZE> population_fitness;
	
	//calculate fitness based on mean payoff per round
	for (int i=0; i<POPULATION_SIZE; ++i) {
		population_fitness[i] = (double(population_payoff_sum[i])/double(population_game_count[i])) - (NODE_FITNESS_PENALTY * population[i]->getInnerNodeCount());
	}
	
	//probability of selection is proportional to individual's fitness
	std::discrete_distribution<int> distribution_population(population_fitness.begin(), population_fitness.end());
	
	//select new population from the previous one, based on the distribution
	NeuralNetwork* new_population[POPULATION_SIZE];
	int selected_index;
	
	for (int i=0; i<POPULATION_SIZE; ++i) {
		selected_index = distribution_population(rng.generator); //choose an index
		new_population[i] = new NeuralNetwork(*(population[selected_index])); //copy the NN
	}
	
	//replace the population, reset their stats, and mutate the new individuals
	for (int i=0; i<POPULATION_SIZE; ++i) {
		delete population[i];
		population_game_count[i] = 0;
		population_payoff_sum[i] = 0;
		population[i] = new_population[i];
		population[i]->mutate();
	}
}