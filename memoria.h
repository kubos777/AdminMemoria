#ifndef MEMORIA_H_ 
#define MEMORIA_H_

//Estructura que simula un marco de página
typedef struct marco{
	struct marco *next;
	struct marco *prev;
	int pid;
	int size;
	int paginas;
}marco;
//Apuntador al inicio de la lista
typedef struct memoria{
	marco *front;
	marco *back;
	int size;

}memoria;

marco *createMarco (int, int, int);
int removeMarco (marco*);
memoria *createQueue ();
int removeQueue (memoria*);
marco *peek (memoria*);
int enqueue (int, int, int, memoria*);
marco *dequeue (memoria*);
int printQueue (memoria*);
int getQueueSize (memoria*);

#endif 