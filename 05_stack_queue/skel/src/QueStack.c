#include <stdlib.h>

#include "QueStack.h"
#include "utils.h"

qstack_t *
qst_create(unsigned int data_size, unsigned int max_size)
{
	qstack_t* new = malloc(sizeof(*new));
	DIE(new == NULL, "Eroare de alocare");
	new->aux=q_create(data_size, max_size);
	new->p=q_create(data_size, max_size);
	return new;
}

unsigned int
qst_get_size(qstack_t *st)
{
	if (!st) return 0;
	return st->p->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
qst_is_empty(qstack_t *st)
{
	if (!st || st->p->size == 0) return 1;
	return 0;
}

void *
qst_peek(qstack_t *st)
{
	return q_front(st->p);
}

void
qst_pop(qstack_t *st)
{
	if (!st) return;
	q_dequeue(st->p);

}

void
qst_push(qstack_t *st, void *new_data)
{
	if (!st) return;
	while (!q_is_empty(st->p))
	{
		void *data = q_front(st->p);
		q_enqueue(st->aux, data);
		q_dequeue(st->p);
	}
	q_enqueue(st->p, new_data);
	while (!q_is_empty(st->aux))
	{
		void *data = q_front(st->aux);
		q_enqueue(st->p, data);
		q_dequeue(st->aux);
	}
}

void
qst_clear(qstack_t *st)
{
	if (!st) return;
	q_clear(st->p);
	
}

void
qst_free(qstack_t *st)
{
	q_free(st->p);
	q_free(st->aux);
	free(st);
}
