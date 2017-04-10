#include "Simulation.hpp"


/*Static members*/
RNG Simulation::rng = RNG();

Simulation::Simulation(const GamePayoffs& payoffs):
	strats(payoffs), //strategy evaluation
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
	std::cout << "# RNG seed: " << rng.getSeed();
	if (rng.seedIsRandom())
		std::cout << " (random)" << std::endl << std::endl;
	else
		std::cout << " (provided)" << std::endl << std::endl;
	
	for (unsigned int gen_count=0; gen_count<generations; ++gen_count) {
		playGeneration();
		evaluatePopulationFitness();
		assessPopulation();
		nextGeneration();
	}
	outputResults(generations);
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
		game_payoffs.payoffsFromChoices(playerA_cooperates, playerB_cooperates, playerA_payoff, playerB_payoff);
		playerA_payoff_sum += playerA_payoff;
		playerB_payoff_sum += playerB_payoff;
		
		//Play subsequent iterations
		playerA_cooperates = playerA(playerA_payoff, playerB_payoff);
		playerB_cooperates = playerB(playerB_payoff, playerA_payoff);
		
		//Count total cooperations/defections
		if (playerA_cooperates) total_cooperations += 1;
		else total_defections += 1;
		if (playerB_cooperates) total_cooperations += 1;
		else total_defections += 1;
	}
	
	//Modify the player's stats accordingly
	population_payoff_sum[playerAIndex] += playerA_payoff_sum;
	population_payoff_sum[playerBIndex] += playerB_payoff_sum;
	population_game_count[playerAIndex] += round_iterations;
	population_game_count[playerBIndex] += round_iterations;
}

void Simulation::evaluatePopulationFitness()
{
	//calculate fitness based on mean payoff per round
	for (int i=0; i<POPULATION_SIZE; ++i) {
		population_fitness[i] = (double(population_payoff_sum[i])/double(population_game_count[i])) - (NODE_FITNESS_PENALTY * population[i]->getInnerNodeCount());
	}
}

/*Determines the current population's typical strategies and other metrics*/
void Simulation::assessPopulation()
{
	//Population intelligence and fitness
	for (int i=0; i<POPULATION_SIZE; ++i) {
		pop_intelligence += std::to_string(population[i]->getInnerNodeCount()) + " ";
		pop_fitness += std::to_string(population_fitness[i]) + " ";
	}
	pop_intelligence += "\n";
	pop_fitness += "\n";
	
	//Average cooperation frequency
	avg_cooperation += std::to_string(static_cast<double>(total_cooperations) / static_cast<double>(total_cooperations + total_defections)) + "\n";
	
	//Strategy counts
	for (int i=0; i<POPULATION_SIZE; ++i) {
		population_strategies[strats.closestPureStrategy(*(population[i]))] += 1;
 	}
	for (int i=0; i<STRATEGIES_COUNT; ++i) {
		strategies_counts[i] += std::to_string(population_strategies[i]) + "\n";
	}
}

/*Replaces the current generation by selection based on fitness followed by mutation*/
void Simulation::nextGeneration()
{	
	//select population with probability proportional to individual's fitness
	std::array<int, POPULATION_SIZE> new_population_indexes;
	rng.selectPopulation<POPULATION_SIZE>(population_fitness, new_population_indexes);
	
	//create the new population with the new selection
	NeuralNetwork* new_population[POPULATION_SIZE];
	int selected_index;
	for (int i=0; i<POPULATION_SIZE; ++i) {
		selected_index = new_population_indexes[i]; //index of selected individual
		new_population[i] = new NeuralNetwork(*population[selected_index]); //copy the NN
	}
	
	//replace the old population, reset their stats, and mutate the new individuals
	for (int i=0; i<POPULATION_SIZE; ++i) {
		delete population[i];
		population_game_count[i] = 0;
		population_payoff_sum[i] = 0;
		population[i] = new_population[i]; //copy pointer to new NeuralNetwork
		population[i]->mutate();
	}
	
	//reset strategies counts
	for (int i=0; i<STRATEGIES_COUNT; ++i) {
		population_strategies[i] = 0;
	}
	
	total_defections = 0;
	total_cooperations = 0;
}

/*Writes the simulation's results to the standard output*/
void Simulation::outputResults(unsigned int generations)
{
	//Population intelligence
	std::cout << "# name: pop_intelligence" << std::endl;
	std::cout << "# type: matrix" << std::endl;
	std::cout << "# rows: " << generations << std::endl;
	std::cout << "# columns: "<< POPULATION_SIZE << std::endl;
	std::cout << pop_intelligence << std::endl;
	
	//Population fitness
	std::cout << "# name: pop_fitness" << std::endl;
	std::cout << "# type: matrix" << std::endl;
	std::cout << "# rows: " << generations << std::endl;
	std::cout << "# columns: "<< POPULATION_SIZE << std::endl;
	std::cout << pop_fitness << std::endl;
	
	//Average cooperation
	std::cout << "# name: avg_cooperation" << std::endl;
	std::cout << "# type: matrix" << std::endl;
	std::cout << "# rows: " << generations << std::endl;
	std::cout << "# columns: 1" << std::endl;
	std::cout << avg_cooperation << std::endl;
	
	//Strategies counts
	for (int i=0; i<STRATEGIES_COUNT; ++i) {
		std::cout << "# name: count_" << strategies_names[i] << std::endl;
		std::cout << "# type: matrix" << std::endl;
		std::cout << "# rows: " << generations << std::endl;
		std::cout << "# columns: 1" << std::endl;
		std::cout << strategies_counts[i] << std::endl;
	}
}