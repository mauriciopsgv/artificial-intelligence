/*header*/

struct bin {
	int fullness;	/*quanto ja esta cheio*/
	int *pesos;
	int qtditens;
};
typedef struct bin Bin;

int binPacking(int *a, int cap, int n, Bin *bins);
float fitness(Bin *bins, int cap, int contBin);
Bin * hillClimbing(Bin * inicial, int cap, int countBin, int numberOfItems);
Bin * swap(Bin * new_bin, int cap, int countBin);
int * set_weight_array(Bin * bins, int n, int countBin);