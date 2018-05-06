#include <vector>


class Solution {
public:
	Solution(std::vector<int> weights, int binCapacity);
	Solution(const Solution &copy);
	~Solution();

	float fitnessValue();

	int getNumItems();

	void swap(int position1, int position2);

	void print();

private:
	int _binCapacity;
	
	int _numItems;

	int _numBins;

	std::vector<int> _weights;
};