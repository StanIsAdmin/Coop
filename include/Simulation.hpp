#ifndef SIMULATION_H
#define SIMULATION_H

#include "NeuralNetwork.hpp"

#include <array>
#include <iostream>

#define POPULATION_SIZE 50
#define ROUND_ITERATIONS_STOP_COUNT 1
#define ROUND_ITERATIONS_MEAN_PROB 0.98
#define NODE_FITNESS_PENALTY 0.01

struct GamePayoffs {
	payoff both_collaborate = 0;
	payoff both_defect = 0;
	payoff self_collaborates_other_defects = 0;
	payoff self_defects_other_collaborates = 0;
};

class Simulation
{
	private:
		///Randomness
		static std::random_device generator; //random number generator TODO: check efficiency
		//number of iterations per round
		static std::negative_binomial_distribution<int> distribution_iterations;
		
		NeuralNetwork* population[POPULATION_SIZE];
		unsigned long long population_payoff_sum[POPULATION_SIZE];
		unsigned long long population_game_count[POPULATION_SIZE];
		
		const GamePayoffs& game_payoffs;
		
		//translate individual's decision into payoffs according to game rules
		void payoffsFromChoices(bool playerACoops, bool playerBCoops, payoff& playerAPayoff, payoff& playerBPayoff);
		
		void playGeneration(); //play all games for the entire generation
		void playRound(int playerAIndex, int playerBIndex); //play a full round between two players
		
		void nextGeneration(); //replaces the current generation by the next one
	public:
		Simulation(const GamePayoffs& payoffs);
		
		//run the simulation for n generations
		void run(unsigned int generations);
};

#endif // SIMULATION_H
