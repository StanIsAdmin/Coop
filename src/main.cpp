#include "NeuralNetwork.hpp"
#include "NeuralNetworkTest.hpp"
#include "Simulation.hpp"

#include <iostream>
#include <cstring>

int main(int argc, char** argv)
{
	if (argc == 1) {
		std::cout << "Error: no args provided" << std::endl;
		return 1;
	}
	else if (std::string(argv[1]) == "test" and argc == 2) {
		test();
	}
	else if (std::string(argv[1]) == "run" and argc == 4) {
		unsigned int sim_rounds = atoi(argv[2]);
		std::cout << "Rounds: " << sim_rounds << std::endl;
		
		GamePayoffs sim_payoffs{};
		
		//Iterated Prisoner's Game
		if (std::string(argv[3]) == "IPD") {
			std::cout << "Game: IPD" << std::endl;
			sim_payoffs.both_cooperate = 1;
			sim_payoffs.both_defect = 2;
			sim_payoffs.self_cooperates_other_defects = 3;
			sim_payoffs.self_defects_other_cooperates = 4;
		}
		//Iterated Snowdrift Game
		else if (std::string(argv[3]) == "ISD") {
			std::cout << "Game: ISD" << std::endl;
			sim_payoffs.both_cooperate = 1;
			sim_payoffs.both_defect = 2;
			sim_payoffs.self_cooperates_other_defects = 3;
			sim_payoffs.self_defects_other_cooperates = 4;
		}
		else {
			std::cout << "Error: unknown game" << std::endl;
			return 1;
		}
		
		Simulation sim = Simulation(sim_payoffs);
		sim.run(sim_rounds);
	}
	else {
		std::cout << "Error: unknown options" << std::endl;
		return 1;
	}
	
	return 0;
}
