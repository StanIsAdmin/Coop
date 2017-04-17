#ifndef RNG_H
#define RNG_H

#include <random>
#include <array>
#include <iostream>

#define ROUND_ITERATIONS_STOP_COUNT 1
#define ROUND_ITERATIONS_MEAN_PROB 0.98
#define MAXINITIALNODES 3
#define NUMVAL_MEAN 0
#define NUMVAL_STDDEV 0.5

class RNG 
{
	private:
		static unsigned seed;
		static bool seed_is_random;
		static std::mt19937_64 generator;
		
		//random boolean
		static std::uniform_int_distribution<int> distribution_bool;
		
		//random probability
		static std::uniform_real_distribution<double> distribution_probabilities;
		
		//random real value
		static std::normal_distribution<double> distribution_numvals; 
		
		//random number of initial nodes
		static std::uniform_int_distribution<int> distribution_initial_nodes;
		
		//random number of game iterations between two players
		static std::negative_binomial_distribution<int> distribution_iterations;

	public:
		static void setSeed(unsigned new_seed);
		
		static void setRandomSeed();
	
		static long long int getSeed();
		
		static bool seedIsRandom();
		
		static bool getRandomBool();
		
		static bool getTrueWithProbability(double trueProbability);
		
		static int getRandomInt(int rangeStart, int rangeStop);
		
		static double getRandomNumval();
		
		static int getInitialNodeCount();
		
		static int getIterationCount();
		
		//selects random individuals from population based on their fitness
		template<std::size_t SIZE>
		static void selectPopulation(std::array<double, SIZE>& population_fitness, std::array<int, SIZE>& new_population_indexes) {
			//distribution with probability based on fitness
			std::discrete_distribution<int> distribution_population(population_fitness.begin(), population_fitness.end());
			
			//choose indexes
			for (int i=0; i<SIZE; ++i) {
				new_population_indexes[i] = distribution_population(generator);
			}
		}
};

#endif //RNG_H
