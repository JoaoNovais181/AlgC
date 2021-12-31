
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


unsigned hash(char *str){
    unsigned hash = 5381;
    int c;
    
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

#define Size 10
typedef struct nodo {
    char *chave; int ocorr;
    struct nodo *prox;
} Nodo, *THash [Size];


void initEmpty (THash t)
{
    int i = Size;

    while(i)
        t[--i] = NULL;
}

Nodo *createNodo (char *chave)
{
    Nodo *nodo = malloc(sizeof(struct nodo));
    nodo->chave = strdup(chave);
    nodo->ocorr = 1;
    nodo->prox  = NULL;

    return nodo;
}

void add (char *s, THash t)
{
    int indx = hash(s) % Size;

    if (!t[indx])
    {
        t[indx] = createNodo(s);
        return;
    }

    Nodo *ant = NULL, *atual = t[indx];
    while (atual)
    {
        if (!strcmp(atual->chave, s)) 
            atual->ocorr++;
        ant = atual;
        atual = atual->prox;
    }
    
    atual = createNodo(s);
    ant->prox = atual;
}

int lookup (char *s, THash t)
{
    int indx = hash(s) % Size; 

    if (!t[indx])
        return -1;

    Nodo *atual = t[indx];
    while (atual)
    {
        if (!strcmp(atual->chave, s)) 
            return atual->ocorr;
        atual = atual->prox;
    }
    
    return -1;
}

int Remove (char *s, THash t)
{
    int indx = hash(s) % Size;

    if (!t[indx])
    {
        return -1;
    }

    Nodo *ant = NULL, *atual = t[indx];
    while (atual)
    {
        if (!strcmp(atual->chave, s)) 
        {
            atual->ocorr--;
            if (!atual->ocorr)
            {
                if (atual->prox)
                    ant->prox = atual->prox;
                else
                    ant->prox = NULL;
            }
            return 0;
        }
        ant = atual;
        atual = atual->prox;
    }
    return -1;    
}
