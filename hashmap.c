//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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value)
{
    int hashPosicion;
    hashPosicion =  hash(key, map -> capacity);
    while(map -> buckets[hashPosicion] != NULL && map -> buckets[hashPosicion] -> key != NULL)
    {
        hashPosicion = (hashPosicion + 1) % map -> capacity;
    }
    map -> buckets[hashPosicion] = createPair(key,value);
    map -> size++;
    map -> current = hashPosicion;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    
    int capacidad;
    capacidad = map -> capacity;
    Pair **copiaArreglo = map -> buckets;
    map -> capacity = capacidad * 2;
    map -> buckets = malloc(sizeof(Pair) * (capacidad*2));
    map -> size = 0;
    for(int i = 0 ; i < capacidad ; i++)
    {
        if(copiaArreglo[i] != NULL)
        {
            insertMap(map,copiaArreglo[i] -> key,copiaArreglo[i] -> value);
        }
    }
    free(copiaArreglo);

}


HashMap * createMap(long capacity)
{
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    map -> buckets = malloc(sizeof(Pair*) * capacity);
    map -> size = 0;
    map -> capacity = capacity;
    for(int i  = 0; i < capacity ; i++)
    {
        map -> buckets[i] = NULL;
    }
    map -> current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) 
{    
    int hashPosicion;
    hashPosicion =  hash(key, map -> capacity);

    if(is_equal(map -> buckets[hashPosicion] -> key , key))
    {
        map -> size--;
        map -> buckets[hashPosicion] -> key = NULL;
    }
    else
    {
        while(map -> buckets[hashPosicion] != NULL)
        {
            if(is_equal( map -> buckets[hashPosicion] -> key , key))
            {
                map -> size--;
                map -> buckets[hashPosicion] -> key = NULL;
            }
            hashPosicion = (hashPosicion + 1) % map -> capacity;
        }
    }
}

Pair * searchMap(HashMap * map,  char * key) 
{   
    int hashPosicion;
    hashPosicion =  hash(key, map -> capacity);

    while(map -> buckets[hashPosicion] != NULL)
    {   
        if(is_equal(map -> buckets[hashPosicion] -> key,key))
        {
            map -> current = hashPosicion;
            return map -> buckets[hashPosicion];
        }
        else
        {
            if(hashPosicion == (map -> capacity))
                hashPosicion = 0;
            else
                hashPosicion++; 
        }
    }
    map -> current = hashPosicion;
    return NULL;
}

Pair * firstMap(HashMap * map) {
    int posicion = 0;
    while(map -> buckets[posicion] == NULL || map -> buckets[posicion] -> key == NULL)
    {
        posicion = (posicion + 1) % map -> capacity;
    }
    map -> current = posicion;
    return map -> buckets[posicion];
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
