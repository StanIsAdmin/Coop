#include "RngTest.hpp"

void testRng()
{
	std::cout << "Testing Rng...";
	
	///Static seed and generator
	RNG rng = RNG();
	RNG rng2 = RNG();
	assert(rng.getRandomNumval() != rng2.getRandomNumval());
	
	rng.setSeed(2);
	assert(rng2.getSeed() == 2);
	
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
}