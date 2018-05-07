#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <time.h>

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
	clock_t time;

	loadFromFile(std::string("Falkenauer_t60_00.txt"), weights, binCapacity);
	Solution falkenauer_t60(weights, binCapacity);
	time = clock();
	falkenauer_t60 = HillClimbing::runComplex(falkenauer_t60);
	time = clock() - time;
	falkenauer_t60.printBins();
	std::cout << "time passed = " << (float)time / CLOCKS_PER_SEC << " seconds" << std::endl;

	weights.clear();
	loadFromFile(std::string("Falkenauer_t120_01.txt"), weights, binCapacity);
	Solution falkenauer_t120(weights, binCapacity);
	time = clock();
	falkenauer_t120 = HillClimbing::runComplex(falkenauer_t120);
	time = clock() - time;
	falkenauer_t120.printBins();
	std::cout << "time passed = " << (float)time / CLOCKS_PER_SEC << " seconds" << std::endl;

	weights.clear();
	loadFromFile(std::string("Falkenauer_u120_02.txt"), weights, binCapacity);
	Solution falkenauer_u120(weights, binCapacity);
	time = clock();
	falkenauer_u120 = HillClimbing::runComplex(falkenauer_u120);
	time = clock() - time;
	falkenauer_u120.printBins();
	std::cout << "time passed = " << (float)time / CLOCKS_PER_SEC << " seconds" << std::endl;

	weights.clear();
	loadFromFile(std::string("Falkenauer_u250_04.txt"), weights, binCapacity);
	Solution falkenauer_u250(weights, binCapacity);
	time = clock();
	falkenauer_u250 = HillClimbing::runComplex(falkenauer_u250);
	time = clock() - time;
	falkenauer_u250.printBins();
	std::cout << "time passed = " << (float)time / CLOCKS_PER_SEC << " seconds" << std::endl;

	weights.clear();
	loadFromFile(std::string("Falkenauer_u500_05.txt"), weights, binCapacity);
	Solution falkenauer_u500(weights, binCapacity);
	time = clock();
	falkenauer_u500 = HillClimbing::runComplex(falkenauer_u500);
	time = clock() - time;
	falkenauer_u500.printBins();
	std::cout << "time passed = " << (float)time / CLOCKS_PER_SEC << " seconds" << std::endl;

	system("pause");
}