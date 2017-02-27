#ifndef SIMULATION_H
#define SIMULATION_H

#include "NeuralNetwork.hpp"

#include <iostream>

#define POPULATION_SIZE 50

struct GamePayoffs {
	int both_collaborate = 0;
	int both_defect = 0;
	int self_collaborates_other_defects = 0;
	int self_defects_other_collaborates = 0;
};

class Simulation
{
	private:
		NeuralNetwork* population[POPULATION_SIZE];
		const GamePayoffs& payoffs;
		
	public:
		Simulation(const GamePayoffs& payoffs);
		
		//run the simulation for n rounds
		void run(unsigned int rounds);
};

#endif // SIMULATION_H
