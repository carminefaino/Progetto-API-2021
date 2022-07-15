#include <stdio.h>
#include <stdlib.h>

#define INF 2147483647            //numero max per int

int sizeClassifica = 0;
int sizeCoda = 0;

typedef struct {
	int n_nodo;
	int cammino;
} Grafo;

void swap(Grafo *a, Grafo *b) {
	Grafo temp = *b;
	*b = *a;
	*a = temp;
}

void CostruisciMaxHeap(Grafo array[], int sizeClassifica, int i) {
	int max, left, right;
	if (sizeClassifica == 1) {
		// non fare nulla
	} else {
    	max = i;
    	left = 2 * i + 1;
    	right = 2 * i + 2;
    	if (left < sizeClassifica && array[left].cammino > array[max].cammino) {
      		max = left;
		}	  
    	if (right < sizeClassifica && array[right].cammino > array[max].cammino) {
      		max = right;
		}	  
    	if (max != i) {
      		swap(&array[i], &array[max]);
      		CostruisciMaxHeap(array, sizeClassifica, max);
    	}
  	}
}

void CostruisciMinHeap(Grafo array[], int sizeCoda, int i) {
	int min, left, right;
	if (sizeCoda == 1) {
		// non fare nulla
	} else {
    	min = i;
    	left = 2 * i + 1;
    	right = 2 * i + 2;
    	if (left < sizeCoda && array[left].cammino < array[min].cammino) {
      		min = left;
		}	  
    	if (right < sizeCoda && array[right].cammino < array[min].cammino) {
      		min = right;
		}	  
    	if (min != i) {
      		swap(&array[i], &array[min]);
      		CostruisciMinHeap(array, sizeCoda, min);
    	}
  	}
}

void CancellaNodoMin(Grafo array[]) {
	int i;

  	swap(&array[0], &array[sizeCoda - 1]);

  	sizeCoda--;

  	for (i = sizeCoda / 2 - 1; i >= 0; i--) {
    	CostruisciMinHeap(array, sizeCoda, i);
  	}
}

void InserisciTopKPieno(Grafo array[], int nodo, int cammino) {
	array[0].n_nodo = nodo;
  	array[0].cammino = cammino;

  	for (int i = sizeClassifica / 2 - 1; i >= 0; i--) {
    	CostruisciMaxHeap(array, sizeClassifica, i);
  	}
}

int potenza (int x, int y) {
	int i, z=0;

	if(y == 0) {
		return 1;
	}
	if(y == 1) {
		return x;
	}
	else {
		z = x*x;    
		for (i = 3; i < (y + 1); i++) {
			z = z*x;
		}
		return z;
	}
}

int legginumero() {
	int i,j,k = 0, l;
	char c;
	char numero[10];

	c = getchar_unlocked();
	
	for (i = 0, l = 0; c != ',' && c != '\n'; i++, l++) {
		numero[i] = c;
		c = getchar_unlocked();
	}
	for (j = (l - 1), i = 0, k = 0; j >= 0; j--, i++) {
		k = k + ((numero[j] - 48) * potenza(10,i)); 
	}	
	return k;
}

int CalcolaCamminoMin(int d) {
	int i, j, x, cammino = 0, nodomin = 0, costodelnodo;
	int grafo[d][d];
	Grafo coda[d];
	
	for(i = 0; i < d; i++) {
		for(j = 0; j < d; j++) {
			grafo[i][j] = legginumero();
		}

		if (i == 0) {
			coda[i].n_nodo = i;
			coda[i].cammino = 0;
		} else {
			coda[i].n_nodo = i;
			coda[i].cammino = INF;
		}
	}

	sizeCoda = d;

	for(i = 0; i < d; i++) {
		nodomin = coda[0].n_nodo;
		costodelnodo = coda[0].cammino;
		if(costodelnodo < INF) {
			cammino = cammino + costodelnodo;
			for(x = 0; x < (d - i); x++) {
				if(((costodelnodo + grafo[nodomin][coda[x].n_nodo]) < coda[x].cammino) && grafo[nodomin][coda[x].n_nodo] > 0) {
					coda[x].cammino = (costodelnodo + grafo[nodomin][coda[x].n_nodo]);
				}
			}
		} else {
			i = d;
		}
		
		CancellaNodoMin(coda);
	}

	return cammino;
}

void kpiccolo(int d, int k) {
	int i;
	char input[15];
	int cammino = 0, contgrafo = -1, primavolta = 1;
	Grafo topK[k];

	while (fgets(input, 15, stdin) != NULL) {

		if (input[0] == 'A') {
			contgrafo++;
			cammino = CalcolaCamminoMin(d);
			sizeCoda = 0;
			if (contgrafo < k) {
				topK[contgrafo].n_nodo = contgrafo;
				topK[contgrafo].cammino = cammino;
			} else {
				if(primavolta == 1) {
					sizeClassifica = k;
					for (int i = sizeClassifica / 2 - 1; i >= 0; i--) {
    					CostruisciMaxHeap(topK, sizeClassifica, i);
					}
					primavolta = 0;
				}
				if(cammino < topK[0].cammino) {
          			InserisciTopKPieno(topK, contgrafo, cammino);
        		}
			}
		} else {
			if(contgrafo < k) {
				for(i = 0; i <= contgrafo; i++) {
					if(i == 0) {
						printf("%d", topK[i].n_nodo);
					} else {
						printf(" %d", topK[i].n_nodo);
					}
				}
				printf("\n");
			} else {
				for(i = 0; i < k; i++) {
					if(i == 0) {
						printf("%d", topK[i].n_nodo);
					} else {
						printf(" %d", topK[i].n_nodo);
					}
				}
				printf("\n");
			}
		}
	}
}

void kgrande(int d, int k) {
	int i, j = 0;
	char input[15];
	int cammino = 0, contgrafo = -1, primavolta = 1, camminolast, decrescente = 1;
	Grafo topK[k];

	while (fgets(input, 15, stdin) != NULL) {

		if (input[0] == 'A') {
			contgrafo++;
			camminolast = cammino;
			cammino = CalcolaCamminoMin(d);
			sizeCoda = 0;
			if (contgrafo < k) {
				topK[contgrafo].n_nodo = contgrafo;
				topK[contgrafo].cammino = cammino;
			} else {
				if(cammino > camminolast) {
					decrescente = 0;
				}
				if(decrescente == 1) {
					topK[j].n_nodo = contgrafo;
					topK[j].cammino = cammino;
					j++;
					if(j == k) {
						j = 0;
					}
				} else {
					if(primavolta == 1) {
						sizeClassifica = k;
						for (int i = sizeClassifica / 2 - 1; i >= 0; i--) {
    						CostruisciMaxHeap(topK, sizeClassifica, i);
						}
						primavolta = 0;
					}
					if(cammino < topK[0].cammino) {
          				InserisciTopKPieno(topK, contgrafo, cammino);
        			}
				}
			}
		} else {
			if(contgrafo < k) {
				for(i = 0; i <= contgrafo; i++) {
					if(i == 0) {
						printf("%d", topK[i].n_nodo);
					} else {
						printf(" %d", topK[i].n_nodo);
					}
				}
				printf("\n");
			} else {
				for(i = 0; i < k; i++) {
					if(i == 0) {
						printf("%d", topK[i].n_nodo);
					} else {
						printf(" %d", topK[i].n_nodo);
					}
				}
				printf("\n");
			}
		}
	}
}

int main() {
	int d, k, i;
	
	i = scanf("%d", &d);
	i = scanf("%d", &k);
	getchar_unlocked();

	i++;

	if(k < 100000) {
		kpiccolo(d, k);
	} else {
		kgrande(d, k);
	}

	return 0;
}