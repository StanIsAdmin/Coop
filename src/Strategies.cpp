#include "Strategies.hpp"


Strategies::Strategies(RNG& rng) :
	rng(rng)
{
	initStrategies();
}

void Strategies::initStrategies()
{
	double coopProb = 0;
	for (int sequence=0; sequence<ASSESSMENT_COUNT; ++sequence){
		coopProb += ASSESSMENT_PROB_STEP;
		
		for (int choice=0; choice<ASSESSMENT_SIZE; ++choice) {
			assessment_sequences[sequence][choice] = rng.getTrueWithProbability(coopProb);
			
			//Always cooperate/defect
			always_cooperate_strat[sequence][choice] = true;
			always_defect_strat[sequence][choice] = false;
			
			//Tit-for-tat imitates the opponent's previous decision (first choice is random)
			if (choice == 0) {
				tit_for_tat_strat[sequence][choice] = rng.getTrueWithProbability(coopProb);
			} else {
				tit_for_tat_strat[sequence][choice] = assessment_sequences[sequence][choice-1];
			}
			
			//Tit-for-two-tats responds to two sequential defections with a defection, otherwise cooperates
			if (choice <= 1) {
				tit_for_two_tats_strat[sequence][choice] = rng.getTrueWithProbability(coopProb);
			} else {
				bool cooperates = assessment_sequences[sequence][choice-1] || assessment_sequences[sequence][choice-2];
				tit_for_two_tats_strat[sequence][choice] = cooperates;
			}
			
			//Pavlov-like cooperates when two players previously made the same choice, defects otherwise
			if (choice == 0) {
				pavlov_like_strat[sequence][choice] = rng.getTrueWithProbability(coopProb);
			} else {
				bool cooperates = (assessment_sequences[sequence][choice-1] == pavlov_like_strat[sequence][choice-1]);
				pavlov_like_strat[sequence][choice] = cooperates;
			}
		}
	}
}

/*Returns the player's closest pure strategy*/
std::string Strategies::closestPureStrategy(NeuralNetwork& player)
{
	return "none";
	/*
	NeuralNetwork& player(*population[playerIndex]);
	
	payoff player_payoff, opponent_payoff; //results of each game iteration
	
	//Play initial iteration (no input)
	bool player_cooperates = player();
	bool opponent_cooperates = rng.getTrueWithProbability(opponentCoopProb);
	
	for (int iteration=0; iteration<ASSESSMENT_SIZE; ++iteration) {
		//Gather payoffs from individual's decisions
		payoffsFromChoices(player_cooperates, opponent_cooperates, player_payoff, opponent_payoff);
		
		//Play subsequent iterations
		player_cooperates = player(player_payoff, opponent_payoff);
		opponent_cooperates = rng.getTrueWithProbability(opponentCoopProb);
	}*/
}