#include "Simulation.hpp"


/*Constructor*/
Simulation::Simulation(const GamePayoffs& payoffs):
	game_payoffs(payoffs), //use provided payoffs
	strats(payoffs), //strategy evaluation class
	nn_population(), //nullptr array
	nn_game_counts(), //arrays of 0s
	nn_payoff_sums()
{
	//create initial population of NeuralNetworks
	for (int i=0; i<POPULATION_SIZE; ++i) {
		//NNs are initialized with random structures (as specified)
		nn_population[i] = new NeuralNetwork();
	}
}

/*Executes one complete simulation with a certain number of generations*/
void Simulation::run(unsigned int generations)
{
	//reserve array capacity for output data (optimisation)
	population_intelligence.reserve(generations);
	population_fitness.reserve(generations);
	cooperation_frequency.reserve(generations);
	strategies_count.reserve(generations);
	
	//main loop of simulation
	for (unsigned int i=0; i<generations; ++i) {
		presetCounters();
		playGeneration();
		assessPopulation();
		nextGeneration();
	}
	outputResults();
}

/*Resets all generation-specific counters*/
void Simulation::presetCounters()
{
	//population counters
	for (int i=0; i<POPULATION_SIZE; ++i) {
		nn_game_counts[i] = 0;
		nn_payoff_sums[i] = 0;
	}
	total_cooperations = 0;
	total_defections = 0;
	
	//output data
	population_intelligence.emplace_back();
	population_fitness.emplace_back();
	cooperation_frequency.emplace_back();
	strategies_count.emplace_back();
}

/*Plays all individuals from this generation against each other*/
void Simulation::playGeneration()
{
	//Iterate over every possible pair of players from the population
	for (int index_a=0; index_a<POPULATION_SIZE-1; ++index_a) {
		for (int index_b=index_a+1; index_b<POPULATION_SIZE; ++index_b) {
			playEachOther(index_a, index_b);
		}
	}
}

/*Plays two individuals against each other for a number of iterations (or "rounds")*/
void Simulation::playEachOther(int index_a, int index_b)
{
	NeuralNetwork& player_a(*nn_population[index_a]);
	NeuralNetwork& player_b(*nn_population[index_b]);
	
	payoff player_a_payoff, player_b_payoff; //results of each game iteration
	unsigned long player_a_payoff_sum(0), player_b_payoff_sum(0); //sum of all game payoffs
	
	//Play initial iteration (no input)
	bool player_a_cooperates = player_a();
	bool player_b_cooperates = player_b();
	
	//Choose a random number of iterations to play
	int round_iterations = RNG::getIterationCount();
	
	for (int iteration=0; iteration<round_iterations; ++iteration) {
		//count each player's cooperations
		if (player_a_cooperates) total_cooperations += 1;
		else total_defections += 1;
		if (player_b_cooperates) total_cooperations += 1;
		else total_defections += 1;
		
		//gather payoffs from individual's decisions
		game_payoffs.payoffsFromChoices(player_a_cooperates, player_b_cooperates, player_a_payoff, player_b_payoff);
		
		//add payoffs to player's stats
		player_a_payoff_sum += player_a_payoff;
		player_b_payoff_sum += player_b_payoff;
		
		//Play subsequent iterations
		player_a_cooperates = player_a(player_a_payoff, player_b_payoff);
		player_b_cooperates = player_b(player_b_payoff, player_a_payoff);
	}
	
	//Modify the player's counters accordingly
	nn_game_counts[index_a] += round_iterations;
	nn_game_counts[index_b] += round_iterations;
	nn_payoff_sums[index_a] += player_a_payoff_sum;
	nn_payoff_sums[index_b] += player_b_payoff_sum;
}

/*Determines the current population's typical strategies and other metrics*/
void Simulation::assessPopulation()
{
	//references to output arrays corresponding to current generation
	std::array<int, POPULATION_SIZE>& current_intelligence = population_intelligence.back();
	std::array<double, POPULATION_SIZE>& current_fitness = population_fitness.back();
	std::array<int, STRATEGIES_COUNT>& current_strategies = strategies_count.back();
	
	for (int i=0; i<POPULATION_SIZE; ++i) {
		
		//intelligence
		current_intelligence[i] = nn_population[i]->getInnerNodeCount();
		
		//fitness
		current_fitness[i] = (static_cast<double>(nn_payoff_sums[i])/static_cast<double>(nn_game_counts[i])) 
			- (NODE_FITNESS_PENALTY * current_intelligence[i]);
			
		//strategy
		current_strategies[strats.closestPureStrategy(*(nn_population[i]))] += 1;
	}
	//average cooperation frequency
	cooperation_frequency.back()[0] = static_cast<double>(total_cooperations) / static_cast<double>(total_cooperations + total_defections);
}

/*Replaces the current generation by selection based on fitness followed by mutation*/
void Simulation::nextGeneration()
{	
	//select individuals to reproduce with probability proportional to their fitness
	std::array<int, POPULATION_SIZE> new_population_indexes;
	RNG::selectPopulation<POPULATION_SIZE>(population_fitness.back(), new_population_indexes);
	
	//create the new population with the new selection
	NeuralNetwork* new_population[POPULATION_SIZE];
	int selected_index;
	for (int i=0; i<POPULATION_SIZE; ++i) {
		selected_index = new_population_indexes[i]; //index of selected individual
		new_population[i] = new NeuralNetwork(*nn_population[selected_index]); //copy the NN
	}
	
	//replace the old population and mutate the new individuals
	for (int i=0; i<POPULATION_SIZE; ++i) {
		delete nn_population[i]; //delete previous NeuralNetwork
		nn_population[i] = new_population[i]; //copy pointer to new NeuralNetwork
		nn_population[i]->mutate(); //mutate new NeuralNetwork
	}
}

template<typename T, int N>
void printMatrix(const std::vector<std::array<T, N>>& matrix, std::string variable_name)
{
	std::string output = "";
	output += "# name: " + variable_name + "\n";
	output += "# type: matrix\n";
	output += "# rows: " + std::to_string(matrix.size()) + "\n";
	output += "# columns: " + std::to_string(N) + "\n";
	
	for (unsigned int row=0; row<matrix.size(); ++row) {
		for (unsigned int col=0; col<N; ++col) {
			output += std::to_string(matrix[row][col]) + " ";
		}
		output += "\n";
	}
	output += "\n";
	std::cout << output;
}

void printScalar(int value, std::string variable_name)
{
	std::string output = "";
	output += "# name: " + variable_name + "\n";
	output += "# type: scalar\n";
	output += std::to_string(value) + "\n\n";
	std::cout << output;
}

/*Writes the simulation's results to the standard output*/
void Simulation::outputResults()
{
	//Intelligence
	printMatrix<int, POPULATION_SIZE>(population_intelligence, std::string("pop_intelligence"));
	
	//Fitness
	printMatrix<double, POPULATION_SIZE>(population_fitness, std::string("pop_fitness"));
	
	//Average cooperation
	printMatrix<double, 1>(cooperation_frequency, std::string("cooperation_freq"));
	
	//Strategies
	std::cout << "# STRATEGIES are [always defect, always cooperate, tit for tat, tit for two tats, pavlov-like]\n";
	printMatrix<int, STRATEGIES_COUNT>(strategies_count, std::string("strategies_count"));
}
