#include "Strategies.hpp"


/*Static members*/
RNG Strategies::rng = RNG();

Strategies::Strategies(const GamePayoffs& payoffs) :
	game_payoffs(payoffs),
	all_strat_names({"cooper", "defect", "tittat", "twotat", "pavlov"})
{
	initStrategies();
}


/*
Initializes the sequences of moves that correspond to each pure strategy.
Each sequence of moves is made of 5 series of 20 moves (a move is a decision -cooperate or defect- in a game iteration)
The network's moves will be compared to these moves to determine which is their closest pure strategy.*/
void Strategies::initStrategies()
{
	double coopProb = 0;
	for (int sequence=0; sequence<ASSESSMENT_COUNT; ++sequence){
		
		for (int choice=0; choice<ASSESSMENT_SIZE; ++choice) {
			//The "virtual opponent" used to assess the network chooses to cooperate randomly with probability coopProb
			opponent_choices[sequence][choice] = rng.getTrueWithProbability(coopProb);
			
			//Always cooperate/defect strategies do not depend on the "virtual opponent"'s choice
			always_cooperate_strat[sequence][choice] = true;
			always_defect_strat[sequence][choice] = false;
			
			//Tit-for-tat imitates the opponent's previous decision (first choice is random)
			if (choice == 0) {
				tit_for_tat_strat[sequence][choice] = rng.getTrueWithProbability(coopProb);
			} else {
				tit_for_tat_strat[sequence][choice] = opponent_choices[sequence][choice-1];
			}
			
			//Tit-for-two-tats responds to two sequential defections with a defection, otherwise cooperates
			if (choice <= 1) {
				tit_for_two_tats_strat[sequence][choice] = rng.getTrueWithProbability(coopProb);
			} else {
				bool cooperates = opponent_choices[sequence][choice-1] || opponent_choices[sequence][choice-2];
				tit_for_two_tats_strat[sequence][choice] = cooperates;
			}
			
			//Pavlov-like cooperates when two players previously made the same choice, defects otherwise
			if (choice == 0) {
				pavlov_like_strat[sequence][choice] = rng.getTrueWithProbability(coopProb);
			} else {
				bool cooperates = (opponent_choices[sequence][choice-1] == pavlov_like_strat[sequence][choice-1]);
				pavlov_like_strat[sequence][choice] = cooperates;
			}
		}
		
		coopProb += ASSESSMENT_PROB_STEP;
	}
}

/*Makes the NeuralNetwork play against its virual opponent and returns the its closest pure strategy.*/
std::string Strategies::closestPureStrategy(NeuralNetwork& player)
{
	payoff player_payoff, opponent_payoff; //results of each game iteration
	
	bool player_cooperates;
	bool opponent_cooperates;
	double opponentCoopProb = 0;
	
	for (int sequence=0; sequence<ASSESSMENT_COUNT; ++sequence) {
		//Play initial iteration (no input)
		player_cooperates = player();
		opponent_cooperates = rng.getTrueWithProbability(opponentCoopProb);
		
		for (int iteration=0; iteration<ASSESSMENT_SIZE; ++iteration) {
			//Store the player's choice
			player_choices[sequence][iteration] = player_cooperates;
			
			//Gather payoffs from individual's decisions
			game_payoffs.payoffsFromChoices(player_cooperates, opponent_cooperates, player_payoff, opponent_payoff);
			
			//Play subsequent iterations
			player_cooperates = player(player_payoff, opponent_payoff);
			opponent_cooperates = opponent_choices[sequence][iteration];
		}
		
		opponentCoopProb += ASSESSMENT_PROB_STEP;
	}
	
	return compareChoices();
}

/*
Compares the NeuralNetwork's sequence of choices to the pure strategie's.
Returns the name of the network's closest pure strategy in terms of the least sum of squares.*/
std::string Strategies::compareChoices()
{
	bool (*currStrat)[ASSESSMENT_COUNT][ASSESSMENT_SIZE];	
	int currScore;
	int bestScore = -1;
	int bestScoreStratIndex = -1;
	
	for (int stratIndex=0; stratIndex<STRAT_COUNT; stratIndex++) {
		currStrat = all_strats[stratIndex];
		currScore = 0;
		
		for (int sequence=0; sequence<ASSESSMENT_COUNT; ++sequence) {
			for (int iteration=0; iteration<ASSESSMENT_SIZE; ++iteration) {
				if (player_choices[sequence][iteration] == *currStrat[sequence][iteration]) {
					currScore += 1;
				}
			}
		}
		if (currScore > bestScore) {
			bestScore = currScore;
			bestScoreStratIndex = stratIndex;
		}
	}
	
	return all_strat_names[bestScoreStratIndex];
}