#ifndef MEMORIA_H_ 
#define MEMORIA_H_

//Estructura que simula un marco de página
typedef struct marco{
	struct marco *next;
	struct marco *prev;
	int pid;			//Variable que contiene el PID del proceso.
	int size;			//Variable que contiene el tamaño en palabras del proceso.
	int paginas;		//Variable que contiene el tamaño en páginas del proceso.
	int inst_ejec; 	//Variable que simula el apuntador de instrucciones.
}marco;
//Apuntador al inicio de la lista
typedef struct memoria{
	marco *front;
	marco *back;
	int size;

}memoria;

marco *createMarco (int, int, int, int);
int removeMarco (marco*);
memoria *createQueue ();
int removeQueue (memoria*);
marco *peek (memoria*);
int enqueue (int, int, int, int, memoria*);
marco *dequeue (memoria*);
int printQueue (memoria*);
int getQueueSize (memoria*);

#endif 