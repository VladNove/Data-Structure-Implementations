#include <stdio.h>
#include <stdlib.h>

#include "MatrixGraph.h"
#include "utils.h"

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */
matrix_graph_t*
mg_create(int nodes)
{
	matrix_graph_t* new = malloc(sizeof(*new));
	DIE(!new , "eroare alocare");

	new->nodes = nodes;

	new->matrix = malloc(nodes*sizeof(int*));
	DIE(!new->matrix , "eroare alocare");

	for (int i = 0; i < nodes; i++)
	{
		new->matrix[i] = calloc(nodes, sizeof(int));
		DIE(!new->matrix[i] , "eroare alocare");
	}
	
	return new;
}

/* Adauga o muchie intre nodurile sursa si destinatie */
void
mg_add_edge(matrix_graph_t* graph, int src, int dest)
{
	if (!graph || src >= graph->nodes || dest >= graph->nodes) return;
	graph->matrix[src][dest] = 1;
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int
mg_has_edge(matrix_graph_t* graph, int src, int dest)
{
	if (!graph || src >= graph->nodes || dest >= graph->nodes) return 0;
	return graph->matrix[src][dest];
}

/* Elimina muchia dintre nodurile sursa si destinatie */
void
mg_remove_edge(matrix_graph_t* graph, int src, int dest)
{
	if (!graph || src >= graph->nodes || dest >= graph->nodes) return;
	graph->matrix[src][dest] = 0;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void
mg_free(matrix_graph_t* graph)
{
	if (!graph) return;
	for (int i = 0; i < graph->nodes; i++)
	{
		free(graph->matrix[i]);
	}
	free(graph->matrix);
	free(graph);
}