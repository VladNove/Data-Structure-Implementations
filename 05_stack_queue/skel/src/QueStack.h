#ifndef QUESTACK_H_
#define QUESTACK_H_

#include "LinkedList.h"
#include "Queue.h"
typedef struct qstack_t qstack_t;
struct qstack_t
{
	queue_t* p;
	queue_t* aux;
};

qstack_t *
qst_create(unsigned int data_size, unsigned int max_size);

unsigned int
qst_get_size(qstack_t *st);

unsigned int
qst_is_empty(qstack_t *st);

void *
qst_peek(qstack_t *st);

void
qst_pop(qstack_t *st);

void
qst_push(qstack_t *st, void *new_data);

void
qst_clear(qstack_t *st);

void
qst_free(qstack_t *st);

#endif  /* QUESTACK_H_ */
