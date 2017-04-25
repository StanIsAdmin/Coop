#ifndef PAYOFFS_H
#define PAYOFFS_H

#include <stdexcept>
#include <string>

/*IPD Payoffs*/
#define IPD_BOTH_COOPERATE 6
#define IPD_BOTH_DEFECT 2
#define IPD_SELF_COOPERATES 7
#define IPD_SELF_DEFECTS 1

/*ISD Payoffs*/
#define ISD_BOTH_COOPERATE 5
#define ISD_BOTH_DEFECT 1
#define ISD_SELF_COOPERATES 8
#define ISD_SELF_DEFECTS 2

typedef unsigned short payoff;

/*Contains the payoffs for each possible outcome of a game*/
class Payoffs {
	private:
		payoff both_cooperate = 0;
		payoff both_defect = 0;
		payoff self_cooperates_other_defects = 0;
		payoff self_defects_other_cooperates = 0;
	
	public:
		//Assigns payoffs to both players depending on their choices and the game rules
		void payoffsFromChoices(bool a_cooperates, bool b_cooperates, payoff& a_payoff, payoff& b_payoff) const;
		static Payoffs getPayoffsForGameType(std::string game_type);
};

#endif // PAYOFFS_H
