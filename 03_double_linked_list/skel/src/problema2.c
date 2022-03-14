#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CircularDoublyLinkedList.h"

#define MAX_STRING_SIZE 64


doubly_linked_list_t* dll_sum(doubly_linked_list_t* ListA, doubly_linked_list_t* ListB)
{
    if (!ListA || !ListB) return NULL;
    dll_node_t* NodeA = ListA->head;
    dll_node_t* NodeB = ListB->head;
    doubly_linked_list_t* Rez = dll_create(sizeof(int));
    while (NodeA || NodeB)
    {
        int r = 0;
        if(NodeA)
            r += *(int*)NodeA->data,
            NodeA=NodeA->next;
        if(NodeA == ListA->head) NodeA = NULL;

        if(NodeB)
            r += *(int*)NodeB->data,
            NodeB = NodeB->next;
        if(NodeB == ListB->head) NodeB = NULL;
        dll_add_nth_node(Rez, Rez->size, &r);
    }
    return Rez;
}

int main() {
    doubly_linked_list_t* ListA=dll_create(sizeof(int));
    doubly_linked_list_t* ListB=dll_create(sizeof(int));
    doubly_linked_list_t* ListC=NULL;
    int n,x;
    printf("lista A: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &x);
        dll_add_nth_node(ListA, i, &x);
    }
    printf("lista B: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &x);
        dll_add_nth_node(ListB, i, &x);
    }

    printf("\n");
    dll_print_int_list(ListA);
    dll_print_int_list(ListB);
    printf("------\n");
    ListC = dll_sum(ListA, ListB);
    dll_print_int_list(ListC);

    dll_free(&ListA);
    dll_free(&ListB);
    dll_free(&ListC);

    
    return 0;
}
