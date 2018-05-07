#include "Charizard.h"
#include <concurrent_priority_queue.h>
#include <iostream>
#include <set>
#include <time.h>
#include <random>
#include <ostream>
#include <fstream>

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
	return _population[0].second;
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

std::set<int> getWeightsInInterval(CharizardSolution solution, int begin, int end)
{
	std::set<int> list;
	for (int i = begin; i <= end; i++)
		for (int weight : solution.genes[i].weightIds)
			list.insert(weight);
	return list;
}



bool hasItem(std::set<int> set, int item)
{
	if (set.find(item) != set.end())
		return true;
	return false;
}

std::pair<CharizardSolution, CharizardSolution> Charizard::crossover(std::pair<CharizardSolution, CharizardSolution> parents)
{
	// refer to https://www.codeproject.com/Articles/633133/ga-bin-packing
	CharizardSolution father, mother;
	father = parents.second;
	mother = parents.first;

	CharizardSolution firstClone, secondClone;

	int fatherBegin, fatherEnd, motherBegin, motherEnd;
	fatherBegin = rand() % father.genes.size();
	fatherEnd = rand() % father.genes.size();
	motherBegin = rand() % mother.genes.size();
	motherEnd = rand() % mother.genes.size();

	createInitialClones(father, mother, fatherBegin, fatherEnd, 
		motherBegin, motherEnd, firstClone, secondClone);

	// Now solutions are invalid, so we need to correct them
	// first we identify duplicated weights (out of protected zone)
	std::set<int> redundancyInClone1 = getWeightsInInterval(mother, motherBegin, motherEnd);
	std::set<int> redundancyInClone2 = getWeightsInInterval(father, fatherBegin, fatherEnd);
	
	// then we must pop the Bins containing them and get the weights with no bin that aren't duplicates
	std::set<int>binsToBePopped1, binsToBePopped2;

	for (int i = 0; i <= fatherEnd; i++)
		for (int weightId : firstClone.genes[i].weightIds)
			if (hasItem(redundancyInClone1, weightId))
			{
				binsToBePopped1.insert(i);
				break;
			}
	for(int i=fatherEnd+1+motherEnd-motherBegin;i<(int)firstClone.genes.size();i++)
		for (int weightId : firstClone.genes[i].weightIds)
			if (hasItem(redundancyInClone1, weightId))
			{
				binsToBePopped1.insert(i);
				break;
			}

	for(int i=0;i<motherBegin;i++)
		for (int weightId : secondClone.genes[i].weightIds)
			if (hasItem(redundancyInClone2, weightId))
			{
				binsToBePopped2.insert(i);
				break;
			}
	for(int i=motherBegin+1+fatherEnd-fatherBegin;i<(int)secondClone.genes.size();i++)
		for (int weightId : secondClone.genes[i].weightIds)
			if (hasItem(redundancyInClone2, weightId))
			{
				binsToBePopped2.insert(i);
				break;
			}

	// To capture weights that are not in redundancy
	std::vector<int> unassignedItems1, unassignedItems2;
	for (int soonToBePopped : binsToBePopped1)
		for (int weightId : firstClone.genes[soonToBePopped].weightIds)
			if (!hasItem(redundancyInClone1, weightId))
				unassignedItems1.push_back(weightId);

	for (int soonToBePopped : binsToBePopped2)
		for (int weightId : secondClone.genes[soonToBePopped].weightIds)
			if (!hasItem(redundancyInClone2, weightId))
				unassignedItems2.push_back(weightId);
	//actually pop
	std::vector<Bin> newClone1;
	for (int i = 0; i < firstClone.genes.size(); i++)
	{
		if (!hasItem(binsToBePopped1, i))
			newClone1.push_back(firstClone.genes[i]);
	}

	std::vector<Bin> newClone2;
	for (int i = 0; i < secondClone.genes.size(); i++)
	{
		if (!hasItem(binsToBePopped1, i))
			newClone2.push_back(secondClone.genes[i]);
	}
	firstClone.genes = newClone1;
	secondClone.genes = newClone2;

	// finally we rearrenge the weights that are not in any bin
	std::sort(unassignedItems1.begin(), unassignedItems1.end());
	std::sort(unassignedItems2.begin(), unassignedItems2.end());

	replacement(firstClone, unassignedItems1);
	replacement(secondClone, unassignedItems2);

	std::sort(unassignedItems1.begin(), unassignedItems1.end());
	std::sort(unassignedItems2.begin(), unassignedItems2.end());

	firstFitDescendingHeuristic(firstClone,unassignedItems1);
	firstFitDescendingHeuristic(secondClone, unassignedItems2);

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
	for (int i = 0;i<(int)invalidSolution.genes.size();i++)
	{

	}
}

bool Charizard::replacement(Bin& target, int id)
{
	// Para cada peso dentro do bin
	//		subistitiu por peso de id
	//		
	//
	return false;
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

void Charizard::testGenerateInitialPopulation()
{
	generateInitialPopulation();
	for (auto evaluatedIndividual : _population)
	{
		std::cout << evaluatedIndividual.first << std::endl;
		std::cout << evaluatedIndividual.second << std::endl;
	}
}