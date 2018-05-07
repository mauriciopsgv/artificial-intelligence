#include "Charizard.h"
#include <concurrent_priority_queue.h>
#include <iostream>
#include <set>
#include <time.h>
#include <random>
#include <ostream>
#include <fstream>

#define EVALUATION_CONSTANT 3.0
#define POPULATIONSIZE 40
#define NOTIMPROVEDGENERATIONS 10
#define MUTATION_RATE 1.2

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

bool operator<(const std::pair<float, CharizardSolution> & op1, const std::pair<float, CharizardSolution>& op2)
{
	return op1.first < op2.first;
}

bool operator<(const CharizardSolution right, const CharizardSolution left)
{
	return true;
}

/***********************************************/

void transferBins(CharizardSolution& destin, CharizardSolution source, int begin, int end);

void exchange(int& first, int&second);

bool hasItem(std::set<int> set, int item);

bool hasItem(Bin bin, int item);

/***********************************************/

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
	std::default_random_engine _generator;
	std::uniform_real_distribution<double> distribution(0.0, 100.0);

	generateInitialPopulation();
	updateTotalFitness();
	int genNotImproved = 0;

	// Convergence criteria is overall population improvement
	while (genNotImproved < NOTIMPROVEDGENERATIONS)
	{	// convergence criteria not met
		double previousFitness = _totalFitness;
		std::vector<std::pair<float, CharizardSolution>> offspring;
		for (unsigned int i = 0; i < _population.size(); i += 2)
		{

			std::pair<CharizardSolution, CharizardSolution> individual = crossover(selectParents());

			if (distribution(_generator) <= MUTATION_RATE)
				individual.first = mutate(individual.first);

			if (distribution(_generator) <= MUTATION_RATE)
				individual.second = mutate(individual.second);


			offspring.emplace_back(evaluate(individual.first), individual.first);
			offspring.emplace_back(evaluate(individual.second), individual.second);
		}
		_population = offspring;

		updateTotalFitness();

		if (_totalFitness <= previousFitness)
			genNotImproved++;
		else
			genNotImproved = 0;
	}

	std::make_heap(_population.begin(), _population.end());
	
	return translateIdsToWeights(_population[0].second);
}

void Charizard::generateInitialPopulation()
{
	int z = (int)_weights.size();
	_population.clear();
	std::vector<int> weightsIds;
	weightsIds.resize(z);
	for (int i = 0; i < z; i++)
		weightsIds[i] = i;
	int i = 0;
	while (i++<POPULATIONSIZE)
	{
		std::default_random_engine generator(time(NULL));
		CharizardSolution newIndividual;
		std::shuffle(weightsIds.begin(), weightsIds.end(),generator);
		firstFitDescendingHeuristic(newIndividual, weightsIds);
		_population.emplace_back(evaluate(newIndividual),newIndividual);
	}
}

std::pair<CharizardSolution, CharizardSolution> Charizard::selectParents()
{
	// Generate a number between 0 and 1 and find the parent that occupies its position
	
	double ratio1 = (double)rand() / (double)(RAND_MAX);
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
	if (!father.genes.size()) //mother was last individual
		father = _population[0].second;
	return std::pair<CharizardSolution, CharizardSolution>(mother,father);
}

std::pair<CharizardSolution, CharizardSolution> Charizard::crossover(std::pair<CharizardSolution, CharizardSolution> parents)
{
	CharizardSolution father, mother;
	father = parents.second;
	mother = parents.first;

	CharizardSolution firstClone, secondClone;

	int fatherBegin, fatherEnd, motherBegin, motherEnd;
	fatherBegin = rand() % father.genes.size();
	fatherEnd = rand() % father.genes.size();
	motherBegin = rand() % mother.genes.size();
	motherEnd = rand() % mother.genes.size();
	if (fatherEnd < fatherBegin)
		exchange(fatherBegin, fatherEnd);
	if (motherEnd < motherBegin)
		exchange(motherBegin, motherEnd);

	transferBins(firstClone, mother, motherBegin, motherEnd);
	transferBins(secondClone, father, fatherBegin, fatherEnd);

	mergeSecondParent(firstClone, father);
	mergeSecondParent(secondClone, mother);

	return std::pair<CharizardSolution, CharizardSolution>(firstClone,secondClone);
}

CharizardSolution Charizard::mutate(CharizardSolution individual)
{
	int binToBePopped, range = (int)individual.genes.size();
	binToBePopped = rand() % range;
	std::vector<int> weightIds;
	weightIds = individual.genes[binToBePopped].weightIds;

	individual.genes[binToBePopped] = individual.genes[range - 1];
	individual.genes.pop_back();

	// replacement should be called here
	replacement(individual, weightIds);
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

void Charizard::firstFitDescendingHeuristic(CharizardSolution& invalidSolution, std::vector<int> unassignedItemsIds)
{
	// Places each unassigned item in the first bin that can hold it
	for (int itemId : unassignedItemsIds)
	{
		bool hasFit = false;
		for (unsigned int i = 0; i<invalidSolution.genes.size(); i++)
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
	// Try to put unassigned items inside invalid solution so bins get fuller
	std::vector<int> itemsToBeAssigned = *new std::vector<int>(unassignedItemsIds);

	// Tries to insert according to the replacement method
	for (int item = itemsToBeAssigned.size() - 1; item >= 0; item--) {
		for (int binNumber = (int)invalidSolution.genes.size() - 1; binNumber >= 0; binNumber--) {
			if (replacement(invalidSolution.genes[binNumber], itemsToBeAssigned[item], unassignedItemsIds)) {
				unassignedItemsIds.erase(unassignedItemsIds.begin() + item);
				break;
			}
		}
	}
}

bool Charizard::replacement(Bin& target, int id, std::vector<int>& unassignedItemsIds)
{
	// Para cada peso dentro do bin
	//		subistitiu por peso de id
	int currentFullness;
	int originalBinFullness = sumWeights(target.weightIds);

	if (originalBinFullness + _weights[id] < _binCapacity) {
		target.weightIds.push_back(id);
		return true;
	}

	for (int i = 0; i < target.weightIds.size(); i++) {
		currentFullness = originalBinFullness - _weights[target.weightIds[i]] + _weights[id];
		if (currentFullness < _binCapacity &&
			currentFullness > originalBinFullness) {
			unassignedItemsIds.push_back(target.weightIds[i]);
			target.weightIds.erase(target.weightIds.begin() + i);
			target.weightIds.push_back(id);
			return true;
		}
	}

	for (int i = 0; i < target.weightIds.size() - 1; i++) {
		for (int j = i + 1; j < target.weightIds.size(); j++) {
			currentFullness = originalBinFullness - (_weights[target.weightIds[i]] + _weights[target.weightIds[j]]) + _weights[id];
			if (currentFullness < _binCapacity &&
				currentFullness > originalBinFullness) {
				unassignedItemsIds.push_back(target.weightIds[i]);
				unassignedItemsIds.push_back(target.weightIds[j]);
				target.weightIds.erase(target.weightIds.begin() + j);
				target.weightIds.erase(target.weightIds.begin() + i);
				target.weightIds.push_back(id);
				return true;
			}
		}
	}

	return false;
}

CharizardSolution Charizard::translateIdsToWeights(CharizardSolution idContainer)
{
	CharizardSolution external;
	for (Bin gene : idContainer.genes)
	{
		for (int i = 0; i < gene.weightIds.size(); i++)
		{
			gene.weightIds[i] = _weights[gene.weightIds[i]];
		}
		external.genes.push_back(gene);
	}
	return external;
}

void Charizard::updateTotalFitness()
{
	_totalFitness = 0.0;
	for (auto i : _population)
		_totalFitness += i.first;
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

void Charizard::mergeSecondParent(CharizardSolution& invalidSolution, CharizardSolution validSolution)
{
	// Weights that are already inside my solution
	std::set<int> allocatedWeights;
	for (Bin alreadyInside : invalidSolution.genes)
	{
		for (int weightId : alreadyInside.weightIds)
			allocatedWeights.insert(weightId);
	}

	std::vector<int> unassignedItems;
	for (Bin gene : validSolution.genes)
	{
		bool geneIsDirty = false;
		for (int weight : allocatedWeights)
		{
			if (hasItem(gene, weight))
			{
				geneIsDirty = true;
				break;
			}
		}
		if (geneIsDirty)
		{
			// In case this gene contains a weight already inside my incomplete solution
			// we have to track the other weights inside this same gene that are not 
			// assigned to any bin yet
			for (int weight : gene.weightIds)
			{
				if (!hasItem(allocatedWeights, weight))
					unassignedItems.push_back(weight);
			}
		}
		else
		{
			invalidSolution.genes.push_back(gene);
		}
	}
	std::sort(unassignedItems.begin(), unassignedItems.end());
	replacement(invalidSolution, unassignedItems);
	firstFitDescendingHeuristic(invalidSolution, unassignedItems);
}

int Charizard::sumWeights(std::vector<int> weightIds) 
{
	int sum = 0;
	for (int i = 0; i < weightIds.size(); i++) {
		sum += _weights[weightIds[i]];
	}
	return sum;
}

void Charizard::printMauricioStyle(CharizardSolution solution)
{
	std::cout << "Number of bins:" << solution.genes.size() << " Fitness: " << evaluate(solution) << std::endl;
	int i = 0;
	for (Bin gene : solution.genes)
	{
		std::cout << i++ << " ";
		std::cout << "[ ";
		for (int weightId : gene.weightIds)
		{
			std::cout << weightId << ":" << _weights[weightId] << " ";
		}
		std::cout << "]";

		std::cout << " " << sumWeights(gene.weightIds) << " out of " << _binCapacity << std::endl;
	}
}

void transferBins(CharizardSolution& destin, CharizardSolution source, int begin, int end)
{
	for (int i = begin; i <= end; i++)
		destin.genes.push_back(source.genes[i]);
}

void exchange(int& first, int&second)
{
	int aux = first;
	first = second;
	second = aux;
}

bool hasItem(std::set<int> set, int item)
{
	if (set.find(item) != set.end())
		return true;
	return false;
}

bool hasItem(Bin bin, int item)
{
	for (int weight : bin.weightIds)
		if (weight == item)
			return true;
	return false;
}

// Tests
void Charizard::testFirstFitHeuristic()
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
	printMauricioStyle(target);
	target = mutate(target);
	printMauricioStyle(target);
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

void Charizard::testGenerateInitialPopulation()
{
	generateInitialPopulation();
	for (auto evaluatedIndividual : _population)
	{
		std::cout << evaluatedIndividual.first << std::endl;
		std::cout << evaluatedIndividual.second << std::endl;
	}
}

void Charizard::testCrossover() 
{
	std::ofstream log("log.txt");
	if (!log.is_open())
	{
		std::cerr << "Deu ruim no log." << std::endl;
	}
	
	CharizardSolution father, mother;
	std::vector<int> _w1, _w2;
	int range = (int)_weights.size();
	int i = 0;
	while (i++ < range)
	{
		_w1.push_back(i - 1);
		_w2.push_back(range - i);
	}
	firstFitDescendingHeuristic(father, _w1);
	firstFitDescendingHeuristic(mother, _w2);
	auto offspring = crossover(std::make_pair(father, mother));
	log << offspring.first << std::endl;
	log << offspring.second << std::endl;
	log.close();
}