#include <stdio.h>
#include <stdlib.h>
#include "memoria.h"

/*
int main (int argc, char *argv[])
{
	marco *queue = createQueue();

	enqueue ("Yo", queue);
	printQueue(queue);
	enqueue ("Tú", queue);
	printQueue(queue);
	enqueue ("El", queue);
	printQueue(queue);

	printf("%s\n", dequeue(queue));
	removeQueue(queue);

	printQueue(queue);

	return 0;
}
*/

marco *createMarco (int pid, int size, int paginas)
{
	marco *node = (marco*) calloc (1, sizeof(marco));
	node->pid = pid;
	node->size = size;
	node->paginas = paginas;
}

int removeMarco (marco *node)
{
	node->pid = 0;
	node->next = NULL;
	node->prev = NULL;
	free (node);

	return (node == NULL);
}

memoria *createQueue ()
{
	memoria *queue = (memoria*) calloc (1, sizeof(marco));
	return queue;
}

int removeQueue (memoria* queue)
{
	marco *ptr = queue->front;
	marco *aux;

	while (ptr->next != NULL)
	{
		aux = ptr;
		ptr = ptr->next;
		removeMarco (aux);
		queue->size--;
	}
	removeMarco(ptr);
	free(queue);

	return (queue == NULL);
}

marco *peek (memoria* queue)
{
	if (queue != NULL)
		return queue->front;
	else
		return NULL;
}

int enqueue (int pid, int size, int paginas, memoria* queue)
{
	if (queue != NULL)
	{
		marco *new = createMarco(pid, size, paginas);
		if (queue->front == NULL)
		{
			queue->back = new;
			queue->front = new;
			queue->front->prev = queue->back;
		}
		else
		{
			marco *ptr = queue->front;
			while (ptr != queue->back)
				ptr = ptr->next;
			ptr->next = new;
			new->prev = ptr;
			new->next = NULL;
			queue->back = new;
			queue->front->prev == NULL;
		}
		queue->size++;

		return EXIT_SUCCESS;
	}
	else
		return EXIT_FAILURE;
}

marco *dequeue (memoria* queue)
{
	if (queue != NULL && queue->front != NULL)
	{
		marco *aux = queue->front;
		queue->front = aux->next;
		queue->front->prev = NULL;

		removeMarco(aux);
		queue->size --;

		return aux;
	}
	else
		return NULL;
}

int printQueue (memoria* queue)
{
	marco *ptr = queue->front;
	int i, size = getQueueSize(queue);

	for (i = 0; i < size; i ++)
	{
		//printf("[%d] Dirección: %p - Dato: %s - Siguiente: %p - Anterior: %p\n", i, ptr, ptr->pid, ptr->next, ptr->prev);
		ptr = ptr->next;
	}
}

int getQueueSize (memoria* queue)
{
	return queue->size;
}
