#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "NeuralNetwork.hpp"
#include "Rng.hpp"
#include "Payoffs.hpp"

#define ASSESSMENT_COUNT 5
#define ASSESSMENT_SIZE 20
#define ASSESSMENT_PROB_STEP 0.25

class Strategies
{
	private:
		RNG rng; //random number generator
		const GamePayoffs& game_payoffs; //payoffs to use depending on game outcomes
	
		///Choices sequences
		bool assessment_choices[ASSESSMENT_COUNT][ASSESSMENT_SIZE]; //random choices
		bool player_choices[ASSESSMENT_COUNT][ASSESSMENT_SIZE]; //assessed player's choices
	
		///Pure strategies move sequences
		bool always_cooperate_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool always_defect_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool tit_for_tat_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool tit_for_two_tats_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool pavlov_like_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		
		void initStrategies(); //initialize choice sequences and pure strategies
		
		std::string compareChoices(); //compares player_choices to each pure strategy
		
	public:
		Strategies(RNG& rng, const GamePayoffs& payoffs);
		
		//returns the player's closest pure strategy
		std::string closestPureStrategy(NeuralNetwork& player);
		
};

#endif //STRATEGIES_H
