#include "Rng.hpp"


/*Static members*/
unsigned RNG::seed = std::random_device()();
bool RNG::seed_is_random = true;
std::mt19937_64 RNG::generator = std::mt19937_64(seed);

//uniform int distribution [0, 1]
std::uniform_int_distribution<int> RNG::distribution_bool = std::uniform_int_distribution<int>(0,1);

//uniform double distribution [0, 1[
std::uniform_real_distribution<double> RNG::distribution_probabilities = std::uniform_real_distribution<double>(0, 1);

//normal distribution with mean NUMVAL_MEAN and standard deviation NUMVAL_STDDEV
std::normal_distribution<double> RNG::distribution_numvals = std::normal_distribution<double>(NUMVAL_MEAN, NUMVAL_STDDEV);

//uniform int distribution [0, MAXINITIALNODES]
std::uniform_int_distribution<int> RNG::distribution_initial_nodes = std::uniform_int_distribution<int>(0,MAXINITIALNODES);

//negative binomial distribution with stop count ROUND_ITERATIONS_STOP_COUNT and probability ROUND_ITERATIONS_MEAN_PROB
std::negative_binomial_distribution<int> RNG::distribution_iterations = std::negative_binomial_distribution<int>(ROUND_ITERATIONS_STOP_COUNT, ROUND_ITERATIONS_MEAN_PROB);

/*If called at all, this function should be called before any of the following functions.*/
void RNG::setSeed(unsigned new_seed) {
	seed = new_seed;
	seed_is_random = false;
	generator.seed(seed);
}

long long int RNG::getSeed() {
	return seed;
}

bool RNG::seedIsRandom() {
	return seed_is_random;
}

bool RNG::getRandomBool() {
	return distribution_bool(generator);
}

bool RNG::getTrueWithProbability(double trueProbability) {
	return trueProbability > distribution_probabilities(generator);
}

/*Returns a random integer in the range [rangeStart, rangeStop] (inclusive)*/
int RNG::getRandomInt(int rangeStart, int rangeStop) {
	std::uniform_int_distribution<int> distribution_int = std::uniform_int_distribution<int>(rangeStart, rangeStop);
	return distribution_int(generator);
}

double RNG::getRandomNumval() {
	return distribution_numvals(generator);
}

int RNG::getInitialNodeCount() {
	return distribution_initial_nodes(generator);
}

int RNG::getIterationCount() {
	int iterations = 1;
	while (distribution_iterations(generator) == 0) ++iterations;
	
	return iterations;
}
