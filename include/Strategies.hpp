#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <array>

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
		
		///Pure strategies and player's average cooperation per assessment
		std::array<double, ASSESSMENT_COUNT> always_cooperate_avg;
		std::array<double, ASSESSMENT_COUNT> always_defect_avg;
		std::array<double, ASSESSMENT_COUNT> tit_for_tat_avg;
		std::array<double, ASSESSMENT_COUNT> tit_for_two_tats_avg;
		std::array<double, ASSESSMENT_COUNT> pavlov_like_avg;
		std::array<double, ASSESSMENT_COUNT> player_avg;
		
		//array of pointers to pure strategie's average cooperations
		std::array<std::array<double, ASSESSMENT_COUNT>*, STRAT_COUNT> all_avg = {{{&always_cooperate_avg}, {&always_defect_avg}, {&tit_for_tat_avg}, {&tit_for_two_tats_avg}, {&pavlov_like_avg}}};
		
		//array of names of each pure strategy
		const std::array<std::string, STRAT_COUNT> all_strat_names = {{"cooper", "defect", "tittat", "twotat", "pavlov"}};
		
		void initStrategies(); //initialize choice sequences and pure strategies
		
		std::string compareChoices(); //compares the player's choices to each pure strategy
		
	public:
		Strategies(const GamePayoffs& payoffs);
		
		//returns the player's closest pure strategy
		std::string closestPureStrategy(NeuralNetwork& player);
		
};

#endif //STRATEGIES_H
