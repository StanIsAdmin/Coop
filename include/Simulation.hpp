#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <vector>
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
		///Game
		const Payoffs& game_payoffs; //payoffs to use depending on game outcomes
		
		///Strategy evaluation
		Strategies strats; //pure strategy evaluator
		
		///Neural Networks
		NeuralNetwork* nn_population[POPULATION_SIZE]; //dynamically allocated NNs
		
		///NN counters
		unsigned long int nn_game_counts[POPULATION_SIZE]; //number of games played
		unsigned long int nn_payoff_sums[POPULATION_SIZE]; //sum of all game payoffs
		unsigned long int total_defections; //number of defections
		unsigned long int total_cooperations; //number of cooperations
		
		///Population history (output data)
		std::vector<std::array<int, POPULATION_SIZE>> population_intelligence;
		std::vector<std::array<double, POPULATION_SIZE>> population_fitness;
		std::vector<std::array<double, 1>> cooperation_frequency;
		std::vector<std::array<int, STRATEGIES_COUNT>> strategies_count;
		
		///Game (re)initialization
		void presetCounters(); //resets all neural network counters
		
		///Game development
		void playGeneration(); //play all games for the entire generation
		void playEachOther(int playerAIndex, int playerBIndex); //play a number of rounds between two players
		
		///Population assessment
		void assessPopulation(); //generates all required output data from population
		
		///Selection
		void nextGeneration(); //replaces the current generation by the next one
		
		///Simulation output
		void outputResults(); //prints the simulation results
	
	public:
		Simulation(const Payoffs& payoffs);
		
		void run(unsigned int generations); //run the simulation for n generations
};

#endif // SIMULATION_H
