#include "Payoffs.hpp"


/*Assigns payoffs to both players depending on their choices and the game rules*/
void Payoffs::payoffsFromChoices(bool a_cooperates, bool b_cooperates, payoff& a_payoff, payoff& b_payoff) const
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

Payoffs Payoffs::getPayoffsForGameType(std::string game_type)
{
	Payoffs game_payoffs = {};
	
	//Iterated Prisoner's Game
	if (game_type == "IPD") {
		game_payoffs.both_cooperate = IPD_BOTH_COOPERATE;
		game_payoffs.both_defect = IPD_BOTH_DEFECT;
		game_payoffs.self_defects_other_cooperates = IPD_SELF_DEFECTS;
		game_payoffs.self_cooperates_other_defects = IPD_SELF_COOPERATES;
	}
	
	//Iterated Snowdrift Game
	else if (game_type == "ISD") {
		game_payoffs.both_cooperate = ISD_BOTH_COOPERATE;
		game_payoffs.both_defect = ISD_BOTH_DEFECT;
		game_payoffs.self_defects_other_cooperates = ISD_SELF_DEFECTS;
		game_payoffs.self_cooperates_other_defects = ISD_SELF_COOPERATES;
	}
	
	else {
		throw std::invalid_argument("GamePayoffs: unknown game type");
	}
	
	return game_payoffs;
}