#include "Rng.hpp"


/*Static members*/
std::random_device RNG::seed;
std::mt19937_64 RNG::generator = std::mt19937_64(seed());

//Game iterations
std::negative_binomial_distribution<int> RNG::distribution_iterations = std::negative_binomial_distribution<int>(ROUND_ITERATIONS_STOP_COUNT, ROUND_ITERATIONS_MEAN_PROB);

//Binary choice
std::uniform_int_distribution<int> RNG::distribution_bool = std::uniform_int_distribution<int>(0,1);

//Number of initial nodes
std::uniform_int_distribution<int> RNG::distribution_initial_nodes = std::uniform_int_distribution<int>(0,MAXINITIALNODES);

//Real values
std::normal_distribution<double> RNG::distribution_real_values = std::normal_distribution<double>(NUMVAL_MEAN, NUMVAL_STDDEV);

//Probabilities
std::uniform_real_distribution<double> RNG::distribution_prob_values = std::uniform_real_distribution<double>(0, 1);


int RNG::getIterationCount() {
	return distribution_iterations(generator);
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
