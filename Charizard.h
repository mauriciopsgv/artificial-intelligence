#pragma once
#include <vector>

struct Bin {
	std::vector<int> weightIds;
};

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

struct CharizardSolution {
	std::vector<Bin> genes;
};

std::ostream& operator<<(std::ostream& os, const CharizardSolution& obj)
{
	for (Bin gene : obj.genes)
	{
		os << gene << std::endl;
	}
	return os;
}

class Charizard {
public:
	Charizard(std::vector<int> weights, int binCapacity);
	~Charizard();

	CharizardSolution execute(/* verbose argument may be added in the future */);

	void testReplacement();

	void testFirstFitHeuristic();

	void testCrossover();

	void testSelectParensts();

protected:

	void generateInitialPopulation();

	virtual std::pair<CharizardSolution, CharizardSolution> selectParents();

	virtual CharizardSolution crossover(std::pair<CharizardSolution, CharizardSolution> parents);

	virtual CharizardSolution mutate(CharizardSolution individual);

	virtual float evaluate(CharizardSolution individual);

	virtual bool algorithmHasConverged();

	int _binCapacity;

	float last;

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