#include <string>
#include <fstream>
#include <vector>
#include <iostream>
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


int main(void)
{
	int binCapacity;
	std::vector<int> weights;

	loadFromFile(std::string("Falkenauer_t60_00.txt"),weights,binCapacity);

	Charizard falkenauer_t60_00(weights, binCapacity);
	//CharizardSolution bestSolution = falkenauer_t69_00.execute();
	//falkenauer_t60_00.testFirstFitHeuristic();
	//falkenauer_t60_00.testEvaluate();
	//falkenauer_t60_00.testMutate();
	//falkenauer_t60_00.testSelectParents();
	//falkenauer_t60_00.testGenerateInitialPopulation();
	//Charizard controlado("controlado.txt");
	std::cout<< falkenauer_t60_00.execute();
	system("pause");
}
