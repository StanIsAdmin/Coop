#include "Simulation.hpp"


Simulation::Simulation():
	population()
{
	for (int i=0; i<POPULATION_SIZE; ++i) {
		population[i] = new NeuralNetwork();
	}
}

void Simulation::run(unsigned int rounds)
{
	for (unsigned int round_count=0; round_count<rounds; ++round_count) {
		population[0]->mutate();
	}
}