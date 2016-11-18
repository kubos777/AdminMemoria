#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista_procs.h"

proceso *createProc(int pid, int size){
	proceso *node = (proceso *) malloc(sizeof(proceso));
	node->pid = pid;
	node->size = size;
	node->paginas = ceil((float)size / 15);
	node->next = NULL;

	return node;
}

int removeProc(proceso *node){
	//node->pid = NULL;
	node->next = NULL;
	free(node);

	return (node == NULL);
}

lista *createList(){
	lista *list = (lista *) malloc (sizeof(lista));
	list->head = NULL;
	list->length = 0;

	return list;
}

int removeList(lista *list){
	int size = getLength(list), i;
	proceso *ptr = list->head;
	proceso *temp;

	for(i = 0; i<size; i++){
		temp = ptr;
		ptr = ptr->next;
		removeProc
	(temp);
		list->length--;
	}

	free(list);

	return (list == NULL);
}

void printList(lista *list){
	int length = getLength(list)-1, i;
	proceso *ptr = list->head;

	printf("PID\tPAGS.\n");
	for(i = 0; i<=length-1; i++){

		printf("%d\t%d\n", ptr->pid, ptr->paginas);
		ptr = ptr->next;
	}

	printf("\n");

	return;
}

void printNode(proceso *node){
	

	return;
}
/*
void printListByData(lista *list){
	proceso *ptr = list->head;
	while(ptr->next != NULL){
		printf("%s->", ptr->pid);
		ptr = ptr->next;
	}

	printf("%s\n", ptr->pid);

	return;
}
*/
int getLength(lista *list){
	return list->length;
}

int insertAtStart(int pid, int size, lista *list){
	if(list != NULL){
		proceso *new = createProc(pid, size);
		if(list->head == NULL){
			list->head = new;
		}else{
			proceso *r_node = list->head;
			new->next = r_node;
			list->head = new;
		}

		list->length++;

		return EXIT_SUCCESS;
	}else{
		return EXIT_FAILURE; //return -1;
	}
}

int insertAtEnd(int pid, int size, lista *list){
	if(list != NULL){
		if(list->head == NULL){
			insertAtStart(pid, size, list);
		}else{
			proceso *new = createProc(pid, size);
			proceso *l_node = list->head;

			while(l_node->next != NULL){
				l_node = l_node->next;
			}

			l_node->next = new;

			list->length++;
		}

	}else{
		return EXIT_FAILURE;
	}
}

int insertAtPos(int pid, int size, lista *list, int n){
	int length = getLength(list);
	if(list != NULL || n>length){
		if(list->head == NULL || n == 0){
			insertAtStart(pid, size, list);
		}else if(n == length){
			insertAtEnd(pid, size, list);
		}else{
			int i;
			proceso *new = createProc(pid, size);
			proceso *l_node = list->head;

			for(i = 0; i<n-1; i++){
				l_node = l_node->next;
			}

			proceso *r_node = l_node->next;
			l_node->next = new;
			new->next = r_node;

			list->length++;
		}

	}else{
		return EXIT_FAILURE;
	}
}

int removeAtStart(lista *list){
	if(list != NULL && list->head != NULL){
		proceso *delete = list->head;
		list->head = delete->next;
		removeProc
	(delete);

		list->length--;

	}else{
		return EXIT_FAILURE;
	}
}
int removeAtEnd(lista *list){
	if(list != NULL && list->head != NULL){
		proceso *l_node = list->head;

		while(l_node->next->next != NULL){
			l_node = l_node->next;

		}

		removeProc
	(l_node->next);
		l_node->next = NULL;
		list->length--;


	}else{
		return EXIT_FAILURE;
	}
}

int removeAtPos(lista *list, int n){
	int size = getLength(list)-1;
	if(list != NULL && list->head != NULL){
		if(n == 0){
			removeAtStart(list);
		}else if(n == size){
			removeAtEnd(list);
		}else if(n<size){
			int i;
			proceso *l_node = list->head;

			for(i = 0; i<n-1; i++){
				l_node = l_node->next;
			}

			proceso *r_node = l_node->next->next;
			removeProc
		(l_node->next);
			l_node->next = r_node;

			list->length--;
		}

		return EXIT_SUCCESS;

	}else{
		return EXIT_FAILURE;
	}
}
/*
int main(){

	lista *lista = createList();
	insertAtStart ("1", lista);
	insertAtStart ("2", lista);
	insertAtEnd ("2.5", lista);
	insertAtEnd ("3", lista);
	insertAtPos ("4", lista, 3);	
	printListByData (lista);
	printList (lista);

	removeAtEnd(lista);
	removeAtStart(lista);
	removeAtPos(lista, 2);

	printListByData(lista);
	printList(lista);

	removeList (lista);
}
*/