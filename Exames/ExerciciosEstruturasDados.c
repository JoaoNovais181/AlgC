#include <stdio.h>
#include <stdlib.h>
// #include <string.h>


#define SIZE 2
typedef struct no  
{
    char matricula[6];
    struct no *next;
} No;

typedef No *Tabela[SIZE];

int hash(char matricula[6])
{
    int idx = 0, val;
    while ((val=*matricula++)) idx += val; 
    return idx%SIZE;
}

int strcmp (char *str1, char *str2)
{
    int i;
    for (i=0 ; str1[i] && str2[i]; i++)
        if (str1[i]!=str2[i]) return str1[i]-str2[i];

    return str1[i] - str2[i];
}

int length (char *str)
{
    int l = 0;
    while (*str++) l++;
    return l;
}

// char *strncpy (char *dest, const char *src, int n)
// {
//     for (int i=0 ; i<n && src[i] ; i++) dest[i] = src[i];
//     dest[n] = 0;
//     return dest;
// }

No *newNo (char matricula[6])
{
    No *r = (No *)malloc(sizeof(struct no));
    for (int i=0 ; i<6 ; i++) r->matricula[i] = matricula[i];
    r->next = NULL;
    return r;
}


int insert(Tabela t, char matricula[6])
{
    int idx = hash(matricula);

    if (!t[idx])    
    {
        t[idx] = newNo(matricula);
        return 0;
    }
    
    No *aux = t[idx], *ant=NULL;

    while (aux) 
    {
        if (!strcmp(aux->matricula, matricula)) return 1;
        ant = aux; aux = aux->next;
    }
    
    ant->next = newNo(matricula);

    return 0;
}

typedef struct nod {
    int info;
    int bal;
    struct nod *esq, *dir;
} Nod;

typedef Nod *Arvore;

int max (int a, int b)
{
    return (a>b) ?a :b;
}

int height (Arvore arv)
{
    if (!arv) return 0;
    return 1 + max(height(arv->esq), height(arv->dir));
}

int bf (Arvore arv)
{
    if (!arv) return 0;
    return height(arv->esq) - height(arv->dir);
}

Arvore rr(Arvore arv)
{
    if (arv && arv->esq)
    {
        Arvore aux = arv->esq;
        arv->esq = aux->dir;
        aux->dir = arv;
        arv->bal = bf(arv);
        aux->bal = bf(aux);
        return aux;
    }
}

typedef struct s {
    char *sin;
    struct s *next;
} Sin;
typedef struct p {
    char *pal;
    Sin *sins;
    struct p *next;
} Pal;
#define TAM 10
typedef Pal *Dic[TAM];
int hash(char *pal);

void sinonimos(Dic d, char *pal)
{
    int idx = hash(pal);
    Pal *aux = d[idx];
    int r=1;
    while (r && aux)
    {
        if (!strcmp(aux->pal, pal)) r=0;
        else aux = aux->next;
    }

    if (r=1)
    {
        printf("Não existe essa palavra no dicionario\n");
    }
    else 
    {
        Sin *aux2 = aux->sins;
        while (aux2) { printf("%s ", aux2->sin); aux2=aux2->next;}
    }
}

typedef int Heap[TAM];
typedef struct nodo {
    int val;
    struct no *esq, *dir;
} Nodo, *Tree;

Tree criaTree (Heap h, int i)
{
    Tree a = NULL;
    if (i<TAM)
    {
        a = malloc(sizeof(struct nodo));
        a->val = h[i];
        a->esq=criaTree(h, 2*i+1);
        a->dir=criaTree(h, 2*i+2);
    }
    return a;
}

int main (int argc, char *args[])
{
    Tabela t;

    for (int i=0 ; i<SIZE ; i++) t[i] = NULL;

    char pal[7] = "AA99BB", str2[7] = "OL56LK";

    insert(t, pal);
    insert(t, str2);

    for (int i=0 ; i<SIZE ; i++)
    {
        printf("idx: %d -> ", i);
        No *aux = t[i];

        while (aux) { printf("%s -> ", aux->matricula); aux = aux->next;}
        putchar('\n');
    }

    return 0;
}