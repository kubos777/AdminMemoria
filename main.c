#include <stdio.h>
#include <stdlib.h>
#include "lista_procs.h"

lista *leer_archivo(); 	//Función que lee y almacena los "procesos" en una lista.

//FUNCIÓN MAIN.
int main (int argc, char *argv[])
{
	lista *procesos;
	procesos = leer_archivo();
	printList(procesos);

	removeList(procesos);
	return 0;
}

//Función que lee y almacena los "procesos" en una lista.
lista *leer_archivo()
{
	FILE *archivo = NULL;
	int pid_temp, size_temp;

	lista *lista = createList(); //Se crea la lista de procesos.

	//Validación y apertura del archivo.
	if ((archivo = fopen("procesos.txt", "r")) == NULL)
	{
		printf("No se pudo abrir el archivo que contiene los procesos.\n");
		printf("La ejecución del programa termina en este punto.\n");
		exit(0);
	}

	//Lectura y almacenamiento de los datos del archivo.
	while(feof(archivo) == 0)
	{
		fscanf(archivo, "%d %d", &pid_temp, &size_temp);
		insertAtEnd (pid_temp, size_temp, lista);
	}
                           
	return lista;
}