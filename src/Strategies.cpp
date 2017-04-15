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
	double coop_prob = 0;
	bool strat_cooperates, prev_pavlov_choice;
	
	for (int assessment_index=0; assessment_index<ASSESSMENT_COUNT; ++assessment_index){
		//Initialize cooperation counts
		for (int strat_index=0; strat_index<STRATEGIES_COUNT; ++strat_index) {
			strats_avg_coop[strat_index][assessment_index] = 0;
		}
		
		//Initialize previous choices for virtual opponent
		for (int prev_choice_index=0; prev_choice_index<ASSESSMENT_PREV_CHOICES; ++prev_choice_index) {
			opponent_choices[assessment_index][prev_choice_index] = rng.getTrueWithProbability(coop_prob);
		}
		prev_pavlov_choice = rng.getTrueWithProbability(coop_prob); //Init previous choice for pavlov strategy
		
		for (int iteration=ASSESSMENT_PREV_CHOICES; iteration<ASSESSMENT_SIZE + ASSESSMENT_PREV_CHOICES; ++iteration) {
			//The "virtual opponent" used to assess the network chooses to cooperate randomly with probability coop_prob
			opponent_choices[assessment_index][iteration] = rng.getTrueWithProbability(coop_prob);
			
			//Always cooperate/defect strategies do not depend on the "virtual opponent"'s choice
			strats_avg_coop[STRATEGIES_ALWAYS_COOPERATE][assessment_index] += 1;
			
			//Tit-for-tat imitates the opponent's previous decision (first choice is random)
			strat_cooperates = opponent_choices[assessment_index][iteration-1];
			if (strat_cooperates) strats_avg_coop[STRATEGIES_TIT_FOR_TAT][assessment_index] += 1;
			
			//Tit-for-two-tats responds to two sequential defections with a defection, otherwise cooperates
			strat_cooperates = opponent_choices[assessment_index][iteration-1] || opponent_choices[assessment_index][iteration-2];
			if (strat_cooperates) strats_avg_coop[STRATEGIES_TIT_FOR_TWO_TATS][assessment_index] += 1;
			
			//Pavlov-like cooperates when two players previously made the same choice, defects otherwise
			strat_cooperates = (opponent_choices[assessment_index][iteration-1] == prev_pavlov_choice);
			if (strat_cooperates) strats_avg_coop[STRATEGIES_PAVLOV_LIKE][assessment_index] += 1;
			prev_pavlov_choice = strat_cooperates;
		}
		
		//Transform cooperation counts into averages
		strats_avg_coop[STRATEGIES_ALWAYS_COOPERATE][assessment_index] /= ASSESSMENT_SIZE;
		strats_avg_coop[STRATEGIES_TIT_FOR_TAT][assessment_index] /= ASSESSMENT_SIZE;
		strats_avg_coop[STRATEGIES_TIT_FOR_TWO_TATS][assessment_index] /= ASSESSMENT_SIZE;
		strats_avg_coop[STRATEGIES_PAVLOV_LIKE][assessment_index] /= ASSESSMENT_SIZE;
		
		coop_prob += ASSESSMENT_PROB_STEP; //increase virtual opponent's cooperation probability
	}
}

/*Makes the NeuralNetwork play against its virual opponent and returns the its closest pure strategy.*/
int Strategies::closestPureStrategy(NeuralNetwork& player)
{
	payoff player_payoff, opponent_payoff; //results of each game iteration
	
	bool player_cooperates, opponent_cooperates;
	
	for (int assessment_index=0; assessment_index<ASSESSMENT_COUNT; ++assessment_index) {
		//Initialize cooperation counts
		player_avg_coop[assessment_index] = 0;
		
		//Play initial iteration (no input)
		player_cooperates = player();
		opponent_cooperates = opponent_choices[assessment_index][ASSESSMENT_PREV_CHOICES - 1];
		
		for (int iteration=ASSESSMENT_PREV_CHOICES; iteration<ASSESSMENT_SIZE + ASSESSMENT_PREV_CHOICES; ++iteration) {
			//Increase the player's cooperation count
			if (player_cooperates) player_avg_coop[assessment_index] += 1;
			
			//Gather payoffs from individual's decisions
			game_payoffs.payoffsFromChoices(player_cooperates, opponent_cooperates, player_payoff, opponent_payoff);
			
			//Play subsequent iterations
			player_cooperates = player(player_payoff, opponent_payoff);
			opponent_cooperates = opponent_choices[assessment_index][iteration];
		}
		player_avg_coop[assessment_index] /= ASSESSMENT_SIZE; //transform cooperation count into average
	}
	
	return compareChoices();
}

/*
Compares the NeuralNetwork's sequence of choices to the pure strategie's.
Returns the name of the network's closest pure strategy in terms of the least sum of squares.*/
int Strategies::compareChoices()
{
	double current_score; //score of similarity between player and current "pure strategy"
	double best_score = -1; //score for closest pure strategy found so far
	int best_strat_index = -1; //index of best strat found so far
	
	for (int strat_index=0; strat_index<STRATEGIES_COUNT; strat_index++) {
		std::array<double, ASSESSMENT_COUNT>& currStrat = strats_avg_coop[strat_index]; //current "pure strategy"
		current_score = 0;
		
		for (int assessment_index=0; assessment_index<ASSESSMENT_COUNT; ++assessment_index) {
			current_score += pow(currStrat[assessment_index] - player_avg_coop[assessment_index], 2);
		}
		
		if (current_score < best_score or best_score < 0) {
			best_score = current_score;
			best_strat_index = strat_index;
		}
	}
	return best_strat_index;
}