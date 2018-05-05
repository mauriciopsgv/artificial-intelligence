#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "binPackingSolver_1.h"

//
//int main(void)
//{
//	int n, c;
//	std::vector<int> weights;
//	std::ifstream input;
//	std::string line;
//	input.open("Falkenauer_t60_00.txt");
//	if (!input.is_open())
//	{
//		std::cerr << "File couldn't be opened" << std::endl;
//		system("pause");
//		exit(1);
//	}
//	input >> n;
//	input >> c;
//	for (int i = 0; i < n; i++)
//	{
//		int weight;
//		input >> weight;
//		weights.push_back(weight);
//		std::cout << weight << std::endl;
//		std::cout << "Quantidade lida: " << (float)i / n * 100 << "%" << std::endl;
//
//	}
//	input.close();
//
//	system("pause");
//}

//double world = 0.0;
//for (int i = 0; i < sizeof(fitnesses) / 4; i++)
//{
//	world += 1.0 / (double)fitnesses[i];
//}
//std::cout << "World: " << world << std::endl;
//for (int i = 0; i < sizeof(fitnesses) / 4; i++)
//{
//	std::cout << "Pontuacao: " << 1 / (double)fitnesses[i] / world << std::endl;
//}

void generateInitialPopulation(std::ifstream& inputFile, std::vector<skittySolution>& container, int size)
{
	
}

int main(void)
{
	std::ifstream input;
	int GENERATIONS = 60;
	int POPULATIONSIZE = 100;
	float MUTATIONRATIO = 0.01f;
	std::vector<skittySolution> initialPopulation;

	input.open("Falkenauer_t60_00.txt");
	if (!input.is_open())
	{
		std::cerr << "File couldn't be opened" << std::endl;
		system("pause");
		exit(1);
	}
	skitty problem(GENERATIONS,POPULATIONSIZE,MUTATIONRATIO);
	generateInitialPopulation(input,initialPopulation,POPULATIONSIZE);
	problem.fillInInitialPopulation(initialPopulation);
	system("pause");
}