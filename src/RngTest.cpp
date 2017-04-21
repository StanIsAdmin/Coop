#include "RngTest.hpp"


void testRng()
{
	std::cout << "Testing Rng...";
	
	///Random booleans
	double true_freq = 0;
	for (int i=0; i<SAMPLE_SIZE; ++i) {
		if (RNG::getRandomBool()) true_freq += 1;
	}
	true_freq /= SAMPLE_SIZE;
	assert(fabs(true_freq - BOOL_GOAL) < BOOL_DIFF);
	
	///True with given probability
	true_freq = 0;
	for (int i=0; i<SAMPLE_SIZE; ++i) {
		if (RNG::getTrueWithProbability(TRUE_PROB)) true_freq += 1;
	}
	true_freq /= SAMPLE_SIZE;
	assert(fabs(true_freq - TRUE_PROB) < TRUE_DIFF);
	
	///Iteration count
	double avg_iterations = 0;
	for (int i=0; i<SAMPLE_SIZE; ++i) {
		avg_iterations += RNG::getIterationCount();
	}
	avg_iterations /= SAMPLE_SIZE;
	assert(fabs(avg_iterations - ITERATIONS_GOAL) < ITERATIONS_DIFF);
	
	///Numeric values
	double avg_numval = 0;
	for (int i=0; i<SAMPLE_SIZE; ++i) {
		avg_numval += RNG::getRandomNumval();
	}
	avg_numval /= SAMPLE_SIZE;
	assert(fabs(avg_numval - NUMVAL_GOAL) < NUMVAL_DIFF);
	
	///Population selection
	std::array<double, SAMPLE_SIZE> fitness;
	std::array<int, SAMPLE_SIZE> selected_pop;
	int lower_pop(0), upper_pop(0);
	for (int i=0; i<SAMPLE_SIZE; ++i) {
		if (i < (SAMPLE_SIZE/2)) fitness[i] = FITNESS_LOW;
		else fitness[i] = FITNESS_HIGH;
	}
	RNG::selectPopulation(fitness, selected_pop);
	for (int i=0; i<SAMPLE_SIZE; ++i) {
		if (selected_pop[i] > (SAMPLE_SIZE/2)) upper_pop++;
		else lower_pop++;
	}
	assert(upper_pop > (FITNESS_LOW_FACTOR * lower_pop) and upper_pop < (FITNESS_HIGH_FACTOR * lower_pop));
	
	std::cout << " done!" << std::endl;
}