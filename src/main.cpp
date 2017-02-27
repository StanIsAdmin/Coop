#include "NeuralNetwork.hpp"
#include "NeuralNetworkTest.hpp"
#include "Simulation.hpp"

#include <iostream>
#include <cassert>

#define SIMULATION_ROUNDS 1000

int main()
{
	test();
	
	Simulation sim = Simulation();
	sim.run(SIMULATION_ROUNDS);
	
	return 0;
}
