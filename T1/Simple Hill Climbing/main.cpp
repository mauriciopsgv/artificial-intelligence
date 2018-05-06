#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "hillClimbing.h"

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
		//std::cout << weight << std::endl;
		//std::cout << "Quantidade lida: " << (float)(i + 1) / numOfElements * 100 << "%" << std::endl;
	}
	input.close();
}

int main(void)
{
	int binCapacity;
	std::vector<int> weights;

	loadFromFile(std::string("Falkenauer_t60_00.txt"), weights, binCapacity);

	Solution falkenauer_t60_00(weights, binCapacity);

	float fitValue = falkenauer_t60_00.fitnessValue();
	falkenauer_t60_00.print();
	
	Solution pleaseWork = HillClimbing::run(falkenauer_t60_00);
	pleaseWork.print();

	system("pause");
}