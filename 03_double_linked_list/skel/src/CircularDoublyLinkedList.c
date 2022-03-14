#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CircularDoublyLinkedList.h"
#include "utils.h"

/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
    doubly_linked_list_t* new = malloc(sizeof(*new));
    DIE(new == NULL, "Eroare alocare");
    new->data_size = data_size;
    new->head = NULL;
    new->size = 0;
    return new;

}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    if (!list)return NULL;
    if (!list->head)return NULL;
    if (n >= list->size)
        n = list->size % n;
    dll_node_t* aux = list->head;
    for(int i = 0; i < n; i++)
        aux = aux->next;
    return aux;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data)
{
    if (!list) return;
    dll_node_t* new = malloc(sizeof(*new));
    DIE(new == NULL, "Eroare alocare");
    new->data = malloc(list->data_size);
    DIE(new->data == NULL, "Eroare alocare");
    memcpy(new->data, data, list->data_size);

    if (!list->head) {
        new->next = new;
        new->prev = new;
        list->head = new;
    }
    else {
        if (n >= list->size) n = list->size;

        dll_node_t* old = dll_get_nth_node(list, n);
        new->next = old;
        new->prev = old->prev;
        old->prev->next = new;
        old->prev = new;
        if (n == 0)
            list->head = new;
    }
    list->size++;

}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
dll_node_t*
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    if (!list||!list->head) return NULL;
    dll_node_t *elim;
    if (list->size == 1) {
        elim = list->head;
        list->head = NULL;
    }
    else {
        if (n >= list->size - 1) n = list->size-1;
        elim = dll_get_nth_node(list, n);
        elim->prev->next = elim->next;
        elim->next->prev = elim->prev;
        if (elim == list->head)
            list->head = elim->next;
    }
    list->size--;
    return elim;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
dll_get_size(doubly_linked_list_t* list)
{
    if (!list) return 0;
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void
dll_free(doubly_linked_list_t** pp_list)
{
    doubly_linked_list_t* list = *pp_list;
    if (!list) return;
    while(list->head)
    {
        dll_node_t* aux = dll_remove_nth_node(list, 0);
        free(aux->data);
        free(aux);
    }
    free(list);
    pp_list=NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void
dll_print_int_list(doubly_linked_list_t* list)
{
    if (!list) return;
    if (!list->head) return;
    
    dll_node_t* aux = list->head;
    do
    {
        printf("%d ", *(int*)aux->data);
        aux = aux->next;
    } while (aux != list->head);
    

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void
dll_print_string_list(doubly_linked_list_t* list)
{
    if (!list) return;
    if (!list->head) return;
    
    dll_node_t* aux = list->head;
    do
    {
        printf("%s ", (char*)aux->data);
        aux = aux->next;
    } while (aux != list->head);


    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_left_circular(dll_node_t* start)
{
    if (!start) return;
    
    dll_node_t* aux = start;
    do
    {
        printf("%d ", *(int*)aux->data);
        aux = aux->prev;
    } while (aux != start);

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_right_circular(dll_node_t* start)
{
    if (!start) return;
    
    dll_node_t* aux = start;
    do
    {
        printf("%d ", *(int*)aux->data);
        aux = aux->next;
    } while (aux != start);
    
    printf("\n");
}
