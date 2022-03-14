#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListGraph.h"
#include "Queue.h"
#include "Stack.h"
#include "utils.h"


#define MAX_NODES	100
#define STRING	100
#define INF		0x3f3f3f3f

/**
 * Adaugati nodurile componentei conexe curente in lista component. 
 */
static void
dfs_connected_comps(
	list_graph_t *lg,
	int node,
	int *visited,
	linked_list_t *component
) {
	visited[node]=1;
	ll_add_nth_node(component, component->size, &node);
	linked_list_t* neigh = lg_get_neighbours(lg, node);
	for ( ll_node_t *aux = neigh->head; aux; aux = aux->next )
		if(!visited[*(int*)aux->data])
			dfs_connected_comps(lg, *(int*)aux->data, visited, component);

}

static linked_list_t **
connected_components(list_graph_t *lg, int *visited, unsigned int *num_comp)
{
	linked_list_t **comps = malloc(lg->nodes * sizeof(*comps));
	DIE(!comps, "malloc comps failed");

	for (int i = 0; i < lg->nodes; i++)
	{
		if (!visited[i])
		{
			comps[*num_comp] = ll_create(sizeof(int));
			dfs_connected_comps(lg, i, visited, comps[*num_comp]);
			(*num_comp)++;
		}
	}

	return comps;
}

static void
dfs_topo_sort(list_graph_t *lg, int node, int *visited, linked_list_t *sorted)
{
	if (!lg ||node >= lg->nodes) return;
	stack_t* st = st_create(sizeof(int));

	st_push(st, &node);
	//printf("%d ", node); //afisare parcurgere
	visited[node]=2;

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
				// printf("%d ", val); //afisare parcurgere
				visited[val] = 2;
				break;
			}
		}
		if (!aux)
		{
			visited[node]=1;
			ll_add_nth_node(sorted, 0, &node);
			st_pop(st);
		}
	}
	st_free(st);
}

static linked_list_t *
topo_sort(list_graph_t *lg, int *visited)
{
	linked_list_t *sorted = ll_create(sizeof(int));
	
	for(int i = 0; i < lg->nodes; i++)
		if (!visited[i])
			dfs_topo_sort(lg, i, visited, sorted);

	return sorted;
}

static void
min_path(list_graph_t *lg, int start, int *dist)
{
	int node = start;
	dist[node]=0;
	int *color = calloc(MAX_NODES, sizeof(int));
	int *parent = calloc(MAX_NODES, sizeof(int));
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
				dist[val] = dist[parent[val]]+1;
			}
		}
		color[node]=1;
		q_dequeue(q);
	}
	q_free(q);
	free(color);
	free(parent);
}

static int
check_bipartite(list_graph_t *lg, int *color)
{
	int nvisited = 0;
	while (nvisited<lg->nodes)
	{
		for(int i = 0; i<lg->nodes ;i++)
		{
			if (!color[i])
			{
				int node = i;
				queue_t* q = q_create(sizeof(int), lg->nodes);
				q_enqueue(q, &node);
				color[node]=1;
				nvisited++;
				while (!q_is_empty(q))
				{	
					node = *(int*)q_front(q);
					q_dequeue(q);
					linked_list_t *vecini = lg_get_neighbours(lg, node);
					ll_node_t* aux;
					for (aux = vecini->head; aux; aux=aux->next)
					{
						int val = *(int*)aux->data;
						if (!color[val])
						{
							color[val] = (color[node] == 1) ? 2 : 1;
							nvisited++;
							q_enqueue(q, &val);
						}
						else if (color[val]==color[node]) return 0;
					}

				}
				q_free(q);
			}
		}
	}
	/* TODO: multimile vor avea culorile 1, respectiv 2 */
	return 1;
}

static void
test_connected_comp(void)
{
	unsigned int num_comps=0, i, nodes=0, edges=0;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t **comps;

	printf("UNDIRECTED graph for the connected components problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}
	printf("de aicia");
	comps = connected_components(lg, visited, &num_comps);

	printf("Found %d connected components:\n", num_comps);
	for (i = 0; i != num_comps; ++i) {
		ll_print_int(comps[i]);
		ll_free(comps + i);
	}
	printf("\n");

	free(comps);
	lg_free(lg);
}

static void
test_topo_sort(void)
{
	unsigned int i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t *sorted;

	printf("DIRECTED graph for the topological sort problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
	}

	sorted = topo_sort(lg, visited);

	printf("Topologically sorted nodes:\n");
	ll_print_int(sorted);
	printf("\n");

	ll_free(&sorted);
	lg_free(lg);
}

static void
test_min_dist(void)
{
	unsigned int i, nodes, edges;
	int x, y;
	int dist[MAX_NODES] = {0};
	list_graph_t *lg;

	printf("UNDIRECTED graph for the minimum distance problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	for (i = 0; i != nodes; ++i)
		dist[i] = INF;

	min_path(lg, 0, dist);

	printf("Minimum distances to node 0:\n");
	for (i = 0; i != nodes; ++i)
		printf("%u: %u\n", i, dist[i]);
	printf("\n");

	lg_free(lg);
}

static void
test_bipartite(void)
{
	printf("Test bipartite :\n");
	unsigned int i, nodes, edges;
	int color[MAX_NODES] = {0};
	int x, y;
	list_graph_t *lg;

	printf("UNDIRECTED graph for the bipartite graph problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}
	
	if (check_bipartite(lg, color)) {
		printf("Nodes with colour 1:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 1)
				printf("%d ", i);
		printf("\nNodes with colour 2:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 2)
				printf("%d ", i);
		printf("\n");
	} else
		printf("The graph is not bipartite\n");

	lg_free(lg);
}

static void 
gamer_clans()
{
	printf("test clanuri:\n");
	unsigned int num_comps = 0 , i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t **comps;

	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	comps = connected_components(lg, visited, &num_comps);

	printf("%d\n", num_comps);
	for (i = 0; i != num_comps; ++i) {
		ll_print_int(comps[i]);
		ll_free(comps + i);
	}

	free(comps);
	lg_free(lg);
}

int *
min_path2(list_graph_t *lg, int start, int *dist)
{
	int node = start;
	dist[node]=0;
	int *color = calloc(MAX_NODES, sizeof(int));
	int *parent = calloc(MAX_NODES, sizeof(int));
	if (!lg ||node >= lg->nodes) return NULL;
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
				dist[val] = dist[parent[val]]+1;
			}
		}
		color[node]=1;
		q_dequeue(q);
	}
	q_free(q);
	free(color);
	return parent;
}

static void
curier(void)
{
	printf("test curier:\n");
	unsigned int i, nodes, edges, nr_tests;
	int x, y;
	int dist[MAX_NODES] = {0};
	int a, b;
	list_graph_t *lg;

	scanf("%u %u %u", &nodes, &edges, &nr_tests);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	for (i = 0; i != nodes; ++i)
		dist[i] = INF;

	for (i = 0; i < nr_tests; i++) {
		scanf("%d%d", &a, &b);
		for(i = 0; i<nodes; dist[i]=0, i++);
		int *parents = min_path2(lg, b, dist);
		for(i = a; parents[i]!=0; i = parents[i])
		printf("%d ", i);
		printf("%d ", i);
		free(parents);
		printf("\n");
	}

	lg_free(lg);
}

int add_string(char (*materii)[STRING], char* nou, int *n)
{
	for (int i = 0; i < (*n); i++) {
		if (!strcmp(materii[i], nou))
			return i;
	}
	strcpy(materii[*n], nou);
	(*n)++;
	return (*n) - 1;
}

static void
materii(void)
{
	printf("Test materii:\n");
	unsigned int i, nodes, edges;
	char (*materii)[STRING]=malloc(sizeof(*materii)*MAX_NODES);
	char x[STRING], y[STRING];
	int visited[MAX_NODES] = {0};
	int ind1, ind2, n = 0;
	list_graph_t *lg;
	linked_list_t *sorted;

	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%s %s", x, y);
		ind1 = add_string(materii, x, &n);
		ind2 = add_string(materii, y, &n);

		lg_add_edge(lg, ind1, ind2);
	}

	sorted = topo_sort(lg, visited);

	for (ll_node_t* current = sorted->head; current; current = current->next)
		printf("%s\n", materii[*(int*)current->data]);
		
	printf("\n");

	free(materii);
	ll_free(&sorted);
	lg_free(lg);
}

void hamilton_dfs(list_graph_t* lg, int node, int n, int* visited, stack_t* afis)
{
	if (n<0 && node == 0)
	{
		for(ll_node_t* current = afis->list->head; current; current = current->next)
			printf("%d ", *(int*)current->data);
		printf("0\n");
	}
		
	else if (n>=0)
	{
		linked_list_t *neigh = lg_get_neighbours(lg, node);
		for(ll_node_t* current = neigh->head; current; current = current->next)
			if (!visited[*(int*)current->data])
			{
			st_push(afis, current->data);
			visited[*(int*)current->data]=1;
			hamilton_dfs(lg, *(int*)current->data, n-1, visited, afis);
			visited[*(int*)current->data]=0;
			st_pop(afis);
			}
	}
	
}

static void
hamilton(void)
{
	printf("Test hamilton :\n");
	unsigned int  i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	stack_t *afis = st_create(sizeof(int));
	list_graph_t *lg;

	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	hamilton_dfs(lg, 0, lg->nodes-1, visited, afis);

	printf("\n");

	lg_free(lg);
	st_free(afis);

}

int
main(void)
{
	/* Ex 1 */
	//test_connected_comp();

	/* Ex 2 */
	//test_topo_sort();

	/* Ex 3 */
	//test_min_dist();

	/* Ex 4 */
	//test_bipartite();

	/* Problema 1 */
	gamer_clans();

	/* Problema 2 */
	curier();

	/* Problema 3 */
	materii();

	/* Problema 4 */
	test_bipartite();

	/* Problema 5 */
	hamilton();

	return 0;
}
