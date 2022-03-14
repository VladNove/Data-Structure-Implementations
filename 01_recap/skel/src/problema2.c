//Novetschi Vlad

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void afisaremat (int **mat, int n, int m)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
			printf("%d ", mat[j][i]);
		printf("\n");
	}
}

int main()
{
    int n, m;
    int **matrice;
    scanf("%d %d", &n, &m);
    matrice=malloc(n*sizeof(int*));
    DIE(!matrice, "eroare alocare");
    for (int i = 0; i < n; ++i)
    {
    	matrice[i]=malloc(m*sizeof(int));
    	DIE(!matrice[i], "eroare  alocare");
    	for (int j = 0; j < m; ++j)
    	{
    		scanf("%d", &matrice[i][j]);
    	}
    }
    afisaremat(matrice, n, m);
    for (int i = 0; i < n; ++i)free(matrice[i]);
    free(matrice);


}
