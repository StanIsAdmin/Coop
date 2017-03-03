#ifndef SIMULATION_H
#define SIMULATION_H

#include "NeuralNetwork.hpp"

#include <iostream>

#define POPULATION_SIZE 50

struct GamePayoffs {
	payoff both_collaborate = 0;
	payoff both_defect = 0;
	payoff self_collaborates_other_defects = 0;
	payoff self_defects_other_collaborates = 0;
};

class Simulation
{
	private:
		NeuralNetwork* population[POPULATION_SIZE];
		const GamePayoffs& payoffs;
		
		void payoffsFromChoices(bool playerACoops, bool playerBCoops, payoff& playerAPayoff, payoff& playerBPayoff);
		void playGeneration();
		void playRound(int playerAIndex, int playerBIndex);
		
	public:
		Simulation(const GamePayoffs& payoffs);
		
		//run the simulation for n generations
		void run(unsigned int generations);
};

#endif // SIMULATION_H
