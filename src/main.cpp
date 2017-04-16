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
	if (argc == 1) {
		std::cerr << "Error: no args provided" << std::endl;
		return 1;
	}
	else if (std::string(argv[1]) == "test" and argc == 3) {
		runTests(atoi(argv[2]));
	}
	else if (std::string(argv[1]) == "run" and argc == 4) {
		runSimulation(atoi(argv[2]), std::string(argv[3]));		
	}
	else {
		std::cerr << "Error: unknown options" << std::endl;
		return 1;
	}
	
	return 0;
}

void runTests(int test_rounds)
{
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
	
	std::cout << "# Game: " << game_type << std::endl;
	std::cout << "# Rounds: " << sim_rounds << std::endl;
	
	time_t sim_start = clock();
	
	Simulation sim(sim_payoffs);
	sim.run(sim_rounds);
	
	time_t sim_end = clock();
	std::cout << "# Simulation time: " << static_cast<double>(sim_end - sim_start)/CLOCKS_PER_SEC << std::endl;
}
