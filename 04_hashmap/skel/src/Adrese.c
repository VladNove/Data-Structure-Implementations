/*
 * Testhashtable_t.c
 * Alexandru-Cosmin Mihai
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"

#define MAX_STRING_SIZE		256

typedef struct Adresa {
	char adr[MAX_STRING_SIZE];
	char oras[MAX_STRING_SIZE];
}Adresa;

typedef struct InfoImobil {
    Adresa Adresa;
	char nume_locatar[MAX_STRING_SIZE];
	unsigned int an_cumparare;
	unsigned int nr_animale;
}InfoImobil;

unsigned int
hash_function_adresa(void *a){
    Adresa* adress = a;
    return hash_function_string(adress->adr)^hash_function_string(adress->oras);
    
}

unsigned int
hash_function_oras(void *a){
    Adresa* adress = a;
    return hash_function_string(adress->oras);
    
}



int
cmp_function_adresa(void *a, void*b){
    Adresa* adress1 = a;
    Adresa* adress2 = b;
    if (strcmp(adress1->oras, adress2->oras))return 1;
    if (strcmp(adress1->adr, adress2->adr))return 1;
    return 0;
    
}

struct InfoImobil new_house(char *adr, char* oras, char* name, int an, int nr)
{
    InfoImobil new;
    strcpy(new.Adresa.adr, adr);
    strcpy(new.Adresa.oras, oras);
    strcpy(new.nume_locatar, name);
    new.an_cumparare = an;
    new.nr_animale = nr;
    return new;

}

static void
print_imobil(InfoImobil *p_info)
{
	printf("Adresa: %s, %s\n", p_info->Adresa.adr, p_info->Adresa.oras);
    printf("Locatar: %s\n", p_info->nume_locatar);
    printf("An cumparare: %d\n", p_info->an_cumparare);
    printf("Nr animale: %d\n\n", p_info->nr_animale);
}

void ht_put_house(hashtable_t *ht, InfoImobil house)
{
    ht_put(ht, &house.Adresa, sizeof(Adresa), &house, sizeof(house));
}

void available_house (hashtable_t *ht, char* adr, char* oras)
{
    Adresa search_adress;
    strcpy(search_adress.adr, adr);
    strcpy(search_adress.oras, oras);
    if (ht_get(ht, &search_adress))
        printf("Your dream house is no longer available\n\n");
    else
        printf("Omg! You can move in right now!\n\n");
}

void search_city (hashtable_t *ht, char* oras)
{
    Adresa search_city;
    strcpy(search_city.oras, oras);
    unsigned int indice_bucket = ht->hash_function(&search_city) % ht->hmax;
    linked_list_t* bucket = ht->buckets[indice_bucket];
    ll_node_t* aux = bucket->head;
    while (aux)
    {
        InfoImobil* info_p = ((struct info*)aux->data)->value;
        if (!strcmp(info_p->Adresa.oras, oras))
        print_imobil(info_p);
        aux = aux->next;
    }
    
}

void print_hash (hashtable_t *ht)
{
    for (unsigned int i = 0; i < ht->hmax; i++)
    {
        printf("Hash %d: ", i);
        ll_node_t *aux = ht->buckets[i]->head;
        while (aux)
        {
            InfoImobil* info_p = ((struct info*)aux->data)->value;
            printf("%s,%s\t", info_p->Adresa.adr, info_p->Adresa.oras);
            aux = aux->next;
        }
        
        printf("\n");
        
    }
    printf("\n");
    
}

int conditie(void* value)
{
    InfoImobil* aux_imobil = value;
    if (aux_imobil->nr_animale > 5)return 1;
    return 0;
}


int
main()
{
	hashtable_t *ht_imobiliare; 
    ht_imobiliare = ht_create(3, hash_function_adresa, cmp_function_adresa);

    hashtable_t *ht_orase;
    ht_orase = ht_create(3, hash_function_oras, cmp_function_adresa);

    //Se adauga datele
    InfoImobil house;
    house = new_house("Str1","Focsani","Vlad", 1990, 2);
    ht_put_house(ht_imobiliare, house);
    ht_put_house(ht_orase, house);
    house = new_house("Str2","Focsani","Razvan", 2010, 6);
    ht_put_house(ht_imobiliare, house);
    ht_put_house(ht_orase, house);
    house = new_house("Str3","Bucuresti","Ion", 1980, 4);
    ht_put_house(ht_imobiliare, house);
    ht_put_house(ht_orase, house);
    house = new_house("Str4","Bucuresti","Andrei", 2005, 4);
    ht_put_house(ht_imobiliare, house);
    ht_put_house(ht_orase, house);
    house = new_house("Str5","Bucuresti","Robert", 2015, 6);
    ht_put_house(ht_imobiliare, house);
    ht_put_house(ht_orase, house);
    house = new_house("Str6","Iasi","Isabelle", 2000, 3);
    ht_put_house(ht_imobiliare, house);
    ht_put_house(ht_orase, house);


    printf("Datele au fost introduse\n\n");

    //verificare disponibilitate adresa
    available_house(ht_imobiliare, "Str2","Focsani"); // ocupat
    available_house(ht_imobiliare, "Str7","Focsani"); // disponibil

    //se afiseaza locuintele din orasul bucuresti
    printf("Locuinte in Bucuresti:\n");
    search_city(ht_orase, "Bucuresti");

    //se elimina locuintele cerute, si se afiseaza din nou
    eliminare_conditie(ht_orase, conditie);
    printf("Au fost eliminate locuintele cu peste 5 animale:\n");
    search_city(ht_orase, "Bucuresti");



    //se adauga mai multe intrari
    house = new_house("Str7","Constanta","Vlad", 1990, 2);
    ht_put_house(ht_imobiliare, house);
    house = new_house("Str8","Brasov","Razvan", 2010, 6);
    ht_put_house(ht_imobiliare, house);
    house = new_house("Str9","Iasi","Ion", 1980, 4);
    ht_put_house(ht_imobiliare, house);
    house = new_house("Str10","Brasov","Andrei", 2005, 4);
    ht_put_house(ht_imobiliare, house);
    house = new_house("Str11","Cluj","Robert", 2015, 6);
    ht_put_house(ht_imobiliare, house);
    house = new_house("Str12","Cluj","Isabelle", 2000, 3);
    ht_put_house(ht_imobiliare, house);

    //se afiseaza hash-urile si key-urile inainte is dupa dublare
    print_hash(ht_imobiliare);
    ht_double(ht_imobiliare, 0.5);
    print_hash(ht_imobiliare);

    //se elibereaza memoria
    ht_free(ht_orase);
    ht_free(ht_imobiliare);
	return 0;
}
