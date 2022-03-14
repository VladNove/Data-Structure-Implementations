#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t*
ll_create(unsigned int data_size)
{
    linked_list_t* new_list = malloc(sizeof(*new_list));
    DIE(new_list == NULL, "eroare malloc");
    new_list->head = NULL;
    new_list->data_size = data_size;
    new_list->size = 0;
    return new_list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{

    if(!list)return;

    ll_node_t *new_node = malloc(sizeof(*new_node));
    DIE(new_node == NULL, "malloc error");

    new_node->data = malloc(list->data_size);
    DIE(new_node->data == NULL, "malloc error");
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = NULL;

    if (n > list->size) n = list->size;
    if (n == 0)
    {
    	if (list->head)
    		new_node->next = list->head;
    	list->head = new_node;
    }
    else
    {
    	ll_node_t* current = list->head;
    	for (unsigned int i = 0; i < n-1; ++i)
    		current = current->next;
    	new_node->next = current->next;
    	current->next = new_node;
    }
    (list->size)++;

}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{


    if(!list)return NULL;

    if (n > list->size - 1)
    n = list->size - 1;
    
    if (list->size==0) return NULL; // verificam daca exista noduri in lista

    ll_node_t* current = list->head;
    ll_node_t* prev = NULL;
    for (unsigned int i = 0; i < n; ++i)
    {
        prev = current;
        current = current->next;
    }
    if (prev) prev->next = current->next;
    else list->head = current->next;
    (list->size)--;
    return current;
  
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t* list)
{

    if(!list)return -1;

    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void
ll_free(linked_list_t** pp_list)
{
    linked_list_t* list = *pp_list;

    if(!list)return;

    ll_node_t* next_node = NULL;
    for (ll_node_t* t = list->head; t; t = next_node)
    {
        next_node = t->next;
        free(t->data);
        free(t);
    }
    free(list);

    *pp_list=NULL;

    pp_list=NULL;

}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void
ll_print_int(linked_list_t* list)
{


    if(!list)return;

    for (ll_node_t* t = list->head; t; t = t->next)
    	printf("%d ", *(int*)t->data);

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void
ll_print_string(linked_list_t* list)
{

    if(!list)return;

    for (ll_node_t* t = list->head; t; t = t->next)
    	printf("%s ", (char*)t->data);

    printf("\n");
}

void
ll_add_middle_node(linked_list_t* list, const void* new_data)
{

    if(!list)return;

    ll_add_nth_node(list, list->size/2, new_data);
}
