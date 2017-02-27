#include "Simulation.hpp"


Simulation::Simulation(const GamePayoffs& payoffs):
	population(),
	payoffs(payoffs)
{
	for (int i=0; i<POPULATION_SIZE; ++i) {
		population[i] = new NeuralNetwork();
	}
}

void Simulation::run(unsigned int rounds)
{
	std::cout << "Running simulation..." << std::endl;
	for (unsigned int round_count=0; round_count<rounds; ++round_count) {
		population[0]->mutate();
	}
	std::cout << "Simulation finished!" << std::endl;
}