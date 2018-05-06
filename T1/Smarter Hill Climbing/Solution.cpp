#include <vector>
#include <iostream>

#include "Solution.h"

Solution::Solution(std::vector<int> weights, int binCapacity) {
	_weights = weights;
	_binCapacity = binCapacity;
	_numItems = _weights.size();
}

Solution::Solution(const Solution & copy)
{
	_weights = copy._weights;
	_binCapacity = copy._binCapacity;
	_numItems = _weights.size();
}

Solution::~Solution() {

}

float getFullness(std::vector<int> binWeights) {
	float sum = 0;
	for (int i = 0; i < binWeights.size(); i++) {
		sum += (float) binWeights[i];
	}
	return sum;
}

float Solution::fitnessValue() {
	int numBins = 0, item_index, bin_index = 0, k;
	int c = _binCapacity;
	std::vector<int> peso_temp;
	float total_fullness = 0.0;

	for (item_index = 0; item_index < _weights.size(); item_index++) {
		if (c - _weights[item_index] >= 0) {
			c -= _weights[item_index];				
			peso_temp.push_back(_weights[item_index]);
			continue;
		}
		else {
			total_fullness += ( getFullness(peso_temp) / (float) _binCapacity ) * (getFullness(peso_temp) / (float)_binCapacity);
			item_index--;
			numBins++;
			c = _binCapacity;
			peso_temp.clear();
		}
	}
	total_fullness += (getFullness(peso_temp) / (float)_binCapacity) * (getFullness(peso_temp) / (float)_binCapacity);
	_numBins = numBins + 1;
	return 1 - (total_fullness / (float)_numBins);
}

int Solution::getNumItems()
{
	return _numItems;
}

void Solution::swap(int position1, int position2)
{
	int tmp = _weights[position1];
	_weights[position1] = _weights[position2];
	_weights[position2] = tmp;
}

void Solution::print() {
	std::cout << "Solution: " << std::endl;
	std::cout << "Pesos = { ";
	for (int i = 0; i < _weights.size(); i++) {
		std::cout << _weights[i] << " ";
	}
	std::cout << "}" << std::endl;
	std::cout << "Fitness Value = " << this->fitnessValue() << std::endl;
	std::cout << "Bins = " << _numBins << std::endl;
}