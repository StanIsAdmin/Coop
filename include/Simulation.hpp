#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <iostream>

#include "NeuralNetwork.hpp"
#include "Rng.hpp"
#include "Strategies.hpp"
#include "Payoffs.hpp"

#define POPULATION_SIZE 50
#define NODE_FITNESS_PENALTY 0.01


/*Creates a population of individuals and runs the simulation steps as defined in the paper*/
class Simulation
{
	private:
		///Randomness
		static RNG rng; //random number generator
		
		///Strategy evaluation
		Strategies strats;
		
		///Population
		NeuralNetwork* population[POPULATION_SIZE]; //dynamically allocated NNs
		unsigned long int population_payoff_sum[POPULATION_SIZE]; //sum of all game payoffs
		unsigned long int population_game_count[POPULATION_SIZE]; //number of games played
		unsigned long int total_defections = 0;
		unsigned long int total_cooperations = 0;
		std::array<double, POPULATION_SIZE> population_fitness;
		unsigned int population_strategies[STRATEGIES_COUNT]; //closest pure strategies
		
		///Output
		std::string pop_intelligence = "";
		std::string pop_fitness = "";
		std::string avg_cooperation = "";
		std::array<std::string, STRATEGIES_COUNT> strategies_counts;
		
		//strategy names, in same order as defined in "Strategies.hpp"
		const std::array<std::string, STRATEGIES_COUNT> strategies_names = {{"cooper", "defect", "tittat", "twotat", "pavlov"}}; 
		
		///Game
		const GamePayoffs& game_payoffs; //payoffs to use depending on game outcomes
		
		void playGeneration(); //play all games for the entire generation
		void playEachOther(int playerAIndex, int playerBIndex); //play a number of rounds between two players
		
		///Population assessment
		void evaluatePopulationFitness();
		void assessPopulation(); //assigns to each network its closest pure strategy
		
		///Selection
		void nextGeneration(); //replaces the current generation by the next one
		
		///Simulation output
		void outputResults(unsigned int generations); //prints the simulation results
	
	public:
		Simulation(const GamePayoffs& payoffs);
		
		void run(unsigned int generations); //run the simulation for n generations
};

#endif // SIMULATION_H
