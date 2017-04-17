#include "Rng.hpp"
#include "RngTest.hpp"
#include "NeuralNetwork.hpp"
#include "NeuralNetworkTest.hpp"
#include "Simulation.hpp"

#include <iostream>
#include <cstring>
#include <ctime>

void runTests(int test_rounds);
void runSimulation(int sim_rounds, std::string game_type);

int main(int argc, char** argv)
{
	//no arguments
	if (argc == 1) {
		std::cerr << "Error: no args provided" << std::endl;
		return 1;
	}
	//test application
	else if (std::string(argv[1]) == "test" and argc == 3) {
		runTests(atoi(argv[2]));
	}
	//run application
	else if (std::string(argv[1]) == "run" and argc <= 5) {
		//set the RNG seed from argument if provided
		if (argc == 5) {
			unsigned seed;
			sscanf(argv[4], "%u", &seed); //parse arg as unsigned
			RNG::setSeed(seed);
		}
		
		//run the simulation
		runSimulation(atoi(argv[2]), std::string(argv[3]));		
	}
	//unknown arguments
	else {
		std::cerr << "Error: unknown options" << std::endl;
		return 1;
	}
	
	return 0;
}

void runTests(int test_rounds)
{
	std::cout << "Running tests..." << std::endl;
	
	for (int round=0; round<test_rounds; ++round) {
		testRng();
		testNeuralNetwork();
	}
	
	std::cout << "All tests passed!" << std::endl;
}

void runSimulation(int sim_rounds, std::string game_type)
{	
	GamePayoffs sim_payoffs{};
	
	//Iterated Prisoner's Game
	if (game_type == "IPD") {
		sim_payoffs.both_cooperate = 6;
		sim_payoffs.both_defect = 2;
		sim_payoffs.self_defects_other_cooperates = 7;
		sim_payoffs.self_cooperates_other_defects = 1;
	}
	//Iterated Snowdrift Game
	else if (game_type == "ISD") {
		sim_payoffs.both_cooperate = 5;
		sim_payoffs.both_defect = 1;
		sim_payoffs.self_defects_other_cooperates = 8;
		sim_payoffs.self_cooperates_other_defects = 2;
	}
	else {
		std::cerr << "Error: unknown game" << std::endl;
		return;
	}
	
	//output simulation details
	std::cout << "# Game: " << game_type << std::endl;
	std::cout << "# Rounds: " << sim_rounds << std::endl;
	
	//output the RNG seed and its randomness for future reference
	std::cout << "# RNG seed: " << RNG::getSeed();
	if (RNG::seedIsRandom())
		std::cout << " (random)" << std::endl << std::endl;
	else
		std::cout << " (provided)" << std::endl << std::endl;
	
	//run and time the simulation
	time_t sim_start = clock();
	
	Simulation sim(sim_payoffs);
	sim.run(sim_rounds);
	
	time_t sim_end = clock();
	std::cout << "# Simulation time: " << static_cast<double>(sim_end - sim_start)/CLOCKS_PER_SEC << std::endl;
}
