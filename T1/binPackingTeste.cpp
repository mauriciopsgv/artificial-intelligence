#include <iostream>
#include "Bulbasaur.h"

std::ostream& operator<< (std::ostream& os, BulbasaurSolution bulba)   {
	for (int i = 0; i < bulba.weights.size(); i++) {
		os << bulba.weights[i] << " ";
	}
	return os;
}

void testFitnessFunction() {
	std::vector<int> testeFitnessFunction;
	
	testeFitnessFunction.push_back(10);
	testeFitnessFunction.push_back(3);
	testeFitnessFunction.push_back(15);
	testeFitnessFunction.push_back(20);
	testeFitnessFunction.push_back(30);
	testeFitnessFunction.push_back(25);

	Bulbasaur bulbassauro(30, 20, 35, 2.5);

	BulbasaurSolution solution;

	solution.weights = testeFitnessFunction;

	float teste = bulbassauro.testFitnessFunction(solution);

	std::cout << "Numero de bins igual a " << teste << std::endl;
}

void testMutation() {
	std::vector<int> test;
	test.push_back(10);
	test.push_back(3);
	test.push_back(15);
	test.push_back(20);
	test.push_back(30);
	test.push_back(25);
	BulbasaurSolution solution;
	solution.weights = test;

	Bulbasaur bulbassauro(30, 20, 35, 2.5);

	std::cout << "Solucao original " << solution << std::endl;

	solution = bulbassauro.testMutation(solution);
	std::cout << "Solucao mutado   " << solution << std::endl;
}

int main (void) {
	testFitnessFunction();
	testMutation();
	return 0;
}