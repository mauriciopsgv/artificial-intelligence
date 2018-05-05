#pragma once
#include <vector>
#include <iostream>

struct Bin {
	std::vector<int> weightIds;
};

struct CharizardSolution {
	std::vector<Bin> genes;
};

class Charizard {
public:
	Charizard(std::vector<int> weights, int binCapacity);
	~Charizard();

	CharizardSolution execute(/* verbose argument may be added in the future */);

	void testReplacement();

	void testFirstFitHeuristic();

	void testMutate();

	void testCrossover();

	void testSelectParents();

	void testEvaluate();

protected:

	void generateInitialPopulation();

	virtual std::pair<CharizardSolution, CharizardSolution> selectParents();

	virtual CharizardSolution crossover(std::pair<CharizardSolution, CharizardSolution> parents);

	virtual CharizardSolution mutate(CharizardSolution individual);

	virtual float evaluate(CharizardSolution individual);

	virtual bool algorithmHasConverged();

	int _binCapacity;

	float last;

	double _totalFitness;

	// May be changed to a static container
	std::vector<int> _weights;

	std::vector<std::pair<float, CharizardSolution>> _population;

	friend bool operator<(const std::pair<float, CharizardSolution> & op1, const std::pair<float, CharizardSolution>& op2)
	{
		return op1.first < op2.first;
	}

	void firstFitDescendingHeuristic(CharizardSolution& invalidSolution, std::vector<int> unassignedItemsIds);

	void replacement(CharizardSolution& invalidSolution, std::vector<int>& unassignedItemsIds);

	int getBinFilling(Bin bin);
};