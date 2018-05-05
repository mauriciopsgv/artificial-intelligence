/*code*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "binpacking.h"

void printSolutionT(Bin * bins, int countBin) {
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

int binPacking(int *original_items, int cap, int n, Bin *bins) {
	int contBin = 1, item_index, bin_index = 0, k;
	int currentBinSize = 0;
	int c = cap;		/*auxiliar*/
	int peso_temp[10];	/*vetor de pesos*/

	for (item_index = 0; item_index < n; item_index++) {
		/* função heuristica */
		if (c - *(original_items + item_index) > 0) {
			c -= *(original_items + item_index);				/*capacidade reduz*/
			peso_temp[currentBinSize] = *(original_items + item_index);			/*salva peso no bin*/
			currentBinSize++;
			continue;
		}
		else {
			bins[bin_index].qtditens = currentBinSize;					/*do vetor de struct*/
			bins[bin_index].fullness = cap - c;
			contBin++;
			c = cap;
			item_index--;
		}
		bins[bin_index].pesos = (int*)malloc(bins[bin_index].qtditens * sizeof(int));
		for (k = 0; k < currentBinSize; k++) {
			bins[bin_index].pesos[k] = peso_temp[k];
		}
		currentBinSize = 0;
		bin_index++;
	}
	bins[bin_index].qtditens = currentBinSize;					/*do vetor de struct*/
	bins[bin_index].fullness = cap - c;
	bins[bin_index].pesos = (int*)malloc(bins[bin_index].qtditens * sizeof(int));
	for (k = 0; k < currentBinSize; k++) {
		bins[bin_index].pesos[k] = peso_temp[k];
	}

	return contBin;
}

float fitness(Bin *bins, int cap, int contBin) {
	float sum = 0.0;
	int i;

	for (i = 1; i < contBin; i++)
		sum += (bins[i].fullness / (float) cap) * (bins[i].fullness / (float) cap);

	return 1 - (sum / contBin);
}

void copyBins(Bin * from, Bin * to, int countBin) {
	int i = 0;
	int j = 0;
	for (i = 0; i < countBin; i++) {
		to[i] = *(Bin*)malloc(sizeof(Bin));
		to[i].fullness = from[i].fullness;
		to[i].qtditens = from[i].qtditens;
		to[i].pesos = (int*)malloc(to[i].qtditens * sizeof(int));
		for (j = 0; j < to[i].qtditens; j++) {
			to[i].pesos[j] = from[i].pesos[j];
		}
	}
}

Bin * hillClimbing(Bin * inicial, int cap, int countBin, int numberOfItems) {
	int i;
	float f1, f2;
	int criterio_de_parada = 0;

	Bin * tmp = (Bin*)malloc(countBin * sizeof(Bin));
	copyBins(inicial, tmp, countBin);

	while (criterio_de_parada < 240) {
		tmp = swap(tmp, cap, countBin);

		f1 = fitness(inicial, cap, countBin);
		f2 = fitness(tmp, cap, countBin);

		if (f2 < f1) {
			copyBins(tmp, inicial, countBin);
			criterio_de_parada = 0;
		}
		else
			criterio_de_parada++;
	}
	free(tmp);
	return inicial;
}

Bin * swap(Bin * new_bin, int cap, int countBin) {
	int randomBin1, randomBin2;	/*valores aleatorios*/
	int randomWeight1, randomWeight2;
	int peso_aux;
	int swapped = 0;

	srand(time(NULL));

	while (!swapped) {
		randomBin1 = rand() % countBin;
		randomBin2 = rand() % countBin;

		randomWeight1 = rand() % new_bin[randomBin1].qtditens;
		randomWeight2 = rand() % new_bin[randomBin2].qtditens;


		new_bin[randomBin1].fullness = new_bin[randomBin1].fullness - new_bin[randomBin1].pesos[randomWeight1] + new_bin[randomBin2].pesos[randomWeight2];	/*compare*/
		new_bin[randomBin2].fullness = new_bin[randomBin2].fullness - new_bin[randomBin2].pesos[randomWeight2] + new_bin[randomBin1].pesos[randomWeight1];

		if (new_bin[randomBin1].fullness <= cap && new_bin[randomBin2].fullness <= cap) {
			peso_aux = new_bin[randomBin1].pesos[randomWeight1];
			new_bin[randomBin1].pesos[randomWeight1] = new_bin[randomBin2].pesos[randomWeight2];	/*swap*/
			new_bin[randomBin2].pesos[randomWeight2] = peso_aux;
			swapped = 1;
		}
		else {
			new_bin[randomBin1].fullness = new_bin[randomBin1].fullness + new_bin[randomBin1].pesos[randomWeight1] - new_bin[randomBin2].pesos[randomWeight2];	/*undo*/
			new_bin[randomBin2].fullness = new_bin[randomBin2].fullness + new_bin[randomBin2].pesos[randomWeight2] - new_bin[randomBin1].pesos[randomWeight1];
		}
	}

	return new_bin;
}

int compare(const void * a, const void * b) {
	Bin t = *(Bin*)a;
	Bin s = *(Bin*)b;
	return s.fullness - t.fullness;
}


int * set_weight_array(Bin * bins, int n, int countBin) {
	int i, j, k;
	int * vetor_pesos = (int*)malloc(n * sizeof(int));
	i = 0;
	qsort(bins, countBin, sizeof(Bin), compare);
	for (j = 0; j < countBin; j++) {
		for (k = 0; k < bins[j].qtditens; k++) {
			vetor_pesos[i] = bins[j].pesos[k];
			i++;
		}
	}
	
	return vetor_pesos;
}