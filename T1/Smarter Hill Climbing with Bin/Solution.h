#include <vector>

struct Bin {
	std::vector<int> weights;
};

class Solution {
public:
	Solution(std::vector<int> weights, int binCapacity);
	Solution(const Solution &copy);
	~Solution();

	float fitnessValue();

	int getNumItems();

	void swap(int position1, int position2);

	void print();

	void printBins();

	void writeOutputFile();

private:
	int _binCapacity;
	
	int _numItems;

	int _numBins;

	std::vector<int> _weights;
	
	std::vector<Bin> _bins;
};