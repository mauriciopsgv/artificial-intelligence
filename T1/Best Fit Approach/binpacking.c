/*code*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "binpacking.h"

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
int fitness(Bin *bins, int cap, int contBin) {
	int sum = 0;
	int i;

	for (i = 1; i < contBin; i++)
		sum += (bins[i].fullness / cap) * (bins[i].fullness / cap);

	return 1 - (sum / contBin);
}
Bin * hillClimbing(Bin * inicial, int cap, int countBin) {
	Bin * tmp = inicial;
	int i;

	int f1, f2;
	int criterio_de_parada = 0;

	while (criterio_de_parada < 15) {
		tmp = swap(tmp, cap, countBin);

		f1 = fitness(inicial, cap, countBin);
		f2 = fitness(tmp, cap, countBin);

		if (f2 < f1) {
			inicial = tmp;
			criterio_de_parada = 0;
		}
		else
			criterio_de_parada++;
		
		if (criterio_de_parada % 5 == 0 &&
			criterio_de_parada > 0) {
			printf("criterio de parada atual = %d\n", criterio_de_parada);
		}
	}
	return inicial;
}
Bin * swap(Bin * new_bin, int cap, int countBin) {
	int r1, r2;	/*valores aleatorios*/
	int p1, p2;
	int peso_aux;

	srand(time(NULL));

	r1 = rand() % countBin;
	r2 = rand() % countBin;

	p1 = rand() % new_bin[r1].qtditens;
	p2 = rand() % new_bin[r2].qtditens;


	new_bin[r1].fullness = new_bin[r1].fullness - new_bin[r1].pesos[p1] + new_bin[r2].pesos[p2];	/*compare*/
	new_bin[r2].fullness = new_bin[r2].fullness - new_bin[r2].pesos[p2] + new_bin[r1].pesos[p1];

	if (new_bin[r1].fullness <= cap && new_bin[r2].fullness <= cap) {
		peso_aux = new_bin[r1].pesos[p1];
		new_bin[r1].pesos[p1] = new_bin[r2].pesos[p2];	/*swap*/
		new_bin[r2].pesos[p2] = peso_aux;
	}
	else {
		new_bin[r1].fullness = new_bin[r1].fullness + new_bin[r1].pesos[p1] - new_bin[r2].pesos[p2];	/*undo*/
		new_bin[r2].fullness = new_bin[r2].fullness + new_bin[r2].pesos[p2] - new_bin[r1].pesos[p1];
	}

	return new_bin;
}
int * set_weight_array(Bin * bins, int n, int countBin) {
	int i, j, k;
	int * vetor_pesos = (int*)malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		for (j = 0; j < countBin; j++) {
			for (k = 0; k < bins[j].qtditens; k++)
				vetor_pesos[i] = bins[j].pesos[k];
		}
	}
	return vetor_pesos;
}