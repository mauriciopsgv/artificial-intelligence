#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "hillClimbing.h"

#define MAXEXPLOREDNEIGHBORS 5000000

void mutateSolution(Solution * temporarySolution) {
	int position1 = rand() % temporarySolution->getNumItems();
	int position2 = rand() % temporarySolution->getNumItems();
	temporarySolution->swap(position1, position2);
}

Solution HillClimbing::run(Solution initialSolution) {
	Solution * optimalSolution = new Solution(initialSolution);
	Solution * temporarySolution = new Solution(initialSolution);
	srand(time(NULL));
	float bestFitnessValue = optimalSolution->fitnessValue();
	int sizeOfExploredNeighborhood = 0;

	while (sizeOfExploredNeighborhood < MAXEXPLOREDNEIGHBORS) {
		mutateSolution(temporarySolution);

		if (temporarySolution->fitnessValue() < bestFitnessValue) {
			bestFitnessValue = temporarySolution->fitnessValue();
			sizeOfExploredNeighborhood = 0;
			free(optimalSolution);
			optimalSolution = temporarySolution;
			temporarySolution = new Solution(*optimalSolution);
			std::cout << "Troquei" << std::endl;
		}
		else {
			sizeOfExploredNeighborhood++;
		}
	}
	
	return *optimalSolution;
}