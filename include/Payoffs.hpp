#ifndef PAYOFFS_H
#define PAYOFFS_H

typedef unsigned short payoff;

/*Contains the payoffs for each possible outcome of a game*/
struct GamePayoffs {
	payoff both_cooperate = 0;
	payoff both_defect = 0;
	payoff self_cooperates_other_defects = 0;
	payoff self_defects_other_cooperates = 0;
	
	//Assigns payoffs to both players depending on their choices and the game rules
	void payoffsFromChoices(bool a_cooperates, bool b_cooperates, payoff& a_payoff, payoff& b_payoff) const;
};

#endif // PAYOFFS_H
