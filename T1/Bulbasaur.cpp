#include "Bulbasaur.h"

#include <vector>
#include <stdlib.h>
#include <time.h>

Bulbasaur::Bulbasaur(int binSize, int populationSize, int numberOfGenerations, float mutationRatio) {
	binSize_ = binSize;
	populationSize_ = populationSize;
	numberOfGenerations_ = numberOfGenerations;
	mutationRatio_ = mutationRatio;

	srand(time(NULL));
}

Bulbasaur::~Bulbasaur() {
}

std::pair<BulbasaurSolution,BulbasaurSolution> Bulbasaur::parentSelection() {
	for (int i = 0; i < currentSolutions_.size(); i++) {
		
	}
}

BulbasaurSolution Bulbasaur::crossover() {
	BulbasaurSolution ola;
	return ola;
}

BulbasaurSolution Bulbasaur::mutation(BulbasaurSolution solution) {
	// SWAP
	int position1 = rand() % solution.weights.size();
	int position2 = rand() % solution.weights.size();

	int aux = solution.weights[position1];
	solution.weights[position1] = solution.weights[position2]; 
	solution.weights[position2] = aux;
	return solution;
}


float Bulbasaur::fitnessFunction(BulbasaurSolution solution)  {
	int currentBinContent = 0;
	float numberOfBins = 0;
	for (int i = 0; i < solution.weights.size(); i++) {
		currentBinContent += solution.weights[i];
		if (currentBinContent > binSize_) {
			numberOfBins++;
			currentBinContent = solution.weights[i];
		}
	}
	return ++numberOfBins;
}

float Bulbasaur::testFitnessFunction(BulbasaurSolution solution) {
	return fitnessFunction(solution);
}

BulbasaurSolution Bulbasaur::testMutation (BulbasaurSolution solution) {
	return mutation(solution);
}