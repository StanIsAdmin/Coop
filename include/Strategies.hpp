#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <array>

#include "NeuralNetwork.hpp"
#include "Rng.hpp"
#include "Payoffs.hpp"

/*Assessments are games against a virtual opponent*/
#define ASSESSMENT_SIZE 20
#define ASSESSMENT_COUNT 5
#define ASSESSMENT_PREV_CHOICES 2
#define ASSESSMENT_PROB_STEP 0.25

/*List of pure strategies and their unique index*/
#define STRATEGIES_ALWAYS_DEFECT 0
#define STRATEGIES_ALWAYS_COOPERATE 1
#define STRATEGIES_TIT_FOR_TAT 2
#define STRATEGIES_TIT_FOR_TWO_TATS 3
#define STRATEGIES_PAVLOV_LIKE 4
#define STRATEGIES_COUNT 5

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
		//random choices used for assessment
		bool opponent_choices[ASSESSMENT_COUNT][ASSESSMENT_SIZE + ASSESSMENT_PREV_CHOICES];
		
		///Pure strategies and player's average cooperation per assessment
		std::array<std::array<double, ASSESSMENT_COUNT>, STRATEGIES_COUNT> strats_avg_coop;
		std::array<double, ASSESSMENT_COUNT> player_avg_coop;
		
		void initStrategies(); //initialize choice sequences and pure strategies
		
		int compareChoices(); //compares the player's choices to each pure strategy
		
	public:
		Strategies(const GamePayoffs& payoffs);
		
		//returns the player's closest pure strategy
		int closestPureStrategy(NeuralNetwork& player);
		
};

#endif //STRATEGIES_H
