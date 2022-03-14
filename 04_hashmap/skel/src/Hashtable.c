/*
 * Hashtable.c
 * Alexandru-Cosmin Mihai
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "Hashtable.h"

#define MAX_BUCKET_SIZE 64

/*
 * Functii de comparare a cheilor:
 */
int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int
hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t* new_table = malloc(sizeof(*new_table));
	DIE(new_table == NULL, "eroare alocare\n");
	new_table->size=0;
	new_table->hmax=hmax;
	new_table->hash_function=hash_function;
	new_table->compare_function=compare_function;

	new_table->buckets = malloc(hmax*sizeof(linked_list_t *));
	DIE(new_table->buckets == NULL, "eroare alocare\n");
	for (unsigned int i = 0; i < hmax; i++)
		new_table->buckets[i] = ll_create(sizeof(struct info));

	
	return new_table;
}

/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune tipul ei), in momentul in care
 * se creeaza o noua intrare in hashtable (in cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie
 * a valorii la care pointeaza key si adresa acestei copii trebuie salvata in structura info asociata intrarii din ht.
 * Pentru a sti cati octeti trebuie alocati si copiati, folositi parametrul key_size_bytes.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel put(ht, key_actual, value_actual),
 * valoarea la care pointeaza key_actual poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia unei intrari din hashtable.
 * Nu ne dorim acest lucru, fiindca exista riscul sa ajungem in situatia in care nu mai stim la ce cheie este
 * inregistrata o anumita valoare.
 */
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	linked_list_t* bucket = ht->buckets[ht->hash_function(key) % ht->hmax];
	ll_node_t* aux_node = bucket->head;
	while(aux_node){
		struct info* aux_pair = aux_node->data;
		if (ht->compare_function(aux_pair->key, key) == 0)
		{
			free(aux_pair->value);
			aux_pair->value = malloc(value_size);
			memcpy(aux_pair->value, value, value_size);
			return;
		}
		aux_node=aux_node->next;
	}

	struct info new_pair;
	new_pair.key = malloc(key_size);
	new_pair.value = malloc(value_size);
	memcpy(new_pair.key, key, key_size);
	memcpy(new_pair.value, value, value_size);
	ll_add_nth_node(bucket, 0, &new_pair);
	ht->size++;
}

void *
ht_get(hashtable_t *ht, void *key)
{
	linked_list_t* bucket = ht->buckets[ht->hash_function(key) % ht->hmax];
	ll_node_t* aux_node = bucket->head;
	while(aux_node){
		struct info* aux_pair = aux_node->data;
		if (ht->compare_function(aux_pair->key, key) == 0)
			return aux_pair->value;
		aux_node=aux_node->next;
	}
	return NULL;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable folosind functia put
 * 0, altfel.
 */
int
ht_has_key(hashtable_t *ht, void *key)
{
	if(ht_get(ht, key)) return 1;
	return 0;
}

/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o intrare din hashtable (adica memoria
 * pentru copia lui key --vezi observatia de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void
ht_remove_entry(hashtable_t *ht, void *key)
{
	linked_list_t* bucket = ht->buckets[ht->hash_function(key) % ht->hmax];
	ll_node_t* aux_node = bucket->head;
	int i = 0;
	while(aux_node){
		struct info* aux_pair = aux_node->data;
		if (ht->compare_function(aux_pair->key, key) == 0 )
			break;
		aux_node=aux_node->next;
		i++;
	}
	if(aux_node)
	{
		ll_remove_nth_node(bucket, i);
		free(((struct info*)aux_node->data)->value);
		free(((struct info*)aux_node->data)->key);
		free(aux_node->data);
		free(aux_node);
	}
	ht->size--;

}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable, dupa care elibereaza si memoria folosita
 * pentru a stoca structura hashtable.
 */
void
ht_free(hashtable_t *ht)
{
    if (ht == NULL)
        return;

    for (unsigned int i = 0; i < ht->hmax; i++)
	{
		while(ht->buckets[i]->head)
		{
			ll_node_t* aux_node = ll_remove_nth_node(ht->buckets[i], 0);
			if(aux_node)
			{
				free(((struct info*)aux_node->data)->value);
				free(((struct info*)aux_node->data)->key);
				free(aux_node->data);
				free(aux_node);
			}
		}
		ll_free(&ht->buckets[i]);
	}
	
	free(ht->buckets);
    free(ht);
    

}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}

void
eliminare_conditie(hashtable_t *ht, int (*cond)(void*))
{
    if (ht == NULL)
        return;

    for (unsigned int i = 0; i < ht->hmax; i++)
	{
        ll_node_t* aux_node = ht->buckets[i]->head;
		while(aux_node)
		{
            ll_node_t* next_node = aux_node->next;
            struct info* entry = aux_node->data;

			if(cond(entry->value))
			{
				ht_remove_entry(ht, entry->key);
			}
            aux_node = next_node;
		}
	}
}

void
ht_double(hashtable_t *ht, float raport)
{
    if (ht == NULL) return;

	if ((float)ht->size/ht->hmax <= raport) return;

	hashtable_t *ht_new;
	ht_new = ht_create(ht->hmax*2, ht->hash_function, ht->compare_function);


    for (unsigned int i = 0; i < ht->hmax; i++)
	{
		while(ht->buckets[i]->head)
		{
			ll_node_t* aux_node = ll_remove_nth_node(ht->buckets[i], 0);
			if(aux_node)
			{
				struct info* entry = (struct info*)aux_node->data;
				int new_hash = ht->hash_function(entry->key) % ht_new->hmax;
				ll_add_nth_node(ht_new->buckets[new_hash], 0, entry);


				//free(entry->value);
				//free(entry->key);
				free(aux_node->data);
				free(aux_node);
			}
		}
		ll_free(&ht->buckets[i]);
	}
	
	free(ht->buckets);
	ht->buckets = ht_new->buckets;
	ht->hmax = ht_new->hmax;
    free(ht_new);
    

}
