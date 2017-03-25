#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "NeuralNetwork.hpp"
#include "Rng.hpp"

#define ASSESSMENT_COUNT 5
#define ASSESSMENT_SIZE 20
#define ASSESSMENT_PROB_STEP 0.25

class Strategies
{
	private:
		RNG rng;
	
		///Random choices sequences
		double assessment_sequences[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
	
		///Pure strategies move sequences
		bool always_cooperate_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool always_defect_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool tit_for_tat_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool tit_for_two_tats_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool pavlov_like_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		
		void initStrategies(); //initialize choice sequences and pure strategies
		
	public:
		Strategies(RNG& rng);
		
		//returns the player's closest pure strategy
		std::string closestPureStrategy(NeuralNetwork& player);
		
};

#endif //STRATEGIES_H
