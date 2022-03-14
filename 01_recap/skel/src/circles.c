//Novetschi Vlad

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

typedef struct
{
	int x,y,r;
}cerc;

int intersect(cerc a, cerc b)
{
	int distcentre=(a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
	if (distcentre<=(a.r+b.r)*(a.r+b.r)) return 1;

	return 0;
}

int main()
{
    int n, rez=0, i, j;
    cerc* cercuri;
    
    scanf("%d", &n);
    cercuri=malloc(n*sizeof(cerc));
    DIE(cercuri==NULL, "Eroare la alocare");

    for (i = 0; i < n; ++i)
    {
    	scanf("%d %d %d", &cercuri[i].x, &cercuri[i].y, &cercuri[i].r);

    	for (j = 0; j < i; ++j)
    		if(intersect(cercuri[i],cercuri[j]))
    			rez++;
    }

    free(cercuri);

    printf("%d\n", rez);

}
