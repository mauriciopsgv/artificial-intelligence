#include <stdio.h>
#include <stdlib.h>

#include "binpacking.h"

void printSolution(Bin * bins, int countBin) {
	int debugIndex, debugPesosIndex;
	for (debugIndex = 0; debugIndex < countBin; debugIndex++) {
		printf("Bin numero %d\n", debugIndex);
		printf("fullness = %d\n", bins[debugIndex].fullness);
		printf("pesos = { ");
		for (debugPesosIndex = 0; debugPesosIndex < bins[debugIndex].qtditens; debugPesosIndex++) {
			printf("%d ", bins[debugIndex].pesos[debugPesosIndex]);
		}
		printf("}\n");
		printf("qndItens = %d\n\n", bins[debugIndex].qtditens);
	}
}

int main(void) {
	int n;				/*numero de items*/
	int i = 0;			/*indice*/
	int j;				/*indice*/
	int *array_pesos;	/*array de pesos*/
	int cap;			/*capacidade*/
	int countBin;		/*numero minimo de bins*/

	FILE *arq;
	Bin *bins;

	arq = fopen("Falkenauer_t60_00.txt", "r");
	if (arq == NULL) {
		printf("Não existe arquivo.\n");
		exit(1);
	}

	fscanf(arq, "%d", &n);

	array_pesos = (int*)malloc(n * sizeof(int));
	if (array_pesos == NULL) {
		printf("Memoria insuficiente.\n");
		exit(1);
	}

	bins = (Bin*)malloc(n * sizeof(Bin));
	if (bins == NULL) {
		printf("Erro bins.\n");
		exit(1);
	}

	fscanf(arq, "%d", &cap);

	while (fscanf(arq, "%d", &array_pesos[i]) != EOF) {
		i++;
	}

	countBin = binPacking(array_pesos, cap, n, bins);
	printSolution(bins, countBin);
	bins = hillClimbing(bins, cap, countBin, n);
	array_pesos = set_weight_array(bins, n, countBin);

	countBin = binPacking(array_pesos, cap, n, bins);

	//printSolution(bins, countBin);

	printf("Numero minimo de bins: %d\n", countBin);

	return 0;
}
