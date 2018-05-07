#include <vector>
#include <iostream>
#include <fstream>

#include "Solution.h"

Solution::Solution(std::vector<int> weights, int binCapacity) {
	_weights = weights;
	_binCapacity = binCapacity;
	_numItems = _weights.size();

	int c = _binCapacity;
	std::vector<int> peso_temp;
	float total_fullness = 0.0;

	for (int item_index = 0; item_index < (int) _weights.size(); item_index++) {
		if (c - _weights[item_index] >= 0) {
			c -= _weights[item_index];
			peso_temp.push_back(item_index);
			continue;
		}
		else {
			Bin newBin;
			newBin.weights = peso_temp;
			bins.push_back(newBin);
			item_index--;
			c = _binCapacity;
			peso_temp.clear();
		}
	}
	Bin newBin;
	newBin.weights = peso_temp;
	bins.push_back(newBin);
}

Solution::Solution(const Solution & copy)
{
	_weights = copy._weights;
	_binCapacity = copy._binCapacity;
	_numItems = _weights.size();
	bins = copy.bins;
}

Solution::~Solution() {

}

float Solution::getFullnessByBins(std::vector<int> binWeights) {
	float sum = 0;
	for (int i = 0; i < (int) binWeights.size(); i++) {
		sum += (float) _weights[binWeights[i]];
	}
	return sum;
}

float Solution::fitnessValueByBins() {
	
	float total_fullness = 0.0;

	for (int bin_index = 0; bin_index < (int) bins.size(); bin_index++) {
		total_fullness += (getFullnessByBins(bins[bin_index].weights) / (float)_binCapacity) * (getFullnessByBins(bins[bin_index].weights) / (float)_binCapacity);
	}

	return 1 - (total_fullness / (float)bins.size());
}

void Solution::firstFitRemaining(std::vector<int> unnasignedItems) {
	int c = _binCapacity;
	std::vector<int> peso_temp;

	for (int item_index = 0; item_index < (int) unnasignedItems.size(); item_index++) {
		if (c - _weights[unnasignedItems[item_index]] >= 0) {
			c -= _weights[unnasignedItems[item_index]];
			peso_temp.push_back(unnasignedItems[item_index]);
			continue;
		}
		else {
			Bin newBin;
			newBin.weights = peso_temp;
			bins.push_back(newBin);
			item_index--;
			c = _binCapacity;
			peso_temp.clear();
		}
	}
	Bin newBin;
	newBin.weights = peso_temp;
	bins.push_back(newBin);
}

int Solution::getNumItems()
{
	return _numItems;
}

void Solution::replacement(std::vector<int>& unassignedItemsIds)
{
	// Try to put unassigned items inside invalid solution so bins get fuller
	std::vector<int> itemsToBeAssigned = *new std::vector<int>(unassignedItemsIds);

	// Tries to insert according to the replacement method
	for (int item = itemsToBeAssigned.size() - 1; item >= 0; item--) {
		for (int binNumber = (int)bins.size() - 1; binNumber >= 0; binNumber--) {
			if (replacement(bins[binNumber], itemsToBeAssigned[item], unassignedItemsIds)) {
				unassignedItemsIds.erase(unassignedItemsIds.begin() + item);
				break;
			}
		}
	}
}

bool Solution::replacement(Bin& target, int id, std::vector<int>& unassignedItemsIds)
{
	int currentFullness;
	int originalBinFullness = sumWeights(target.weights);

	if (originalBinFullness + _weights[id] < _binCapacity) {
		target.weights.push_back(id);
		return true;
	}

	for (int i = 0; i < (int) target.weights.size(); i++) {
		currentFullness = originalBinFullness - _weights[target.weights[i]] + _weights[id];
		if (currentFullness < _binCapacity &&
			currentFullness > originalBinFullness) {
			unassignedItemsIds.push_back(target.weights[i]);
			target.weights.erase(target.weights.begin() + i);
			target.weights.push_back(id);
			return true;
		}
	}

	for (int i = 0; i < (int) target.weights.size() - 1; i++) {
		for (int j = i + 1; j < (int) target.weights.size(); j++) {
			currentFullness = originalBinFullness - (_weights[target.weights[i]] + _weights[target.weights[j]]) + _weights[id];
			if (currentFullness < _binCapacity &&
				currentFullness > originalBinFullness) {
				unassignedItemsIds.push_back(target.weights[i]);
				unassignedItemsIds.push_back(target.weights[j]);
				target.weights.erase(target.weights.begin() + j);
				target.weights.erase(target.weights.begin() + i);
				target.weights.push_back(id);
				return true;
			}
		}
	}

	return false;
}

int Solution::sumWeights(std::vector<int> weightIds)
{
	int sum = 0;
	for (int i = 0; i < (int) weightIds.size(); i++) {
		sum += _weights[weightIds[i]];
	}
	return sum;
}

void Solution::printBins() {
	std::cout << "---------------" << std::endl;
	std::cout << "Fitness Value = " << this->fitnessValueByBins() << std::endl;
	std::cout << "Bins = " << bins.size() << std::endl;
}

void Solution::writeOutputFile(std::string fileName) {
	float fitnessValue = this->fitnessValueByBins();
	std::ofstream outputFile;
	outputFile.open(fileName);
	outputFile << bins.size() << std::endl;
	for (int i = 0; i < (int) bins.size(); i++) {
		for (int j = 0; j < (int) bins[i].weights.size(); j++) {
			outputFile << bins[i].weights[j] << " ";
		}
		outputFile << std::endl;
	}
	outputFile.close();
}
