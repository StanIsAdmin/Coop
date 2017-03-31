#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "NeuralNetwork.hpp"
#include "Rng.hpp"
#include "Payoffs.hpp"

#define ASSESSMENT_COUNT 5
#define ASSESSMENT_SIZE 20
#define ASSESSMENT_PROB_STEP 0.25
#define STRAT_COUNT 5


/*
Defines multiple decision patterns called "strategies", which allow us to categorize NeuralNetworks.
To assess a NeuralNetwork's strategy, the class defines a fixed sequence of randomly chosen choices
that will be used as a "virtual opponent" for the NeuralNetwork.
It then defines what "pure strategies" would choose (in terms of collaborations/defections) 
if they faced this exact "virtual opponent". These strategies are the following:
- "always cooperate" always cooperates
- "always defect" always defects
- "tit for tat" imitates the opponent's previous decision (first choice is random)
- "tit for two tats" responds to two sequential defections with a defection, otherwise cooperates
- "pavlov-like" cooperates when two players previously made the same choice, defects otherwise

To assess a NeuralNetwork, it makes it play against the same "virtual opponent" and stores its
move sequence, then compares it to the pure strategie's move sequences to find which one is closest.*/
class Strategies
{
	private:
		static RNG rng; //random number generator
		const GamePayoffs& game_payoffs; //payoffs to use depending on game outcomes
	
		///Choices sequences
		bool opponent_choices[ASSESSMENT_COUNT][ASSESSMENT_SIZE]; //random choices used for assessment
		bool player_choices[ASSESSMENT_COUNT][ASSESSMENT_SIZE]; //assessed NeuralNetwork's choices
	
		///Pure strategies move sequences
		bool always_cooperate_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool always_defect_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool tit_for_tat_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool tit_for_two_tats_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		bool pavlov_like_strat[ASSESSMENT_COUNT][ASSESSMENT_SIZE];
		
		//array of all move sequences
		bool (*all_strats[STRAT_COUNT])[ASSESSMENT_COUNT][ASSESSMENT_SIZE] = {&always_cooperate_strat, &always_defect_strat, &tit_for_tat_strat, &tit_for_two_tats_strat, &pavlov_like_strat};
		
		//name of each strategy
		std::string all_strat_names[STRAT_COUNT];
		
		void initStrategies(); //initialize choice sequences and pure strategies
		
		std::string compareChoices(); //compares player_choices to each pure strategy
		
	public:
		Strategies(const GamePayoffs& payoffs);
		
		//returns the player's closest pure strategy
		std::string closestPureStrategy(NeuralNetwork& player);
		
};

#endif //STRATEGIES_H
