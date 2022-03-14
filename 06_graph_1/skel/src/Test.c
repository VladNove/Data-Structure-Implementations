#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Queue.h"
#include "Stack.h"

#define MAX_NODES 10

void
print_matrix_graph(matrix_graph_t* mg)
{
	if (!mg)return;
	for (int i = 0; i < mg->nodes; i++)
	{
		for (int j = 0; j < mg->nodes; j++)
			printf("%d ", mg->matrix[i][j]);

		printf("\n");
	}
	
}

void
print_list_graph(list_graph_t* lg)
{
	if (!lg)return;
	for (int i = 0; i < lg->nodes; i++)
	{
		printf("%d: ", i);
		ll_print_int(lg->neighbors[i]);
	}
	
}

void
dfs_list_graph(list_graph_t* lg,
		int node,
		int* visited,
		int* t_desc,
		int* t_tin)
{
	// culori : 0 alb, 1 : negru, 2 : gri
	if (!lg ||node >= lg->nodes) return;
	stack_t* st = st_create(sizeof(int));
	int timp=1;

	st_push(st, &node);
	printf("%d ", node); //afisare parcurgere
	visited[node]=2;
	t_desc[node]=timp;
	timp++;

	while (!st_is_empty(st))
	{	
		node = *(int*)st_peek(st);
		linked_list_t *vecini = lg_get_neighbours(lg, node);
		ll_node_t* aux;
		for (aux = vecini->head; aux; aux=aux->next)
		{
			int val = *(int*)aux->data;
			if (!visited[val])
			{
				st_push(st, &val);
				printf("%d ", val); //afisare parcurgere
				visited[val] = 2;
				t_desc[val]=timp;
				timp++;
				break;
			}
		}
		if (!aux)
		{
			visited[node]=1;
			t_tin[node]=timp;
			timp++;
			st_pop(st);
		}
	}
	st_free(st);
}

void
dfs_matrix_graph(matrix_graph_t* mg,
		 int node,
		 int* visited,
		 int* t_desc,
		 int* t_tin)
{
	// culori : 0 alb, 1 : negru, 2 : gri
	if (!mg ||node >= mg->nodes) return;
	stack_t* st = st_create(sizeof(int));
	int timp=1;

	st_push(st, &node);
	printf("%d ", node); //afisare parcurgere
	visited[node]=2;
	t_desc[node]=timp;
	timp++;

	while (!st_is_empty(st))
	{	
		node = *(int*)st_peek(st);
		int i = 0;
		for (i = 0; i<mg->nodes; i++)
		{
			if (mg_has_edge(mg, node, i)&&!visited[i])
			{
				st_push(st, &i);
				printf("%d ", i); //afisare parcurgere
				visited[i] = 2;
				t_desc[i]=timp;
				timp++;
				break;
			}
		}
		if (i>=mg->nodes)
		{
			visited[node]=1;
			t_tin[node]=timp;
			timp++;
			st_pop(st);
		}
	}
	st_free(st);
}

void
bfs_list_graph(list_graph_t* lg, int node, int* color, int* parent)
{
	// culori : 0 alb, 1 : negru, 2 : gri
	if (!lg ||node >= lg->nodes) return;
	queue_t* q = q_create(sizeof(int), lg->nodes);
	q_enqueue(q, &node);
	color[node]=2;
	while (!q_is_empty(q))
	{	
		node = *(int*)q_front(q);
		linked_list_t *vecini = lg_get_neighbours(lg, node);
		ll_node_t* aux;
		for (aux = vecini->head; aux; aux=aux->next)
		{
			int val = *(int*)aux->data;
			if (!color[val])
			{
				color[val] = 2;
				q_enqueue(q, &val);
				parent[val] = node;
			}
		}
		color[node]=1;
		printf("%d ", node); //afisare parcurgere
		q_dequeue(q);
	}
	q_free(q);
}

void
bfs_matrix_graph(matrix_graph_t* mg, int node, int* color, int* parent)
{
	// culori : 0 alb, 1 : negru, 2 : gri
	if (!mg ||node >= mg->nodes) return;
	queue_t* q = q_create(sizeof(int), mg->nodes);
	q_enqueue(q, &node);
	color[node]=2;
	while (!q_is_empty(q))
	{	
		node = *(int*)q_front(q);
		for (int i = 0; i < mg->nodes; i++)
		{
			if (mg_has_edge(mg, node, i))
			{
				if (!color[i])
				{
					color[i] = 2;
					q_enqueue(q, &i);
					parent[i] = node;
				}
			}
		}
		color[node]=1;
		printf("%d ", node); //afisare parcurgere
		q_dequeue(q);
	}
	q_free(q);
}

void
floyd_warshall(matrix_graph_t* mg)
{
	if(!mg)return;
	int **d = mg->matrix;
	int n = mg->nodes;
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (d[i][k]&&d[k][j]&&i!=j)
					if (d[i][j]>d[i][k]+d[k][j] || d[i][j] == 0)
						d[i][j]=d[i][k]+d[k][j];
	
}

int
main()
{
	//int nodes, edges;
	int *color, *parent;
	int *visited, *t_dest, *t_fin;
	list_graph_t* lg = lg_create(MAX_NODES);
	matrix_graph_t* mg = mg_create(MAX_NODES);

	int numbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	printf("-------------------------------- Test ListGraph "
		"--------------------------------\n");
	/* Test add_edge_list_graph has_edge_list_graph */

	lg_add_edge(lg, numbers[0], numbers[1]);
	lg_add_edge(lg, numbers[3], numbers[2]);

	printf("#1:\nOutput: %d\nOutput asteptat: 1\n",
		lg_has_edge(lg, numbers[0], numbers[1]));

	printf("#2:\nOutput: %d\nOutput asteptat: 1\n",
		lg_has_edge(lg, numbers[3], numbers[2]));

	printf("#3:\nOutput: %d\nOutput asteptat: 0\n",
		lg_has_edge(lg, numbers[2], numbers[4]));

	/* Test remove_edge_list_graph */
	lg_remove_edge(lg, numbers[0], numbers[1]);
	printf("#4:\nOutput: %d\nOutput asteptat: 0\n",
		lg_has_edge(lg, numbers[0], numbers[1]));

	/* Test get_neighbours_list_graph */
	lg_add_edge(lg, numbers[0], numbers[1]);
	lg_add_edge(lg, numbers[0], numbers[2]);
	lg_add_edge(lg, numbers[0], numbers[3]);
	lg_add_edge(lg, numbers[0], numbers[4]);

	linked_list_t* l = lg_get_neighbours(lg, numbers[0]);

	printf("#5:\n");
	printf("Output: ");
	ll_print_int(l);
	printf("Output asteptat: 1 2 3 4\n");

	printf("-------------------------------- Test MatrixGraph "
		"--------------------------------\n");
	/* test add edge_matrix_graph has_edge_matrix_graph */
	mg_add_edge(mg, numbers[0], numbers[1]);
	mg_add_edge(mg, numbers[3], numbers[2]);

	printf("#1:\nOutput: %d\nOutput asteptat: 1\n",
		mg_has_edge(mg, numbers[0], numbers[1]));

	printf("#2:\nOutput: %d\nOutput asteptat: 1\n",
		mg_has_edge(mg, numbers[3], numbers[2]));

	printf("#3:\nOutput: %d\nOutput asteptat: 0\n",
		mg_has_edge(mg, numbers[2], numbers[4]));

	/* test remove_edge_matrix_graph */
	mg_remove_edge(mg, numbers[0], numbers[1]);
	printf("#4:\nOutput: %d\nOutput asteptat: 0\n",
		mg_has_edge(mg, numbers[0], numbers[1]));

	lg_free(lg);
	mg_free(mg);


	lg = lg_create(8);
	mg = mg_create(8);
	//exemplul de pe ocw de la parcurgerea in latime.
	//fara muchiile orientate 2->1 3->1 4->1 5->2 6->3.
	lg_add_edge(lg, 1, 2);
	lg_add_edge(lg, 1, 3);
	lg_add_edge(lg, 1, 4);
	lg_add_edge(lg, 2, 3);
	lg_add_edge(lg, 2, 5);
	lg_add_edge(lg, 3, 2);
	lg_add_edge(lg, 3, 6);
	lg_add_edge(lg, 3, 7);
	lg_add_edge(lg, 4, 7);
	lg_add_edge(lg, 7, 3);
	lg_add_edge(lg, 7, 4);

	mg_add_edge(mg, 1, 2);
	mg_add_edge(mg, 1, 3);
	mg_add_edge(mg, 1, 4);
	mg_add_edge(mg, 2, 3);
	mg_add_edge(mg, 2, 5);
	mg_add_edge(mg, 3, 2);
	mg_add_edge(mg, 3, 6);
	mg_add_edge(mg, 3, 7);
	mg_add_edge(mg, 4, 7);
	mg_add_edge(mg, 7, 3);
	mg_add_edge(mg, 7, 4);

	printf("-------------------------------- Test Functii afisare "
		"--------------------------------\n");

	printf("#Test print_list_graph:\n");
	print_list_graph(lg);

	printf("#Test print_matrix_graph:\n");
	print_matrix_graph(mg);

	printf("\n\n-------------------------------- Test Functii BFS "
		"--------------------------------\n");

	//test list_graph
	color = calloc(lg->nodes, sizeof(int));
	parent = calloc(lg->nodes, sizeof(int));

	printf("parcurgere list graph:   ");
	bfs_list_graph(lg, 1, color, parent);
	printf("\nvector parinti:\t");
	for (int i = 1; i < lg->nodes; i++)
	{
		printf("%d ", parent[i]);
	}
	printf("\n\n");
	
	free (color);
	free (parent);

	//test matrix_graph
	color = calloc(mg->nodes, sizeof(int));
	parent = calloc(mg->nodes, sizeof(int));

	printf("parcurgere matrix graph: ");
	bfs_matrix_graph(mg, 1, color, parent);
	printf("\nvector parinti:\t");
	for (int i = 1; i < mg->nodes; i++)
	{
		printf("%d ", parent[i]);
	}
	printf("\n");
	
	free (color);
	free (parent);

	printf("\n\n-------------------------------- Test Functii DFS "
		"--------------------------------\n");

	//test list_graph
	visited = calloc(lg->nodes, sizeof(int));
	t_dest = calloc(lg->nodes, sizeof(int));
	t_fin = calloc(lg->nodes, sizeof(int));
	printf("parcurgere list graph:   ");
	dfs_list_graph(lg, 1, visited, t_dest, t_fin);
	printf("\ntimp desc/fin: ");
	for (int i = 1; i < lg->nodes; i++)
	{
		printf("%d:%d/%d  ", i,t_dest[i],t_fin[i]);
	}
	printf("\n");
	free (visited);
	free (t_dest);
	free (t_fin);

	//test matrix_graph
	visited = calloc(mg->nodes, sizeof(int));
	t_dest = calloc(mg->nodes, sizeof(int));
	t_fin = calloc(mg->nodes, sizeof(int));
	printf("\nparcurgere matrix graph: ");
	dfs_matrix_graph(mg, 1, visited, t_dest, t_fin);
	printf("\ntimp desc/fin: ");
	for (int i = 1; i < mg->nodes; i++)
	{
		printf("%d:%d/%d  ", i,t_dest[i],t_fin[i]);
	}
	printf("\n");
	free (visited);
	free (t_dest);
	free (t_fin);
	
	printf("\n\n-------------------------------- Test Floyd Warshall "
		"--------------------------------\n");

	printf("Matrice initiala:\n");
	print_matrix_graph(mg);
	printf("\nFloyd Warshall:\n");
	floyd_warshall(mg);
	print_matrix_graph(mg);

	lg_free(lg);
	mg_free(mg);
	return 0;
}
