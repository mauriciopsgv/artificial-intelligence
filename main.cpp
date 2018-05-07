#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <time.h>
#include "Charizard.h"

void loadFromFile(std::string fileName, std::vector<int>& weights, int& binCapacity)
{
	std::ifstream input;
	int numOfElements;
	input.open(fileName);
	if (!input.is_open())
	{
		std::cerr << "File couldn't be opened" << std::endl;
		system("pause");
		exit(1);
	}
	input >> numOfElements;
	input >> binCapacity;

	weights.clear();

	for (int i = 0; i < numOfElements; i++)
	{
		int weight;
		input >> weight;
		weights.push_back(weight);
	}
	std::cout << "Quantidade lida: 100%" << std::endl;
	input.close();
}

void runInstance(std::string instanceFileName)
{
	int binCapacity;
	std::vector<int> weights;
	loadFromFile(instanceFileName, weights, binCapacity);
	clock_t begin = clock();
	Charizard instance(weights, binCapacity);
	std::cout << instance.execute();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Ran " << instanceFileName<<", took ";
	std::cout << elapsed_secs << " second(s) of execution." << std::endl << std::endl << std::endl;

}

int main(void)
{
	runInstance("Falkenauer_t60_00.txt");
	runInstance("Falkenauer_t120_01.txt");
	runInstance("Falkenauer_u120_02.txt");
	runInstance("Falkenauer_u250_04.txt");
	runInstance("Falkenauer_u500_05.txt");
	system("pause");
}



	//CharizardSolution bestSolution = falkenauer_t69_00.execute();
	//falkenauer_t60_00.testFirstFitHeuristic();
	//falkenauer_t60_00.testEvaluate();
	//falkenauer_t60_00.testMutate();
	//falkenauer_t60_00.testSelectParents();
	//falkenauer_t60_00.testGenerateInitialPopulation();
	//Charizard controlado("controlado.txt");