#include "RngTest.hpp"

void testRng()
{
	std::cout << "Testing Rng...";
	
	///Different number generation
	RNG rng = RNG();
	RNG rng2 = RNG();
	assert(rng.getRandomNumval() != rng2.getRandomNumval());
	
	std::cout << " done!" << std::endl;
}