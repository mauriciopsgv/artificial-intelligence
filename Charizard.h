#pragma once
#include <vector>
#include <iostream>
#include <set>

struct Bin {
	std::vector<int> weightIds;
};

struct CharizardSolution {
	std::vector<Bin> genes;
};

std::ostream& operator<<(std::ostream& os, const Bin& obj);

std::ostream& operator<<(std::ostream& os, const CharizardSolution& obj);


class Charizard {
public:
	Charizard(std::vector<int> weights, int binCapacity);

	~Charizard();

	CharizardSolution execute(/* verbose argument may be added in the future */);

	void testReplacement();

	void testFirstFitHeuristic();

	void testGenerateInitialPopulation();;

	void testMutate();

	void testCrossover();

	void testSelectParents();

	void testEvaluate();

protected:

	// Maximum capacity of bins in a problem's instance
	int _binCapacity;
	
	// Sum of fitnesses from all individuals
	double _totalFitness;

	// Actual weight values, a weight's Id is its index in this vector
	std::vector<int> _weights;

	// Each individual in this population is a pair of it's fitness value and itself
	std::vector<std::pair<float, CharizardSolution>> _population;

	//******* Genetic Algorithm Specific Functions ********//
	virtual void generateInitialPopulation();

	virtual std::pair<CharizardSolution, CharizardSolution> selectParents();

	virtual std::pair<CharizardSolution, CharizardSolution> crossover(std::pair<CharizardSolution, CharizardSolution> parents);

	virtual CharizardSolution mutate(CharizardSolution individual);

	virtual float evaluate(CharizardSolution individual);

	//******** Bin Packing Heuristics Functions *********//
	
	void firstFitDescendingHeuristic(CharizardSolution& invalidSolution, std::vector<int> unassignedItemsIds);

	void replacement(CharizardSolution& invalidSolution, std::vector<int>& unassignedItemsIds);

	bool replacement(Bin& target, int id, std::vector<int>& unassignedItemsIds);

	//******* Auxiliary functions ********//

	CharizardSolution translateIdsToWeights(CharizardSolution idContainer);

	void updateTotalFitness();

	int getBinFilling(Bin bin);

	void mergeSecondParent(CharizardSolution& invalidSolution, CharizardSolution validSolution);

	friend bool operator<(const std::pair<float, CharizardSolution> & op1, const std::pair<float, CharizardSolution>& op2);

	void printMauricioStyle(CharizardSolution solution);

	int sumWeights(std::vector<int> weightIds);

};

