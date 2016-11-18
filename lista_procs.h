#ifndef LISTAPROCS_H_ 
#define LISTAPROCS_H_

//Estructura que almacena los datos del proceso.
typedef struct proceso{
	struct proceso *next;
	int pid;
	int size;
	int paginas;

}proceso;
//Apuntador al inicio de la lista
typedef struct list{
	proceso *head;
	int length;
}lista;

proceso *createProc(int, int);
int removeProc(proceso *node);

lista *createList();
int removeList(lista *list);

void printList(lista *list);
void printNode(proceso *node);
void printListByData(lista *list);
int getLength(lista *list);

int insertAtStart(int, int, lista *list);
int insertAtEnd(int, int, lista *list);
int insertAtPos(int, int, lista *list, int n);

int removeAtStart(lista *list);
int removeAtEnd(lista *list);
int removeAtPos(lista *list, int n);

#endif 