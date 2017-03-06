#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <iostream>
#include <random>

#include "NeuralNetwork.hpp"
#include "Rng.hpp"

#define POPULATION_SIZE 50
#define NODE_FITNESS_PENALTY 0.01

/*Contains the payoffs for each possible outcome of a game*/
struct GamePayoffs {
	payoff both_cooperate = 0;
	payoff both_defect = 0;
	payoff self_cooperates_other_defects = 0;
	payoff self_defects_other_cooperates = 0;
};

/*Creates a population of individuals and runs the simulation steps as defined in the paper*/
class Simulation
{
	private:
		///Randomness
		static RNG rng; //random number generator
		
		///Population
		NeuralNetwork* population[POPULATION_SIZE]; //dynamically allocated NNs
		unsigned long long population_payoff_sum[POPULATION_SIZE]; //sum of all game payoffs
		unsigned long long population_game_count[POPULATION_SIZE]; //number of games played
		
		///Game
		const GamePayoffs& game_payoffs; //payoffs to use depending on game outcomes
		
		//translate individual's decision into payoffs according to game rules
		void payoffsFromChoices(bool playerACoops, bool playerBCoops, payoff& playerAPayoff, payoff& playerBPayoff);
		
		void playGeneration(); //play all games for the entire generation
		void playEachOther(int playerAIndex, int playerBIndex); //play a number of rounds between two players
		
		///Selection
		void nextGeneration(); //replaces the current generation by the next one
	
	public:
		Simulation(const GamePayoffs& payoffs);
		
		void run(unsigned int generations); //run the simulation for n generations
};

#endif // SIMULATION_H
