#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"
#include "utils.h"

list_graph_t*
lg_create(int nodes)
{
	list_graph_t *new = malloc(sizeof(*new));
	DIE(new == NULL, "Eroare alocare");

	new->nodes = nodes;
	new->neighbors = malloc(nodes * sizeof(linked_list_t*));
	DIE(new->neighbors == NULL, "Eroare alocare");
	for (int i = 0; i < nodes; i++)
	{
		new->neighbors[i] = ll_create(sizeof(int));
	}

	return new;
}

void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
	if (!graph || src >= graph->nodes || dest >= graph->nodes) return;

	ll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dest);
}

int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	if (!graph || src >= graph->nodes || dest >= graph->nodes) return 0;
	ll_node_t* aux;

	for (aux = graph->neighbors[src]->head; aux; aux=aux->next)
	{
		if (*(int*)aux->data == dest) return 1;
	}
	return 0;
}

linked_list_t*
lg_get_neighbours(list_graph_t* graph, int node)
{
	if (!graph ||node >= graph->nodes) return NULL;

	return graph->neighbors[node];
}

void
lg_remove_edge(list_graph_t* graph, int src, int dest)
{
	if (!graph || src >= graph->nodes || dest >= graph->nodes) return;

	ll_node_t* aux;
	int i=0;
	for (aux = graph->neighbors[src]->head; aux; aux=aux->next)
	{
		if (*(int*)aux->data == dest)
			break;
		i++;
	}
	if (aux)
	{
		ll_remove_nth_node(graph->neighbors[src], i);
		free(aux->data);
		free(aux);
	}

}

void
lg_free(list_graph_t* graph)
{
	if (!graph) return;

	for (int i = 0; i < graph->nodes; i++)
	{
		ll_free(&graph->neighbors[i]);
	}
	free(graph->neighbors);
	free(graph);
}