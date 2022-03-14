#include <stdlib.h>

#include "Stack.h"
#include "utils.h"

stack_t *
st_create(unsigned int data_size)
{
	stack_t* new = malloc(sizeof(*new));
	DIE(new == NULL, "Eroare de alocare");
	new->list = ll_create(data_size);
	return new;
}

unsigned int
st_get_size(stack_t *st)
{
	if (!st) return 0;
	return st->list->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
st_is_empty(stack_t *st)
{
	if (!st || st->list->size == 0) return 1;
	return 0;
}

void *
st_peek(stack_t *st)
{
	if (!st||!st->list->head) return NULL;
	return st->list->head->data;
}

void
st_pop(stack_t *st)
{
	if (!st) return;
	ll_node_t *elim = ll_remove_nth_node(st->list, 0);
	if (elim) free(elim->data), free(elim);

}

void
st_push(stack_t *st, void *new_data)
{
	if (!st) return;
	ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t *st)
{
	if (!st) return;
	unsigned int data_size = st->list->data_size;
	ll_free(&st->list);
	st->list=ll_create(data_size);
	
}

void
st_free(stack_t *st)
{
	if (!st) return;
	ll_free(&st->list);
	free(st);
}
