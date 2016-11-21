#include <stdio.h>
#include <stdlib.h>
#include "lista_procs.h"
#include "memoria.h"

lista *leer_archivo(); 	//Función que lee y almacena los "procesos" en una lista.
memoria *mover_a_memoria(lista *); //Función que pasa los procesos de una lista de procesos a la "memoria RAM".
memoria *ejecutar_round_robin(memoria *); //Función que ejecuta procesos por Round Robin.
memoria *cargar_proceso(memoria *, lista *); //Función que carga un nuevo proceso a la memoria

//FUNCIÓN MAIN.
int main (int argc, char *argv[])
{
	lista *procesos;
	memoria *memoria;
	int inst_ejec = 0;
	
	procesos = leer_archivo();	//Se leen y almacenan los procesos en una lista llamada "procesos".
	printf("Listado inicial de procesos en el archivo:\n");
	printList(procesos);
	memoria = mover_a_memoria(procesos);	//Se pasan los procesos a la "memoria" (de 10 páginas).
	printf("Situación inicial de la cola de procesos:\n");
	printQueue(memoria);
	while (inst_ejec < 11)
	{
		memoria = ejecutar_round_robin(memoria);
		getchar(); //Para ver la ejecución del programa.
		printf("Actualización de la cola de procesos:\n");
		printQueue(memoria);
		inst_ejec ++;
	}
	printf("AVISO: Se han ejecutado 55 instrucciones.\n");
	printf("Se cambiará la técnica de planificación a 'First in, first out'\n");

	removeList(procesos);
	removeQueue(memoria);
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
	//Lectura y almacenamiento de los datos del archivo.
	while(feof(archivo) == 0)
	{
		fscanf(archivo, "\t%d %d", &pid_temp, &size_temp);
		insertAtEnd (pid_temp, size_temp, lista);
		if (++procs >= 11 || size_temp > 150) //Se valida que no haya más de 10 procesos y que no ocupen más de 10 páginas.
		{
			printf("ERROR: No hay suficiente memoria para almacenar los procesos.\n");
			printf("La ejecución del programa termina en este punto.\n");
			printf("Por favor, elimine algunos procesos del archivo e inténtelo de nuevo.\n");
			exit(0);
		}
	}
	printf("\n");
                           
	return lista; //Se regresa una lista de todos los procesos cargados desde el archivo.
}

//Función que pasa los procesos de una lista de procesos a la "memoria RAM"
memoria *mover_a_memoria(lista *procesos)
{
	memoria *memoria = createQueue ();	//Se crea la cola que simula la memoria RAM.
	proceso *aux = procesos->head; 		//Apuntador auxiliar para recorrer la lista de procesos
	int i, almacenados = 0, ciclos = 0;

	while (memoria->size < 10 && ciclos < 10) 	//Se meterán hasta 10 páginas, una por cada marco en la memoria RAM
	{
		for (i = 0; i <= getLength(procesos) - 2; i ++) //Para cada proceso en la lista de procesos
			if (aux->paginas <= 0)	//Si ya no tiene páginas, se pasa al siguiente proceso
				aux = aux->next;
			else					//De otro modo, se carga una página de ese proceso a la "memoria".
			{
				enqueue(aux->pid, aux->size, aux->paginas, 0, memoria);
				aux->paginas --;
				aux = aux->next;
			}
		aux = procesos->head;	//Cuando se llega al fin de la lista, se carga la cabeza de la lista al vector auxiliar.
		ciclos ++;
	}

	return memoria;
}

memoria *ejecutar_round_robin(memoria *memoria)
{
	int i;
	marco *ejecutar = dequeue(memoria);
	marco *aux = memoria->front;
	printf("Se ejecutará el proceso con PID: %d\n", ejecutar->pid);
	ejecutar->size -= 5;
	ejecutar->inst_ejec += 5;
	//printf("INST: %d\n",ejecutar->size);
	if (ejecutar->inst_ejec % 15 == 0 && ejecutar->size >= 15) //Si se ejecutan 15 instrucciones...
	{
		ejecutar->paginas --; //Se carga una nueva pagina a memoria
		enqueue(ejecutar->pid, ejecutar->size, ejecutar->paginas, ejecutar->inst_ejec, memoria);
	}
	else if (ejecutar->size <= 0)
	{
		printf("AVISO: El proceso con PID %d ha terminado de ejecutarse\n", ejecutar->pid);
		for (i = 0; i <= getQueueSize(memoria); i ++) //Para cada proceso en la cola de procesos
		{
			marco *eliminar = dequeue(memoria);
			if (eliminar->pid == ejecutar->pid)	//Si un proceso en la cola tiene el mismo PID que el que se está ejecutando
				removeMarco(eliminar);
			else
				enqueue(eliminar->pid, eliminar->size, eliminar->paginas, eliminar->inst_ejec, memoria);			
		}
		//cargar_proceso(memoria, procesos)
	}
	else
	{
		enqueue(ejecutar->pid, ejecutar->size, ejecutar->paginas, ejecutar->inst_ejec, memoria);
	}
	for (i = 0; i <= getQueueSize(memoria)-1; i ++) //Para cada proceso en la cola de procesos
	{
		if (ejecutar->pid == aux->pid)	//Si un proceso en la cola tiene el mismo PID que el que se está ejecutando
		{
			aux->size = ejecutar->size;	//se actualizan sus datos.
			aux->paginas = ejecutar->paginas;
			aux->inst_ejec = ejecutar->inst_ejec;
		}
		aux = aux->next;
	}

	removeMarco(ejecutar);
	return memoria;
}

memoria *cargar_proceso(memoria *memoria, lista *procesos)
{

}