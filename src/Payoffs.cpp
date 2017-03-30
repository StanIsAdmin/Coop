#include "Payoffs.hpp"


/*Assigns payoffs to both players depending on their choices and the game rules*/
void GamePayoffs::payoffsFromChoices(bool playerACoops, bool playerBCoops, payoff& playerAPayoff, payoff& playerBPayoff) const
{
	//both cooperate
	if (playerACoops && playerBCoops)  {
		playerAPayoff = both_cooperate;
		playerBPayoff = both_cooperate;
	}
	//A cooperates, B defects
	else if (playerACoops && !playerBCoops) {
		playerAPayoff = self_cooperates_other_defects;
		playerBPayoff = self_defects_other_cooperates;
	}
	//A defects, B cooperates
	else if (!playerACoops && playerBCoops) {
		playerAPayoff = self_defects_other_cooperates;
		playerBPayoff = self_cooperates_other_defects;
	}
	//both defect
	else {
		playerAPayoff = both_defect;
		playerBPayoff = both_defect;
	}
}