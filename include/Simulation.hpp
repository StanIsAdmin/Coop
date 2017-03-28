#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <iostream>
#include <map>

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
		unsigned long long population_payoff_sum[POPULATION_SIZE]; //sum of all game payoffs
		unsigned long long population_game_count[POPULATION_SIZE]; //number of games played
		std::string population_strategies[POPULATION_SIZE]; //closest pure strategies
		
		///Game
		const GamePayoffs& game_payoffs; //payoffs to use depending on game outcomes
		
		void playGeneration(); //play all games for the entire generation
		void playEachOther(int playerAIndex, int playerBIndex); //play a number of rounds between two players
		
		///Selection
		void nextGeneration(); //replaces the current generation by the next one
		
		///Population assessment
		void assessPopulation(unsigned int generation); //assigns to each network its closest pure strategy
	
	public:
		Simulation(const GamePayoffs& payoffs);
		
		void run(unsigned int generations); //run the simulation for n generations
};

#endif // SIMULATION_H
