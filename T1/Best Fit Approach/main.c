#include <stdio.h>
#include <stdlib.h>

void binPacking(int *a, int tam, int n);

int main(void) {
	int n;
	int *a, i = 0;
	int tam;

	FILE* arq;
	arq = fopen("Falkenauer_t60_00.txt", "r");
	if (arq == NULL) {
		printf("Não existe arquivo.\n");
		exit(1);
	}

	fscanf(arq, "%d", &n);

	a = (int*)malloc(n * sizeof(int));
	if (a == NULL) {
		printf("Memoria insuficiente.\n");
		exit(1);
	}

	fscanf(arq, "%d", &tam);

	while (fscanf(arq, "%d", &a[i]) != EOF) {
		i++;
	}

	binPacking(a, tam, n);

	return 0;
}

void binPacking(int *a, int tam, int n) {
	int contBin = 1, i;
	int t = tam;

	for (i = 0; i < n; i++) {
		/* função heuristica */
		if (t - *(a + i) > 0) {
			t -= *(a + i);
			continue;
		}
		else {
			contBin++;
			t = tam;
			i--;
		}
	}

	printf("Numero de bins necessarios: %d\n", contBin);
}