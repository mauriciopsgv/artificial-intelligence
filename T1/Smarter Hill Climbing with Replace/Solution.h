#include <vector>

struct Bin {
	std::vector<int> weights;
};

class Solution {
public:
	Solution(std::vector<int> weights, int binCapacity);
	Solution(const Solution &copy);
	~Solution();

	float getFullnessByBins(std::vector<int> binWeights);

	float fitnessValueByBins();

	void firstFitRemaining(std::vector<int> unnasignedItems);

	int getNumItems();

	void replacement(std::vector<int>& unassignedItemsIds);

	bool replacement(Bin & target, int id, std::vector<int>& unassignedItemsIds);

	int sumWeights(std::vector<int> weightIds);

	void printBins();

	void writeOutputFile(std::string fileName);

	std::vector<Bin> bins;

private:
	int _binCapacity;
	
	int _numItems;

	int _numBins;

	std::vector<int> _weights;
};