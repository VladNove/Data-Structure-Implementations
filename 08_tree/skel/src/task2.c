/**
 * SD, 2020
 * 
 * Lab #8, Binary Tree
 * 
 * Task #2 - Verificați dacă arborele este echilibrat.
 */

#include "binary_tree.h"


int max(int a, int b) {
    return (a>b)?a:b;
}
int abs(int x) {
    return (x>=0)?x:-x;
}

int find_height(b_node_t* nod)
{
    if (nod == NULL) return -1;
    return 1+max(find_height(nod->left),find_height(nod->right));
}

bool is_balanced(b_node_t* nod) {
    if (nod == NULL) return true;
    int left_height = find_height(nod->left);
    int rigth_height = find_height(nod->right);

    if (abs(left_height-rigth_height)>1)
        return false;
    else
        return is_balanced(nod->left) && is_balanced(nod->right);

}

b_node_t* vector_to_tree(int *v, int n, int i)
{
    if (i>=n) return NULL;
    if (v[i] == 0) return NULL;

    b_node_t *new = malloc(sizeof(*new));
    DIE (!new, "eroare alocare");
    
    new->left = vector_to_tree(v, n, i*2+1);
    new->right = vector_to_tree(v, n, i*2+2);

    new->data = malloc(sizeof(int));
    *(int*)new->data = v[i];
    return new;
}

void print_data(void *data)
{
    printf("%d ", *(int *) data);
}
int pow2(int x) {
    int r = 1;
    for (int i = 0; i < x; i++)
        r*=2;
    return r;
    
}

void tree_to_vector(b_node_t* nod, int *v, int i){
    if(!nod)return;
    
    v[i] = *(int*)nod->data;

    tree_to_vector(nod->left, v, 2*i+1);
    tree_to_vector(nod->right, v, 2*i+2);
}

void print_spaces(int x){
    for(int i = 0; i<x; i++)
    printf(" ");
}

//am facut extra o functie care sa afiseze frumos un arbore (cu conditia ca toate elementele sa fie cifre)
void nice_print(b_tree_t* tree) {
    int height = find_height(tree->root);
    int max_nodes = pow2(height+1);
    int aux = max_nodes;
    int *v = calloc(max_nodes, sizeof(int));
    tree_to_vector(tree->root, v, 0);
    int new_layer_index = 0;
    for (int i = 0; i < max_nodes-1; i++)
    {
        if (i == new_layer_index) {
            printf("\n");
            aux/=2;
            print_spaces(aux-1);
            new_layer_index = new_layer_index*2+1;
        }

        if(v[i]!=0) printf("%d",v[i]);
        else printf(" ");

        print_spaces(aux*2-1);
    }
    free(v);



}

int main(void)
{
    int v[100];
    int n = 0;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);

    b_tree_t* tree = b_tree_create(sizeof(int));
    tree->root = vector_to_tree(v, n, 0);

    nice_print(tree);

    if (is_balanced(tree->root))
        printf("\nE echilibrat\n");
    else
        printf("\nNu e echilibrat\n");
    

    b_tree_free(tree, free);

}
