#ifndef SIMULATION_H
#define SIMULATION_H

#include "NeuralNetwork.hpp"

#include <iostream>

#define POPULATION_SIZE 50

class Simulation
{
	private:
		NeuralNetwork* population[POPULATION_SIZE];
		
	public:
		Simulation();
		
		void run(unsigned int rounds); //run the simulation for n rounds
};

#endif // SIMULATION_H
