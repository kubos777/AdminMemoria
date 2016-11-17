#include <stdio.h>
#include <stdlib.h>

typedef struct nodo
{
	int dato;
	struct nodo *next;
} nodo_t;

int main (int argc, char*argv[])
{
	nodo_t *inicio = NULL, *tope = NULL;
	int dato1, dato2;

	inicio = (nodo_t*) calloc (1, sizeof(nodo_t));
	tope = (nodo_t*) calloc (1, sizeof(nodo_t));

	inicio->next = tope;
	inicio->dato = 1;
	tope->dato = 2;
	
	dato2 = inicio->next->dato;
	free(inicio->next);
	dato1 = inicio->dato;
	free(inicio);

	return 0;
}