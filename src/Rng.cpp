#include "Rng.hpp"


/*Static members*/
#ifndef SEED
long long int RNG::seed = std::random_device()();
bool RNG::seed_is_random = true;
#else
long long int RNG::seed = SEED;
bool RNG::seed_is_random = false;
#endif
std::mt19937_64 RNG::generator = std::mt19937_64(seed);

//Game iterations
std::negative_binomial_distribution<int> RNG::distribution_iterations = std::negative_binomial_distribution<int>(ROUND_ITERATIONS_STOP_COUNT, ROUND_ITERATIONS_MEAN_PROB);

//Binary choice
std::uniform_int_distribution<int> RNG::distribution_bool = std::uniform_int_distribution<int>(0,1);

//Number of initial nodes
std::uniform_int_distribution<int> RNG::distribution_initial_nodes = std::uniform_int_distribution<int>(0,MAXINITIALNODES);

//Real values
std::normal_distribution<double> RNG::distribution_real_values = std::normal_distribution<double>(NUMVAL_MEAN, NUMVAL_STDDEV);

//Probabilities (0 is included, 1 is not)
std::uniform_real_distribution<double> RNG::distribution_prob_values = std::uniform_real_distribution<double>(0, 1);

long long int RNG::getSeed() {
	return seed;
}

bool RNG::seedIsRandom() {
	return seed_is_random;
}

int RNG::getIterationCount() {
	int iterations = 1;
	double continueProb = 1 - distribution_iterations(generator);
	while (continueProb) {
		iterations++;
		continueProb = 1 - distribution_iterations(generator);
	}
	
	return iterations;
}

bool RNG::getRandomBool() {
	return distribution_bool(generator);
}

int RNG::getInitialNodeCount() {
	return distribution_initial_nodes(generator);
}

double RNG::getRandomNumval() {
	return distribution_real_values(generator);
}

double RNG::getRandomProbability() {
	return distribution_prob_values(generator);
}

bool RNG::getTrueWithProbability(double trueProbability) {
	return trueProbability > distribution_prob_values(generator);
}

/*Returns a random integer in the range [rangeStart, rangeStop] (inclusive)*/
int RNG::getRandomInt(int rangeStart, int rangeStop) {
	std::uniform_int_distribution<int> distribution_int = std::uniform_int_distribution<int>(rangeStart, rangeStop);
	return distribution_int(generator);
}
