#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Stack.h"

int match (char x, char y)
{
	if (x=='{'&&y=='}')return 1;
	if (x=='['&&y==']')return 1;
	if (x=='('&&y==')')return 1;
	return 0;
}

int
main(void)
{
	char sir[264];
	int is_correct = 1;
	stack_t* p = st_create(sizeof(char));
	scanf("%s", sir);
	for (unsigned int i = 0; i < strlen(sir); i++)
	{
		if (strchr("{[(", sir[i]))
		{
			if (!st_is_empty(p))
			{
				char aux = *(char*)st_peek(p);
				if (sir[i]=='{' && (aux == '[' || aux == '(')) is_correct = 0;
				if (sir[i]=='[' && aux == '(') is_correct = 0;

				if (!is_correct) break;
			}
			st_push(p,&sir[i]);
		}
		if (strchr(")]}", sir[i]))
		{
			if (st_is_empty(p)) {
				is_correct = 0;
				break;
			}
			char aux = *(char*)st_peek(p);

			if (!match(aux, sir[i])) {
				is_correct = 0;
				break;
			}
			st_pop(p);
		}
	}
	if(st_is_empty(p)&&is_correct)
		printf("Sirul este corect parantezat\n");
	else
		printf("Sirul NU este corect parantezat\n");

	st_free(p);
	
	

	return 0;
}
