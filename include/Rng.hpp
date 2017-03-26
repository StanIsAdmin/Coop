#ifndef RNG_H
#define RNG_H

#include <random>
#include <array>

#define ROUND_ITERATIONS_STOP_COUNT 1
#define ROUND_ITERATIONS_MEAN_PROB 0.98
#define MAXINITIALNODES 3
#define NUMVAL_MEAN 0
#define NUMVAL_STDDEV 0.5

class RNG 
{
	private:
		static std::random_device seed;
		
		static std::mt19937_64 generator;
		
		//number of game iterations between two players (pascal distribution with prob. ROUND_ITERATIONS_MEAN_PROB, stops after ROUND_ITERATIONS_STOP_COUNT)
		static std::negative_binomial_distribution<int> distribution_iterations;
		
		//used for different binary choices (True or False)
		static std::uniform_int_distribution<int> distribution_bool;
		
		//number of initial nodes (uniform distribution between 0 and MAXINITIALNODES)
		static std::uniform_int_distribution<int> distribution_initial_nodes;
		
		//random real value (normal distribution with mean NUMVAL_MEAN, std deviation NUMVAL_STDDEV
		static std::normal_distribution<double> distribution_real_values; 
		
		//random probability (uniform distribution between 0 and 1)
		static std::uniform_real_distribution<double> distribution_prob_values;
		

	public:
		int getIterationCount();
		
		bool getRandomBool();
		
		int getInitialNodeCount();
		
		double getRandomNumval();
		
		double getRandomProbability();
		
		bool getTrueWithProbability(double trueProbability);
		
		//selects random individuals from population based on their fitness
		template<std::size_t SIZE>
		void selectPopulation(std::array<double, SIZE>& population_fitness, std::array<int, SIZE>& new_population_indexes) {
			//distribution with probability based on fitness
			std::discrete_distribution<int> distribution_population(population_fitness.begin(), population_fitness.end());
			
			//choose indexes
			for (int i=0; i<SIZE; ++i) {
				new_population_indexes[i] = distribution_population(generator);
			}
		}
};

#endif //RNG_H
