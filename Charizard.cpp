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
#define NOTIMPROVEDGENERATIONS 30

void Charizard::updateTotalFitness()
{
	_totalFitness = 0.0;
	for (auto i : _population)
		_totalFitness += i.first;
}

CharizardSolution Charizard::execute()
{
	generateInitialPopulation();
	updateTotalFitness();
	int genNotImproved = 0;

	while (genNotImproved < NOTIMPROVEDGENERATIONS)
	{
		double previousFitness = _totalFitness;
		std::vector<std::pair<float, CharizardSolution>> offspring;
		for (unsigned int i = 0; i < _population.size(); i+=2)
		{
			std::pair<CharizardSolution, CharizardSolution> individual = crossover(selectParents());
			// Mutation comes here
			offspring.emplace_back(evaluate(individual.first),individual.first);
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

void exchange(int& first, int&second)
{
	int aux = first;
	first = second;
	second = aux;
}

//cria Soluções inválidas com pesos redundantes
void createInitialClones(CharizardSolution father, CharizardSolution mother,
	int fatherBegin, int fatherEnd, int motherBegin, int motherEnd,
	CharizardSolution& son, CharizardSolution& daughter)
{
	if (fatherEnd < fatherBegin)
		exchange(fatherBegin, fatherEnd);
	if (motherEnd < motherBegin)
		exchange(motherBegin, motherEnd);

	int f1 = fatherBegin, f2 = fatherEnd +1 - fatherBegin;
	int m1 = motherBegin, m2 = motherEnd +1 - motherBegin;
	
	auto dadBin = father.genes.begin();
	auto mumBin = mother.genes.begin();
	while (f1-- >0)
	{
		son.genes.push_back(*dadBin);
		dadBin++;
	}
	while (m1-- >0)
	{
		daughter.genes.push_back(*mumBin);
		mumBin++;
	}
	while (f2--)
	{
		son.genes.push_back(*dadBin);
		daughter.genes.push_back(*dadBin);
		dadBin++;
	}
	while (m2--)
	{
		son.genes.push_back(*mumBin);
		daughter.genes.push_back(*mumBin);
		mumBin++;
	}

	while (dadBin != father.genes.end())
	{
		son.genes.push_back(*dadBin);
		dadBin++;
	}
	while (mumBin != mother.genes.end())
	{
		daughter.genes.push_back(*mumBin);
		mumBin++;
	}
}


std::pair<CharizardSolution, CharizardSolution> Charizard::crossover(std::pair<CharizardSolution, CharizardSolution> parents)
{
	// refer to https://www.codeproject.com/Articles/633133/ga-bin-packing

	// initially selects two ranges in both parents
	// insert these guys in the other's
	CharizardSolution father, mother;
	father = parents.second;
	mother = parents.first;

	CharizardSolution firstClone, secondClone;

	int fatherBegin, fatherEnd, motherBegin, motherEnd;
	fatherBegin = rand()%parents.second.genes.size();
	fatherEnd = rand() % parents.second.genes.size();

	motherBegin = rand() % parents.first.genes.size();
	motherEnd = rand() % parents.first.genes.size();

	createInitialClones(father, mother, fatherBegin, fatherEnd, 
		motherBegin, motherEnd, firstClone, secondClone);
	// Fim das inserções

	// pop original parents bins with repetition of what has been inserted



	// use replacement to further improve descendant's fitness
	// firstfitDescendingHeuristic used to make sure solutions are now valid
	// return offspring
	return std::pair<CharizardSolution, CharizardSolution>(firstClone,secondClone);
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
				target.weightIds.erase(target.weightIds.begin() + i);
				target.weightIds.erase(target.weightIds.begin() + j);
				target.weightIds.push_back(id);
				return true;
			}
		}
	}

	return false;
}

int Charizard::sumWeights(std::vector<int> weightIds) 
{
	int sum = 0;
	for (int i = 0; i < weightIds.size(); i++) {
		sum += _weights[weightIds[i]];
	}
	return sum;
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
	printMauricioStyle(target);
	//std::cout << target << evaluate(target);
	target = mutate(target);
	//std::cout << target <<evaluate(target);
	printMauricioStyle(target);
}

void Charizard::printMauricioStyle(CharizardSolution solution)
{
	std::cout << "Number of bins:" << solution.genes.size() << std::endl;
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

#include <ostream>
#include <fstream>

void Charizard::testCreateInitialClone()
{
	
	CharizardSolution father, mother, daughter, son;
	std::vector<int> _w1, _w2;
	int range = (int)_weights.size();
	int i = 0;
	while (i++ < range)
	{
		_w1.push_back(i-1);
		_w2.push_back(range - i);
	}
	firstFitDescendingHeuristic(father, _w1);
	firstFitDescendingHeuristic(mother, _w2);
	
	std::ofstream output("parents.txt");
	if (!output.is_open())
		std::cerr << "Opening output went wrong."<<std::endl;
	output << "Father: " << father << std::endl;
	output << "Mother: " << mother << std::endl;
	output.close();

	//createInitialClones(father, mother, 7, 9, 15, 21, son, daughter);
	//std::cout << "Son 1: " << son << std::endl;
	//std::cout << "Daughter 1: " << daughter << std::endl;

	son.genes.clear();
	daughter.genes.clear();
	createInitialClones(father, mother, 0, 4, 15, 21, son, daughter);
	std::cout << "Son 2: " << son << std::endl;
	std::cout << "Daughter 2: " << daughter << std::endl;

	son.genes.clear();
	daughter.genes.clear();
	createInitialClones(father, mother, 7, 11, 19, 24, son, daughter);
	std::cout << "Son 3: " << son << std::endl;
	std::cout << "Daughter 3: " << daughter << std::endl;
/*
	son.genes.clear();
	daughter.genes.clear();
	createInitialClones(father, mother, 7, 11, 7, 7, son, daughter);
	std::cout << "Son 4: " << son << std::endl;
	std::cout << "Daughter 4: " << daughter << std::endl;*/
}