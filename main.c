#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista_procs.h"
#include "memoria.h"

lista *leer_archivo(); 	//Función que lee y almacena los "procesos" en una lista.
memoria *mover_a_memoria(lista *); //Función que pasa los procesos de una lista de procesos a la "memoria RAM".
memoria *ejecutar_round_robin(memoria *, lista *); //Función que ejecuta procesos por Round Robin.
memoria *cambiar_memoria_a_FIFO(memoria *);
void ejecutar_fifo(memoria *);

//FUNCIÓN MAIN.
int main (int argc, char *argv[]) {
	lista *procesos = NULL;
	memoria *memoria = NULL, *fifo = NULL;
	int inst_ejec = 0;

	procesos = leer_archivo();	//Se leen y almacenan los procesos en una lista llamada "procesos".
	printf("Listado inicial de procesos en el archivo:\n");
	printList(procesos);
	printf("\t\t\t\t\t      Presione 'enter' para continuar...");
	getchar(); //Para ver la ejecución del programa.

	memoria = mover_a_memoria(procesos);	//Se pasan los procesos a la "memoria" (de 10 páginas).
	printf("Situación inicial de la cola de procesos:\n");
	printQueue(memoria);

	while (++inst_ejec <= 11) { //Se hará 11 veces porque el quantum es de 5 y eso se cubre dentro de "ejecutar_round_robin"
		memoria = ejecutar_round_robin(memoria, procesos); //Se pasan todos los procesos a memoria.
		printf("\t\t\t\t\t      Presione 'enter' para continuar...");
		getchar(); //Para ver la ejecución del programa.
		if (getQueueSize(memoria) <= 0)	{ //Si ya no hay nada en la memoria, se termina el programa.
			printf("Ya no quedan procesos por despachar.\n");
			printf("La ejecución del programa termina en este punto.\n");
			removeList(procesos);
			removeQueue(memoria);
			exit(0);
		}
		printf("Actualización de la cola de procesos:\n");
		printQueue(memoria);
	}
	printf("\nAVISO: Se han ejecutado 55 instrucciones. Se cambiará la técnica\n");
	printf("de planificación a 'primero en entrar, primero en servirse.'\n\n");
	fifo = cambiar_memoria_a_FIFO(memoria);
	printf("Los procesos se ejecutarán de la siguiente forma:\n");
	printQueue(fifo);
	printf("\t\t\t\t\t      Presione 'enter' para continuar...");
	getchar(); //Para ver la ejecución del programa.
	ejecutar_fifo(fifo);

	printf("Ya no quedan procesos por despachar.\n");
	printf("La ejecución del programa termina en este punto.\n");
	removeList(procesos);
	removeQueue(memoria);
	//removeQueue(fifo);
	return 0;
}

//Función que lee y almacena los "procesos" en una lista.
lista *leer_archivo(){
	FILE *archivo = NULL;
	int pid_temp, size_temp, procs = 0;

	lista *lista = createList(); //Se crea la lista de procesos.

	//Validación y apertura del archivo.
	if ((archivo = fopen("procesos.txt", "r")) == NULL){
		printf("ERROR: No se pudo abrir el archivo que contiene los procesos.\n");
		printf("La ejecución del programa termina en este punto.\n");
		exit(0);
	}
	//Lectura y almacenamiento de los datos del archivo.
	while(feof(archivo) == 0) {
		fscanf(archivo, "\t%d %d", &pid_temp, &size_temp);
		insertAtEnd (pid_temp, size_temp, lista);
		if (++procs >= 11 || size_temp > 150) { //Se valida que no haya más de 10 procesos y que no ocupen más de 10 páginas.
			printf("ERROR: No hay suficiente memoria para almacenar los procesos.\n");
			printf("La ejecución del programa termina en este punto.\n");
			printf("Por favor, elimine algunos procesos del archivo e inténtelo de nuevo.\n");
			removeList(lista);
			exit(0);
		}
	}
	printf("\n");
    fclose(archivo);                       
	return lista; //Se regresa una lista de todos los procesos cargados desde el archivo.
}

//Función que pasa los procesos de una lista de procesos a la "memoria RAM"
memoria *mover_a_memoria(lista *procesos) {
	memoria *memoria = createQueue ();	//Se crea la cola que simula la memoria RAM.
	proceso *aux = procesos->head; 		//Apuntador auxiliar para recorrer la lista de procesos
	int i, almacenados = 0, ciclos = 0;

	while (memoria->size < 10 && ciclos < 10) { 	//Se meterán hasta 10 páginas, una por cada marco en la memoria RAM
		for (i = 0; i <= getLength(procesos) - 2; i ++) { //Para cada proceso en la lista de procesos
			if (aux->paginas <= 0)	//Si ya no tiene páginas, se pasa al siguiente proceso
				aux = aux->next;
			else {					//De otro modo, se carga una página de ese proceso a la "memoria".
				enqueue(aux->pid, aux->size, aux->paginas, 0, getQueueSize(memoria), 0, memoria); 
				aux->paginas --;
				aux = aux->next;
			}
			if (memoria->size == 10)
				break;
		}
		aux = procesos->head;	//Cuando se llega al fin de la lista, se carga la cabeza de la lista al apuntador auxiliar.
		ciclos ++;
	}
	return memoria;
}

//Función que despacha los procesos de la memoria utilizando la técnica "Round-Robin" con un quantum de 5.
memoria *ejecutar_round_robin(memoria *memoria, lista *procesos){
	int i;
	marco *ejecutar = dequeue(memoria);
	marco *aux = memoria->front;

	printf("Se ejecutará el proceso con PID: %d\n", ejecutar->pid);
	ejecutar->size -= 5;		//Se ejecutan 5 instrucciones, reduce el tamaño del proceso.
	ejecutar->inst_ejec += 5;	//Se actualiza el "apuntador de instrucciones".
	ejecutar->desplazamiento += 5;	//Se actualiza el desplazamiento sobre el bloque

	if (ejecutar->desplazamiento % 15 == 0) { //Si ya se todas las instrucciones en un bloque
		if (ejecutar->paginas >= 1) { //Si aún hay páginas por cargar
			ejecutar->paginas --; //se carga una nueva pagina a memoria.
			enqueue(ejecutar->pid, ejecutar->size, ejecutar->paginas, ejecutar->inst_ejec, ejecutar->bloque, ejecutar->desplazamiento, memoria);
			printf("Terminó la ejecución del proceso con PID %d. Se quedó en:\n", ejecutar->pid);
			printf("Dirección virtual: (%d, %d). ", ejecutar->bloque, ejecutar->desplazamiento);
			printf("Dirección real: %d\n", 1000 + (ejecutar->bloque*15) + ejecutar->desplazamiento);
			printf("Se cargó una nueva página para el proceso con PID %d\n", ejecutar->pid);
		}
		else {	//Si ya no hay páginas por cargar, se carga una nueva de otro proceso.
			ejecutar->paginas --;
			printf("Terminó la ejecución del proceso con PID %d. Se quedó en:\n", ejecutar->pid);
			printf("Dirección virtual: (%d, %d). ", ejecutar->bloque, ejecutar->desplazamiento);
			printf("Dirección real: %d\n", 1000 + (ejecutar->bloque*15) + ejecutar->desplazamiento);
			printf("Se removió una página de la cola. Se cargará una nueva de ser posible. %d\n", ejecutar->pid);
			mover_a_memoria(procesos);
		}
	}
	else if (ejecutar->size <= 0){ //Si ya no quedan instrucciones para ejecutarse
		printf("AVISO: El proceso con PID %d pasó a estado de 'terminado'.\n", ejecutar->pid);
		printf("Terminó su ejecución estando en las direcciones:\n");
		printf("Dirección virtual: (%d, %d). ", ejecutar->bloque, ejecutar->desplazamiento + ejecutar->size);
		printf("Dirección real: %d\n", 1000 + (ejecutar->bloque*15) + (ejecutar->desplazamiento + ejecutar->size));

		if(getQueueSize(memoria) > 0)	//Si aún hay procesos por despachar,
			for (i = 0; i <= getQueueSize(memoria)+1; i ++){ //se checa cada proceso en la cola de procesos,
				marco *eliminar = dequeue(memoria);
				if (eliminar->pid == ejecutar->pid) {	//si un proceso en la cola tiene el mismo PID que el que ya terminó
					removeMarco(eliminar);
					eliminar = eliminar->next;		//éste tambien se elimina de la cola de procesos,
				}
				else								//de otro modo, se vuelve a meter a la cola.
					enqueue(eliminar->pid, eliminar->size, eliminar->paginas, eliminar->inst_ejec, eliminar->bloque, eliminar->desplazamiento, memoria);				
			}
	}
	else {	//Si no hay que cargar páginas y aún hay instrucciones por ejecutarse, 
			//el proceso sólo entra a la cola de nuevo.
		enqueue(ejecutar->pid, ejecutar->size, ejecutar->paginas, ejecutar->inst_ejec, ejecutar->bloque, ejecutar->desplazamiento, memoria);
		printf("Terminó la ejecución del proceso con PID %d. Se quedó en:\n", ejecutar->pid);
		printf("Dirección virtual: (%d, %d). ", ejecutar->bloque, ejecutar->desplazamiento);
		printf("Dirección real: %d\n", 1000 + (ejecutar->bloque*15) + ejecutar->desplazamiento);
	}

	if (getQueueSize(memoria) > 1)	//Si hay más de un proceso por despachar,
		for (i = 0; i <= getQueueSize(memoria)-1; i ++) {//se busca en cada elemento de la cola de procesos
			if (ejecutar->pid == aux->pid) {	//si un proceso tiene el mismo PID que el que se estaba ejecutando
				aux->size = ejecutar->size;	// y se actualizan sus datos.
				aux->paginas = ejecutar->paginas;
				aux->inst_ejec = ejecutar->inst_ejec;
			}
			aux = aux->next;
		}
	printf("%d\n", ejecutar->size);
	removeMarco(ejecutar);
	return memoria;
}

memoria *cambiar_memoria_a_FIFO(memoria *rr) {
	int pag_cargadas = 0, i, repetido;
	memoria *fifo = createQueue();	//Se crea una nueva cola.
	marco *recorre = NULL;			//Apuntador que recorrerá la nueva cola (FIFO).
	marco *cargar = rr->front;		//Apuntador que recorrerá la cola anterior (RR).

	enqueue(cargar->pid, cargar->size, cargar->size/15, cargar->inst_ejec, cargar->bloque, cargar->desplazamiento, fifo);
	
	recorre = fifo->front;
	cargar = cargar->next;
	while (cargar != NULL) { 					//Mientras el proceso a cargar no sea nulo...
		recorre = fifo->front; 					//El apuntador de la nueva cola será igual a su elemento de adelante
		for (i = 0; i < getQueueSize(fifo); i ++) //Se checa cada proceso en la nueva cola.
			if (cargar->pid == recorre->pid) {	//Si un proceso en la cola tiene el mismo PID que el que se desea cargar.
				recorre = recorre->next;		
				repetido = 1;					//se marca como repetido
			}
			else								//de otro modo, se verifica con el siguiente. 
				recorre = recorre->next;

		if (repetido == 0) //Si el proceso a cargar no estaba en la nueva cola, se carga.
			enqueue(cargar->pid, cargar->size, cargar->size/15, cargar->inst_ejec, cargar->bloque, cargar->desplazamiento, fifo);
		
		repetido = 0;
		cargar = cargar->next;
	}

	recorre = fifo->front;
	while (recorre->next != NULL) {
		printf("El proceso con PID %d aún tiene %d instrucciones por ejecutar.\n", recorre->pid, recorre->size);
		printf("Se recorrió %d páginas al proceso con PID %d para encolarlo.\n", recorre->paginas, recorre->next->pid);
		recorre = recorre->next;
	}
	return fifo;
}

void ejecutar_fifo(memoria *fifo) {
	marco *ejecutar = dequeue(fifo);
	while (ejecutar != NULL) {
		printf("Terminó la ejecución del proceso con PID %d. Se quedó en:\n", ejecutar->pid);
		printf("Dirección virtual: (%d, %d). ", ejecutar->paginas, ejecutar->size%15);
		printf("Dirección real: %d\n", 1000 + ejecutar->size);
		printf("Se recorrerán los procesos restantes hasta el inicio de la cola.\n");
		printf("\t\t\t\t\t      Presione 'enter' para continuar...");
		ejecutar->size = 0;
		ejecutar->paginas = 0;
		getchar(); //Para ver la ejecución del programa.
		ejecutar = dequeue(fifo);
	}
}