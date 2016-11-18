#include <stdio.h>
#include <stdlib.h>
#include "lista_procs.h"
#include "memoria.h"

lista *leer_archivo(); 	//Función que lee y almacena los "procesos" en una lista.
memoria *mover_a_memoria(lista *); //Función que pasa los procesos de una lista de procesos a la "memoria RAM"

//FUNCIÓN MAIN.
int main (int argc, char *argv[])
{
	lista *procesos;
	memoria *memoria;
	
	procesos = leer_archivo();	//Se leen y almacenan los procesos en una lista llamada "procesos"
	printList(procesos);
	memoria = mover_a_memoria(procesos);



	removeList(procesos);
	return 0;
}

//Función que lee y almacena los "procesos" en una lista.
lista *leer_archivo()
{
	FILE *archivo = NULL;
	int pid_temp, size_temp, procs = 0;

	lista *lista = createList(); //Se crea la lista de procesos.

	//Validación y apertura del archivo.
	if ((archivo = fopen("procesos.txt", "r")) == NULL)
	{
		printf("ERROR: No se pudo abrir el archivo que contiene los procesos.\n");
		printf("La ejecución del programa termina en este punto.\n");
		exit(0);
	}
	printf("Listado inicial de procesos en el archivo:\n");
	//Lectura y almacenamiento de los datos del archivo.
	while(feof(archivo) == 0)
	{
		fscanf(archivo, "\t%d %d", &pid_temp, &size_temp);
		insertAtEnd (pid_temp, size_temp, lista);
		if (++procs > 10 || size_temp > 150)
		{
			printf("ERROR: No hay suficiente memoria para almacenar los procesos.\n");
			printf("La ejecución del programa termina en este punto.\n");
			printf("Por favor, elimine algunos procesos del archivo e inténtelo de nuevo.\n");
			exit(0);
		}
	}
	printf("\n");
                           
	return lista;
}

//Función que pasa los procesos de una lista de procesos a la "memoria RAM"
memoria *mover_a_memoria(lista *procesos)
{
	memoria *memoria = createQueue ();
	lista *aux = procesos->head;

	while (getQueueSize(memoria) <= 10)
	{

		enqueue (aux->pid, aux->size, aux->palabras, memoria)
	}
}