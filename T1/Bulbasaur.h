#include <vector>
#include <iostream>

class BulbasaurSolution {

public:
	std::vector<int> weights;
	float fitnessValue;
};

class Bulbasaur {
	
private:

	int binSize_;
	int populationSize_;
	int numberOfGenerations_;
	int mutationRatio_;

	std::vector<BulbasaurSolution> currentSolutions_;

	std::pair<BulbasaurSolution,BulbasaurSolution> parentSelection();

	BulbasaurSolution crossover();

	BulbasaurSolution mutation(BulbasaurSolution solution);

	float fitnessFunction(BulbasaurSolution solution);

public:

	Bulbasaur(int binSize, int populationSize, int numberOfGenerations, float mutationRatio);

	~Bulbasaur();


	// test Functions
	float testFitnessFunction(BulbasaurSolution solution);

	BulbasaurSolution testMutation(BulbasaurSolution solution);
};