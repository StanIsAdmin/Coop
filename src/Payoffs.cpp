#include "Payoffs.hpp"


/*Assigns payoffs to both players depending on their choices and the game rules*/
void GamePayoffs::payoffsFromChoices(bool a_cooperates, bool b_cooperates, payoff& a_payoff, payoff& b_payoff) const
{
	//both cooperate
	if (a_cooperates && b_cooperates)  {
		a_payoff = both_cooperate;
		b_payoff = both_cooperate;
	}
	//A cooperates, B defects
	else if (a_cooperates && !b_cooperates) {
		a_payoff = self_cooperates_other_defects;
		b_payoff = self_defects_other_cooperates;
	}
	//A defects, B cooperates
	else if (!a_cooperates && b_cooperates) {
		a_payoff = self_defects_other_cooperates;
		b_payoff = self_cooperates_other_defects;
	}
	//both defect
	else {
		a_payoff = both_defect;
		b_payoff = both_defect;
	}
}