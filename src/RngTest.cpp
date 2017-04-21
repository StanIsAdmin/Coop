#include "RngTest.hpp"

void testRng()
{
	std::cout << "Testing Rng...";
	
	///Random boolean generation
	int true_count = 0;
	for (int i=0; i<1000; ++i) {
		if (RNG::getRandomBool()) true_count++;
	}
	assert(abs(true_count - 500) < 50);
	
	std::cout << " done!" << std::endl;
	
	///Iteration count generation
	int iterations = 0;
	for (int i=0; i<1000; ++i) {
		iterations += RNG::getIterationCount();
	}
	assert(abs(iterations - 50000) < 5000);
	
	///Numval generation
	double average_numval = 0;
	for (int i=0; i<1000; ++i) {
		average_numval += RNG::getRandomNumval();
	}
	average_numval /= 1000;
	if (average_numval < 0) average_numval *= -1;
	assert(average_numval < 0.1);
	
	///Population selection
	std::array<double, 1000> fitness;
	for (int i=0; i<1000; ++i) {
		if (i < 500) fitness[i] = 0.1;
		else fitness[i] = 0.9;
	}
	
	std::array<int, 1000> new_pop;
	RNG::selectPopulation(fitness, new_pop);
	int lower_pop(0), upper_pop(0);
	for (int i=0; i<1000; ++i) {
		if (new_pop[i] > 500) upper_pop++;
		else lower_pop++;
	}
	
	std::cout << upper_pop << ", " << lower_pop << std::endl;
	assert(upper_pop > (7 * lower_pop) and upper_pop < (11 * lower_pop));
}