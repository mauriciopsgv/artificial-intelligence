#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <algorithm>

#include "hillClimbing.h"

Solution HillClimbing::runComplex(Solution initialSolution) {
	Solution * optimalSolution = new Solution(initialSolution);
	Solution * temporarySolution = new Solution(initialSolution);

	int sizeOfExploredNeighborhood = 0;
	float bestFitnessValue = optimalSolution->fitnessValueByBins();
	std::vector<int> unnasignedItems;
	while (sizeOfExploredNeighborhood < initialSolution.getNumItems() / 6) {
		unnasignedItems.clear();
		for (int binIndex = temporarySolution->bins.size() - 1; binIndex >= 0; binIndex--) {
			float threshhold = 100 * (1.0 / (float)temporarySolution->bins.size());
			bool shouldDestroyBin = (rand() % 100) < threshhold;

			if (shouldDestroyBin) {
				for (int i = 0; i < temporarySolution->bins[binIndex].weights.size(); i++) {
					unnasignedItems.push_back(temporarySolution->bins[binIndex].weights[i]);
				}
				temporarySolution->bins.erase(temporarySolution->bins.begin() + binIndex);
			}
		}

		std::default_random_engine generator(time(NULL));
		std::shuffle(temporarySolution->bins.begin(), temporarySolution->bins.end(), generator);

		temporarySolution->replacement(unnasignedItems);
		temporarySolution->firstFitRemaining(unnasignedItems);

		if (temporarySolution->fitnessValueByBins() < bestFitnessValue) {
			bestFitnessValue = temporarySolution->fitnessValueByBins();
			sizeOfExploredNeighborhood = 0;
			free(optimalSolution);
			optimalSolution = temporarySolution;
			temporarySolution = new Solution(*optimalSolution);
		}
		else {
			sizeOfExploredNeighborhood++;
			free(temporarySolution);
			temporarySolution = new Solution(*optimalSolution);
		}
	}

	return *optimalSolution;
}