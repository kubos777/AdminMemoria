#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	struct Node *next;
	struct Node *prev;
	char *data;
} node_t;

typedef struct Queue
{
	node_t *front;
	node_t *back;
	int size;
} queue_t;

node_t *createNode (char*);
int removeNode (node_t*);

queue_t *createQueue ();
int removeQueue (queue_t*);

char *peek (queue_t*);

int enqueue (char*, queue_t*);
char *dequeue (queue_t*);

int printQueue (queue_t*);
int getQueueSize (queue_t*);

int main (int argc, char *argv[])
{
	queue_t *queue = createQueue();

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


node_t *createNode (char *data)
{
	node_t *node = (node_t*) calloc (1, sizeof(node_t));
	node->data = data;
}

int removeNode (node_t *node)
{
	node->data = NULL;
	node->next = NULL;
	node->prev = NULL;
	free (node);

	return (node == NULL);
}

queue_t *createQueue ()
{
	queue_t *queue = (queue_t*) calloc (1, sizeof(node_t));
	return queue;
}

int removeQueue (queue_t* queue)
{
	node_t *ptr = queue->front;
	node_t *aux;

	while (ptr->next != NULL)
	{
		aux = ptr;
		ptr = ptr->next;
		removeNode (aux);
		queue->size--;
	}
	removeNode(ptr);
	free(queue);

	return (queue == NULL);
}

char *peek (queue_t* queue)
{
	if (queue != NULL)
		return queue->front->data;
	else
		return NULL;
}

int enqueue (char* data, queue_t* queue)
{
	if (queue != NULL)
	{
		node_t *new = createNode(data);
		if (queue->front == NULL)
		{
			queue->back = new;
			queue->front = new;
			queue->front->prev = queue->back;
		}
		else
		{
			node_t *ptr = queue->front;
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

char *dequeue (queue_t* queue)
{
	if (queue != NULL &&queue->front != NULL)
	{
		char *data;
		node_t *aux = queue->front;
		queue->front = aux->next;
		queue->front->prev = NULL;
		data = aux->data;

		removeNode(aux);
		queue->size --;

		return data;
	}
	else
		return NULL;
}

int printQueue (queue_t* queue)
{
	node_t *ptr = queue->front;
	int i, size = getQueueSize(queue);

	for (i = 0; i < size; i ++)
	{
		printf("[%d] Dirección: %p - Dato: %s - Siguiente: %p - Anterior: %p\n", i, ptr, ptr->data, ptr->next, ptr->prev);
		ptr = ptr->next;
	}
}

int getQueueSize (queue_t* queue)
{
	return queue->size;
}
