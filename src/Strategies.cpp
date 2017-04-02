#include "Strategies.hpp"


/*Static members*/
RNG Strategies::rng = RNG();

Strategies::Strategies(const GamePayoffs& payoffs) :
	game_payoffs(payoffs)
{
	initStrategies();
}


/*
Initializes the average cooperation per assessment that correspond to each pure strategy.
Each assessment is made of 20 moves (a move is a decision -cooperate or defect- in a game iteration)
The network's moves will be compared to these moves to determine which is their closest pure strategy.*/
void Strategies::initStrategies()
{
	double coopProb = 0;
	bool stratCooperates, prevPavlovChoice;
	
	for (int sequence=0; sequence<ASSESSMENT_COUNT; ++sequence){
		always_cooperate_avg[sequence] = 0;
		always_defect_avg[sequence] = 0;
		tit_for_tat_avg[sequence] = 0;
		tit_for_two_tats_avg[sequence] = 0;
		pavlov_like_avg[sequence] = 0;
		
		for (int choice=0; choice<ASSESSMENT_SIZE; ++choice) {
			//The "virtual opponent" used to assess the network chooses to cooperate randomly with probability coopProb
			opponent_choices[sequence][choice] = rng.getTrueWithProbability(coopProb);
			
			//Always cooperate/defect strategies do not depend on the "virtual opponent"'s choice
			always_cooperate_avg[sequence] += 1;
			
			//Tit-for-tat imitates the opponent's previous decision (first choice is random)
			if (choice == 0) {
				stratCooperates = rng.getTrueWithProbability(coopProb);
			} else {
				stratCooperates = opponent_choices[sequence][choice-1];
			}
			if (stratCooperates) tit_for_tat_avg[sequence] += 1;
			
			//Tit-for-two-tats responds to two sequential defections with a defection, otherwise cooperates
			if (choice <= 1) {
				stratCooperates = rng.getTrueWithProbability(coopProb);
			} else {
				stratCooperates = opponent_choices[sequence][choice-1] || opponent_choices[sequence][choice-2];
			}
			if (stratCooperates) tit_for_two_tats_avg[sequence] += 1;
			
			//Pavlov-like cooperates when two players previously made the same choice, defects otherwise
			if (choice == 0) {
				stratCooperates = rng.getTrueWithProbability(coopProb);
			} else {
				stratCooperates = (opponent_choices[sequence][choice-1] == prevPavlovChoice);
			}
			if (stratCooperates) pavlov_like_avg[sequence] += 1;
			prevPavlovChoice = stratCooperates;
		}
		
		//Transform cooperation counts into averages
		always_cooperate_avg[sequence] /= ASSESSMENT_SIZE;
		always_defect_avg[sequence] /= ASSESSMENT_SIZE;
		tit_for_tat_avg[sequence] /= ASSESSMENT_SIZE;
		tit_for_two_tats_avg[sequence] /= ASSESSMENT_SIZE;
		pavlov_like_avg[sequence] /= ASSESSMENT_SIZE;
		
		coopProb += ASSESSMENT_PROB_STEP; //increase virtual opponent's cooperation probability
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
		player_avg[sequence] = 0;
		
		for (int iteration=0; iteration<ASSESSMENT_SIZE; ++iteration) {
			//Increase the player's cooperation count
			if (player_cooperates) player_avg[sequence] += 1;
			
			//Gather payoffs from individual's decisions
			game_payoffs.payoffsFromChoices(player_cooperates, opponent_cooperates, player_payoff, opponent_payoff);
			
			//Play subsequent iterations
			player_cooperates = player(player_payoff, opponent_payoff);
			opponent_cooperates = opponent_choices[sequence][iteration];
		}
		player_avg[sequence] /= ASSESSMENT_SIZE; //transform cooperation count into average
		opponentCoopProb += ASSESSMENT_PROB_STEP;
	}
	
	return compareChoices();
}

/*
Compares the NeuralNetwork's sequence of choices to the pure strategie's.
Returns the name of the network's closest pure strategy in terms of the least sum of squares.*/
std::string Strategies::compareChoices()
{
	double currScore; //score of similarity between player and current "pure strategy"
	double bestScore = 100000; //huge score
	int bestStratIndex = -1; //index of best strat 
	
	for (int stratIndex=0; stratIndex<STRAT_COUNT; stratIndex++) {
		std::array<double, ASSESSMENT_COUNT>& currStrat = *all_avg[stratIndex]; //current "pure strategy"
		currScore = 0;
		
		for (int sequence=0; sequence<ASSESSMENT_COUNT; ++sequence) {
			currScore += pow(currStrat[sequence] - player_avg[sequence], 2);
		}
		
		if (currScore < bestScore) {
			bestScore = currScore;
			bestStratIndex = stratIndex;
		}
	}
	return all_strat_names[bestStratIndex];
}