#include "PayoffsTest.hpp"


void testPayoffs()
{
	std::cout << "Testing Payoffs...";
	
	///Gametype payoff getters
	Payoffs ipd_payoffs = Payoffs::getPayoffsForGameType("IPD");
	Payoffs isd_payoffs = Payoffs::getPayoffsForGameType("ISD");
	
	
	///payoff type
	payoff both_cooperate, both_defect, self_cooperates, self_defects;
	
	///IPD values
	ipd_payoffs.payoffsFromChoices(true, true, both_cooperate, both_cooperate);
	ipd_payoffs.payoffsFromChoices(false, false, both_defect, both_defect);
	ipd_payoffs.payoffsFromChoices(true, false, self_cooperates, self_defects);
	assert(both_cooperate == IPD_BOTH_COOPERATE);
	assert(both_defect == IPD_BOTH_DEFECT);
	assert(self_cooperates == IPD_SELF_COOPERATES);
	assert(self_defects == IPD_SELF_DEFECTS);
	
	///IPD values
	isd_payoffs.payoffsFromChoices(true, true, both_cooperate, both_cooperate);
	isd_payoffs.payoffsFromChoices(false, false, both_defect, both_defect);
	isd_payoffs.payoffsFromChoices(true, false, self_cooperates, self_defects);
	assert(both_cooperate == ISD_BOTH_COOPERATE);
	assert(both_defect == ISD_BOTH_DEFECT);
	assert(self_cooperates == ISD_SELF_COOPERATES);
	assert(self_defects == ISD_SELF_DEFECTS);
	
	std::cout << " done!" << std::endl;
}