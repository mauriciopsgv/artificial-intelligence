#include "Charizard.h"
#include <concurrent_priority_queue.h>
#include <iostream>
#include <time.h>

#define EVALUATION_CONSTANT 3.0
#define POPULATIONSIZE 5

std::ostream& operator<<(std::ostream& os, const Bin& obj)
{
	os << "[ ";
	for (int weightId : obj.weightIds)
	{
		os << weightId << " ";
	}
	os << "]";
	return os;
}


std::ostream& operator<<(std::ostream& os, const CharizardSolution& obj)
{
	os <<"Number of bins:"<< obj.genes.size()<<std::endl;
	int i = 0;
	for (Bin gene : obj.genes)
	{
		os <<i++<<" "<< gene << std::endl;
	}
	return os;
}

bool operator<(const CharizardSolution right, const CharizardSolution left)
{
	return true;
}



Charizard::Charizard(std::vector<int> weights, int binCapacity)
{
	_weights = weights;
	_binCapacity = binCapacity;
	CharizardSolution baseIndividual;
	srand(time(NULL));
}

Charizard::~Charizard()
{
	_weights.~vector();
}

CharizardSolution Charizard::execute()
{
	generateInitialPopulation();
	while (!algorithmHasConverged())
	{
		_totalFitness = 0.0;
		for (auto i : _population)
			_totalFitness += i.first;

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
	CharizardSolution baseIndividual;
	// Temporary
	std::vector<int> weightsIds;
	int z = (int)_weights.size();
	int i = 0;
	while (i++ < z)
	{
		weightsIds.push_back(i - 1);
	}
	firstFitDescendingHeuristic(baseIndividual, weightsIds);
	i = 0;
	while (i++<POPULATIONSIZE)
	{
		// Temporary
		_population.emplace_back(evaluate(baseIndividual), mutate(baseIndividual));
		baseIndividual = mutate(baseIndividual);
	}
}

std::pair<CharizardSolution, CharizardSolution> Charizard::selectParents()
{
	// Generate a number between 0 and 1 and find the parent that occupies its position
	
	double ratio1 = (double) rand() / (double) (RAND_MAX);
	double ratio2 = (double)rand() / (double)(RAND_MAX);

	double threshold1 = ratio1 * _totalFitness;
	double threshold2 = ratio2 * _totalFitness;

	if (threshold2 < threshold1)
	{
		double aux = threshold1;
		threshold1 = threshold2;
		threshold2 = aux;
	}

	CharizardSolution mother, father;
	bool foundMother = false;
	double partialFitnessValue = 0.0;
	for (auto i : _population)
	{
		double evaluationPoint = partialFitnessValue + i.first;
		if (evaluationPoint > threshold1)
		{
			if (foundMother && evaluationPoint > threshold2)
			{
				father = i.second;

			}
			else
			{
				mother = i.second;
				foundMother = true;
			}
		}
		partialFitnessValue += i.first;
	}

	return std::pair<CharizardSolution, CharizardSolution>(mother,father);
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
	int binToBePopped, range = (int)individual.genes.size();
	binToBePopped = rand() % range;
	std::cout << "rand: " << binToBePopped <<std::endl;
	std::vector<int> weightIds;
	weightIds = individual.genes[binToBePopped].weightIds;

	individual.genes[binToBePopped] = individual.genes[range - 1];
	individual.genes.pop_back();

	// replacement should be called here
	//replacement(individual, weightIds);

	firstFitDescendingHeuristic(individual, weightIds);

	return individual;
}

float Charizard::evaluate(CharizardSolution individual)
{
	double sum = 0.0;
	for (Bin gene : individual.genes)
	{
		sum += pow((double)getBinFilling(gene) / (double)_binCapacity, EVALUATION_CONSTANT);
	}
	sum /= individual.genes.size();
	return (float)sum;
}

bool Charizard::algorithmHasConverged()
{
	// Stop Criteria: number of generations without improvements
	return false;
}

void Charizard::firstFitDescendingHeuristic(CharizardSolution& invalidSolution, std::vector<int> unassignedItemsIds)
{
	// Places each unassigned item in the first bin that can hold it
	for (int itemId : unassignedItemsIds)
	{
		bool hasFit = false;
		for (unsigned int i=0;i<invalidSolution.genes.size();i++)
		{
			Bin gene = invalidSolution.genes[i];
			if (getBinFilling(gene) + _weights[itemId] <= _binCapacity)
			{
				invalidSolution.genes[i].weightIds.push_back(itemId);
				hasFit = true;
				break;
			}
		}
		if (!hasFit)
		{
			Bin newBin;
			newBin.weightIds.push_back(itemId);
			invalidSolution.genes.push_back(newBin);
		}
	}
}



void Charizard::replacement(CharizardSolution & invalidSolution, std::vector<int>& unassignedItemsIds)
{
	// Tries to insert according to the replacement method
	for (Bin gene : invalidSolution.genes)
	{

	}
}

int Charizard::getBinFilling(Bin bin)
{
	int filling = 0;
	for (int i : bin.weightIds)
	{
		filling += _weights[i];
	}
	return filling;
}

void Charizard::testFirstFitHeuristic()
{
	CharizardSolution target;
	std::vector<int> weightsIds;
	int z = (int)_weights.size();
	int i = 0;
	while (i++ < z)
	{
		weightsIds.push_back(i-1);
	}
	firstFitDescendingHeuristic(target, weightsIds);
	std::cout << target;

}

void Charizard::testMutate()
{
	CharizardSolution target;
	std::vector<int> weightsIds;
	int z = (int)_weights.size();
	int i = 0;
	while (i++ < z)
	{
		weightsIds.push_back(i - 1);
	}
	firstFitDescendingHeuristic(target, weightsIds);
	std::cout << target << evaluate(target);
	target = mutate(target);
	std::cout << target <<evaluate(target);
}

void Charizard::testSelectParents()
{
	generateInitialPopulation();
	for (auto i : _population)
	{
		std::cout << i.first << std::endl;
		std::cout << i.second << std::endl;
	}
	std::cout << std::endl;
	auto parents = selectParents();
	std::cout << parents.first << std::endl;
	std::cout << parents.second << std::endl;
}

void Charizard::testEvaluate()
{
	CharizardSolution target;
	std::vector<int> weightsIds;
	int z = (int)_weights.size();
	int i = 0;
	while (i++ < z)
	{
		weightsIds.push_back(i - 1);
	}
	firstFitDescendingHeuristic(target, weightsIds);

	for (i = 0; i < target.genes.size(); i++)
	{
		std::cout << i << target.genes[i] <<" Filling: "<<getBinFilling(target.genes[i]) << std::endl;
	}
	std::cout << evaluate(target) << std::endl;

	i = 0;
	weightsIds.clear();
	target.genes.clear();
	while (i++ < z)
	{
		weightsIds.push_back(z-i);
	}
	firstFitDescendingHeuristic(target, weightsIds);
	for (int i = 0; i < target.genes.size(); i++)
	{
		std::cout << i << target.genes[i] << " Filling: " << getBinFilling(target.genes[i]) << std::endl;
	}
	std::cout << evaluate(target) << std::endl;
}