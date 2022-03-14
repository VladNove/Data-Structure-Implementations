#include <stdlib.h>

#include "Queue.h"
#include "utils.h"

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t* new = malloc(sizeof(*new));
	DIE(new == NULL, "Eroare la alocare");
	new->size = 0;
	new->read_idx = 0;
	new->write_idx = 0;
	new->data_size = data_size;
	new->max_size = max_size;
	new->buff = malloc(max_size * sizeof(void*));
	DIE(new->buff == NULL, "Eroare la alocare");
	return new;
}

unsigned int
q_get_size(queue_t *q)
{
	if (!q)return 0;
	return q->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	if (!q || q->size == 0)return 1;
	return 0;
}

void *
q_front(queue_t *q)
{
	if (!q) return NULL;
	return q->buff[q->read_idx];
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool
q_dequeue(queue_t *q)
{
	if (!q || q->size == 0) return false;
	free(q->buff[q->read_idx]);
	q->read_idx = (q->read_idx+1)%q->max_size;
	(q->size)--;
	return true;
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool
q_enqueue(queue_t *q, void *new_data)
{
	if (!q || q->size >= q->max_size) return false;
	void *data = malloc (q->data_size);
	DIE(data == NULL, "eroare alocare");

	for (unsigned int i = 0; i < q->data_size; i++)
	((char*)data)[i] = ((char*)new_data)[i]; //memcpy

	q->buff[q->write_idx] = data;
	q->write_idx = (q->write_idx+1)%q->max_size;
	(q->size)++;
	return true;
}

void
q_clear(queue_t *q)
{
	if(!q)return;
	while(q_dequeue(q) == true);
}

void
q_free(queue_t *q)
{
	if(!q)return;
	while(q_dequeue(q) == true);
	free(q->buff);
	free(q);
}
