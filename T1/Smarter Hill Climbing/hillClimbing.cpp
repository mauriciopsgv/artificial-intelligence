#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "hillClimbing.h"

#define MAXEXPLOREDNEIGHBORS 100

void mutateSolution(Solution * temporarySolution) {
	int position1 = rand() % temporarySolution->getNumItems();
	int position2 = rand() % temporarySolution->getNumItems();
	temporarySolution->swap(position1, position2);
}

Solution HillClimbing::run(Solution initialSolution) {
	Solution * optimalSolution = new Solution(initialSolution);
	Solution * localTemporarySolution = new Solution(initialSolution);
	Solution * exploratoryTemporarySolution = new Solution(initialSolution);
	srand(time(NULL));
	float bestFitnessValue = optimalSolution->fitnessValue();
	int sizeOfExploredNeighborhood = 0;

	while (sizeOfExploredNeighborhood < MAXEXPLOREDNEIGHBORS) {
		mutateSolution(localTemporarySolution);
		mutateSolution(exploratoryTemporarySolution);

		if (localTemporarySolution->fitnessValue() < bestFitnessValue) {
			bestFitnessValue = localTemporarySolution->fitnessValue();
			sizeOfExploredNeighborhood = 0;
			free(optimalSolution);
			optimalSolution = localTemporarySolution;
			localTemporarySolution = new Solution(*optimalSolution);
			free(exploratoryTemporarySolution);
			exploratoryTemporarySolution = new Solution(*optimalSolution);
		}
		else if (exploratoryTemporarySolution->fitnessValue() < bestFitnessValue) {
			bestFitnessValue = exploratoryTemporarySolution->fitnessValue();
			sizeOfExploredNeighborhood = 0;
			free(optimalSolution);
			optimalSolution = exploratoryTemporarySolution;
			exploratoryTemporarySolution = new Solution(*optimalSolution);
			free(localTemporarySolution);
			localTemporarySolution = new Solution(*optimalSolution);
		}
		else {
			sizeOfExploredNeighborhood++;
			free(localTemporarySolution);
			localTemporarySolution = new Solution(*optimalSolution);
		}
	}
	
	return *optimalSolution;
}