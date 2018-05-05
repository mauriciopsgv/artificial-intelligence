/*code*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "binpacking.h"

int binPacking(int *array_pesos, int cap, int n, Bin *bins) {
	int contBin = 1, i, j = 0, k;
	int qtd = 0;
	int c = cap;		/*auxiliar*/
	int * peso_temp;	/*vetor de pesos*/

	for (i = 0; i < n; i++) {
		/* função heuristica */
		if (c - *(array_pesos + i) > 0) {
			c -= *(array_pesos + i);				/*capacidade reduz*/
			peso_temp = *(array_pesos + i);			/*salva peso no bin*/
			qtd++;
			continue;
		}
		else {
			bins[j].qtditens = qtd;					/*do vetor de struct*/
			bins[j].fullness = cap - c;
			contBin++;
			c = cap;
			i--;
			j++;
		}
		bins[i].pesos = (int*)malloc(bins[j].qtditens * sizeof(int));
		for (k = 0; k < qtd; k++)
			bins[i].pesos[k] = peso_temp[k];
	}
	return contBin;
}
int fitness(Bin *bins, int cap, int contBin) {
	int sum = 0;
	int i;

	for (i = 1; i < contBin; i++)
		sum += (bins[i].fullness / cap) * (bins[i].fullness / cap);

	return 1 - sum / contBin;
}
Bin * hillClimbing(Bin * inicial, int cap, int countBin) {
	Bin * tmp = inicial;
	int i;

	int f1, f2;
	int criterio_de_parada = 0;

	while (criterio_de_parada < 350) {
		tmp = swap(tmp, cap, countBin);

		f1 = fitness(inicial, cap, countBin);
		f2 = fitness(tmp, cap, countBin);

		if (f2 <= f1) {
			inicial = tmp;
			criterio_de_parada = 0;
		}
		else
			criterio_de_parada++;
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
	int * vetor_pesos;
	for (i = 0; i < n; i++) {
		for (j = 0; j < countBin; j++) {
			for (k = 0; k < bins[j].qtditens; k++)
				vetor_pesos[i] = bins[j].pesos[k];
		}
	}
	return vetor_pesos;
}