#include "Charizard.h"
#include <concurrent_priority_queue.h>

#define EVALUATION_CONSTANT 3.0


Charizard::Charizard(std::vector<int> weights, int binCapacity)
{
	_weights = weights;
	_binCapacity = binCapacity;
}

Charizard::~Charizard()
{
	_weights.~vector();
}

CharizardSolution Charizard::execute()
{
	/*
	Generate Initial Population
	while algorithm has not converged {
		repeat "populationSize" times
			selectParents
			new individual = parents' crossover
			mutate ?
				individual = mutation of individual
			population gains (individual, fitness)
	}
	*/
	generateInitialPopulation();
	while (!algorithmHasConverged())
	{
		std::vector<std::pair<float, CharizardSolution>> offspring;
		for (unsigned int i = 0; i < _population.size(); i++)
		{
			CharizardSolution individual = crossover(selectParents());
			// Mutation comes here
			offspring.emplace_back(evaluate(individual),individual);
		}
		_population = offspring;
	}

	std::sort_heap(_population.begin(), _population.end());
	return _population[0].second;
}

void Charizard::generateInitialPopulation()
{
}

std::pair<CharizardSolution, CharizardSolution> Charizard::selectParents()
{
	//assuming _population is ordered
	double totalFitnessValue=0.0;
	for (auto i : _population)
		totalFitnessValue += i.first;
	// Generate a number between 0 and 1 and find the parent that occupies its position
	return std::pair<CharizardSolution, CharizardSolution>();
}

CharizardSolution Charizard::crossover(std::pair<CharizardSolution, CharizardSolution> parents)
{
	// refer to https://www.codeproject.com/Articles/633133/ga-bin-packing

	// initially selects two ranges in both parents
	// insert these guys in the other's 

	// pop original parents bins with repetition of what has been inserted

	// use replacement to further improve descendant's fitness
	// firstfitDescendingHeuristic used to make sure solutions are now valid
	// return offspring
	return CharizardSolution();
}

CharizardSolution Charizard::mutate(CharizardSolution individual)
{
	// Pop a random bin and distribute its weights
	return CharizardSolution();
}

float Charizard::evaluate(CharizardSolution individual)
{
	double sum = 0.0;
	for (unsigned int i = 0; i < individual.genes.size(); i++)
	{
		double filling = 0.0;
		Bin gene = individual.genes[i];
		for (unsigned int j = 0; j < gene.weightIds.size(); j++)
		{
			int key = gene.weightIds[j];
			filling += pow((double)_weights[key]/(double)_binCapacity, EVALUATION_CONSTANT);
		}
		sum += filling / individual.genes.size();
	}
	return sum;
}

bool Charizard::algorithmHasConverged()
{
	// Stop Criteria: number of generations without improvements
	return false;
}

void Charizard::firstFitDescendingHeuristic(CharizardSolution& invalidSolution, std::vector<int> unassignedItems)
{
	// Should take the unasignedItems and place them in solution bins following the given heuristic
}

void Charizard::replacement(CharizardSolution & invalidSolution, std::vector<int>& unassignedItems)
{
	// Tries to insert according to the replacement method
}
