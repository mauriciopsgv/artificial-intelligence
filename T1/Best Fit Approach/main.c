#include <stdio.h>
#include <stdlib.h>

#include "binpacking.h"

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
	bins = hillClimbing(bins, cap, countBin);
	array_pesos = set_weight_array(bins, n, countBin);
	countBin = binPacking(array_pesos, cap, n, bins);

	printf("Numero minimo de bins: %d\n", countBin);

	/*debug*/
	/*for (i = 0; i < n; i++) {
		for (j = 0; j < bins[i].qtditens; j++) {
			printf("%d ", bins[i].pesos[j]);
		}
		printf("\n");
	}*/

	return 0;
}
