#include <stdlib.h>
#include <stdio.h>



#define NV 10

typedef struct aresta 
{
    int dest; int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM [NV][NV];

void printGrafoLNoCusto (GrafoL g, int nv);
void printLAdjNoCusto (LAdj l);

LAdj createLAdj (int dest, int custo)
{
    LAdj new = malloc(sizeof(struct aresta));
    new->dest = dest;
    new->custo = custo;
    new->prox = NULL;
    return new;
}

void addViz (GrafoL grafo, int ori, int dest, int custo)
{
    LAdj viz = createLAdj(dest, custo);
    if (!grafo[ori]) 
    {
        grafo[ori] = viz;
        return;
    }
    LAdj ant = grafo[ori], atual = ant->prox;
    while(atual)
    {
        if (atual->dest == dest) return;
        ant = ant->prox;
        atual = ant->prox;
    }
    ant->prox = viz;
}

void initGrafoL (GrafoL grafo, int nv)
{
    for (int i = 0 ; i<nv ; i++)
        grafo[i] = NULL;
}

void printLAdjCusto (LAdj l)
{
    LAdj copia = l;
    while (copia)
    {
        printf(" -%2d-> %2d", copia->custo, copia->dest);
        copia = copia->prox;
    }
    printf(" /\n");
}

void printLAdjNoCusto (LAdj l)
{
    LAdj copia = l;
    while (copia)
    {
        printf(" -> %d", copia->dest);
        copia = copia->prox;
    }
    printf(" /\n");
}

void printGrafoLCusto (GrafoL g, int nv)
{
    for (int i = 0 ; i<nv ; i++)
    {
        printf("%2d", i);
        printLAdjCusto(g[i]);
    }
}

void printGrafoLNoCusto (GrafoL g, int nv)
{
    for (int i = 0 ; i<nv ; i++)
    {
        printf("%2d", i);
        printLAdjNoCusto(g[i]);
    }
}

void fromMat (GrafoM in, GrafoL out)
{
    for (int i=0 ; i<NV ; i++)
        for (int j=0 ; j<NV ; j++)
            if (in[i][j]) addViz(out, i, j, in[i][j]);
}

void inverte (GrafoL in, GrafoL out)
{
    for (int i=0 ; i<NV ; i++)
        for (LAdj aux = in[i] ; aux ; aux = aux->prox)
            addViz(out, aux->dest, i, aux->custo);
}

int max (int arr[], int N)
{
    int r = arr[0];
    for (int i=1 ; i<NV ; i++) 
        if (arr[i]>r) r = arr[i];

    return r;
}

int inDegree (GrafoL g)
{
    int indegree[NV] = {0};

    for (int i=0 ; i<NV ; i++)
        for (LAdj aux = g[i] ; aux ; aux = aux->prox)
            indegree[aux->dest]++;

    return max(indegree, NV);
}

int colorOK (GrafoL g, int cor[])
{
    for (int i=0 ; i<NV ; i++)
        for (LAdj aux = g[i] ; aux ; aux = aux->prox)
            if (i!=aux->dest && cor[i] == cor[aux->dest]) return 0;
    return 1;
}

int lookUp (LAdj l, int dest)
{
    for (LAdj aux = l ; aux ; aux=aux->prox)
        if (aux->dest == dest) return 1;
    return 0;
}

int homomorfOK (GrafoL g, GrafoL h, int f[])
{
    for (int i=0 ; i<NV ; i++)
        for (LAdj aux = g[i] ; aux ; aux=aux->prox)
            if (!lookUp(h[f[i]], f[aux->dest])) 
            {
                printf("\ni:%d j:%d f(i):%d f(j):%d\n", i, aux->dest, f[i], f[aux->dest]);
                return 0;
            }
    return 1;
}


// TRAVESSIAS

int DF (GrafoL g, int or, int v[], int p[], int l[]);
int DFRec (GrafoL g, int or, int v[], int p[], int l[]);
void printarr (int arr[], int N);

int DF (GrafoL g, int or, int v[], int p[], int l[])   // Depth-First
{
    int i;
    for (i=0; i<NV; i++) {
        v[i]=0;
        p[i] = -1;
        l[i] = -1;
    }
    p[or] = -1; l[or] = 0;
    return DFRec (g,or,v,p,l);
}

int DFcaminho (GrafoL g, int or, int v[], int p[], int l[], int nv)   // Depth-First
{
    int i;
    for (i=0; i<nv; i++) {
        v[i]=0;
        p[i] = -1;
        l[i] = -1;
    }
    p[or] = -1; l[or] = 0;
    return DFRec (g,or,v,p,l);
}

int DFRec (GrafoL g, int or, int v[], int p[], int l[])
{
    int i; 
    LAdj a;
    i=1;
    v[or]=-1;
    for (a=g[or]; a!=NULL; a=a->prox)
        if (!v[a->dest])
        {
            p[a->dest] = or;
            l[a->dest] = 1+l[or];
            i+=DFRec(g,a->dest,v,p,l);
        }
    v[or]=1;

    return i;
}

int BF (GrafoL g, int or, int v[], int p[], int l[])  // Breadth-First
{
    int i, x; 
    LAdj a;
    int q[NV], front, end;
    for (i=0; i<NV; i++) 
    {
        v[i]=0;
        p[i] = -1;
        l[i] = -1;
    }
    front = end = 0;
    q[end++] = or; //enqueue
    v[or] = 1; p[or]=-1;l[or]=0;
    i=1;
    while (front != end)
    {
        x = q[(front++)%NV]; //dequeue
        for (a=g[x]; a!=NULL; a=a->prox)
            if (!v[a->dest])
            {
                i++;
                v[a->dest]=1;
                p[a->dest]=x;
                l[a->dest]=1+l[x];
                q[(end++)%NV]=a->dest; //enqueue
            }
    }
    return i;
}

int BFcaminho (GrafoL g, int or, int v[], int p[], int l[], int nv)  // Breadth-First
{
    int i, x; 
    LAdj a;
    int q[nv], front, end;
    for (i=0; i<nv; i++) 
    {
        v[i]=0;
        p[i] = -1;
        l[i] = -1;
    }
    front = end = 0;
    q[end++] = or; //enqueue
    v[or] = 1; p[or]=-1;l[or]=0;
    i=1;
    while (front != end)
    {
        x = q[(front++)%nv]; //dequeue
        for (a=g[x]; a!=NULL; a=a->prox)
            if (!v[a->dest])
            {
                i++;
                v[a->dest]=1;
                p[a->dest]=x;
                l[a->dest]=1+l[x];
                q[(end++)%nv]=a->dest; //enqueue
            }
    }
    return i;
}

int maxInd (int arr[], int N)
{
    int r = 0, m = arr[0];
    for (int i=1 ; i<NV ; i++) 
        if (arr[i]>m) { r = i; m = arr[i]; }

    return r;
}

int maisLonga (GrafoL g, int or, int p[])
{
    int v[NV], p2[NV], l[NV];
    BF(g, or, v, p2, l);
    int i = maxInd(l, NV);

    p[0] = i;
    for (int j = i, indx=1 ; j!=or ; j=p2[j], indx++) p[indx] = p2[j];

    return max(l, NV);
}

void printarr (int arr[], int N)
{
    printf("\n{ ");
    int i;
    for (i = 0 ; i<N-1 ; i++) printf("%d , ", arr[i]);
    printf("%d }\n", arr[i]);
}

int BFaux (GrafoL g, int or, int v[], int c[])
{
    if (v[or]) return 0;
    int i; 
    int q[NV], front, end;
    front = end = 0; i = 1;
    q[end++] = or; //enqueue
    v[or] = 1; c[or] = or;
    while (front != end)
    {
        int x = q[(front++)%NV]; //dequeue
        for (LAdj a=g[x]; a!=NULL; a=a->prox)
            if (!v[a->dest])
            {
                i++;
                v[a->dest]=1; c[a->dest]=or;
                q[(end++)%NV]=a->dest; //enqueue
            }
    }
    return i;
}

int componentes (GrafoL g, int c[])
{
    int N = 0, v[NV] = {0};

    for (int i=0 ; i<NV ; i++)
        N+= BFaux(g, i, v, c);

    return N;
}

int BFaux2 (GrafoL g, int or, int v[], int ord[], int indx)
{
    if (v[or]) return 0;
    int q[NV], front, end;
    front = end = 0;
    q[end++] = or; //enqueue
    v[or] = 1; ord[indx++] = or;
    while (front != end)
    {
        int x = q[(front++)%NV]; //dequeue
        for (LAdj a=g[x]; a!=NULL; a=a->prox)
            if (!v[a->dest])
            {
                v[a->dest]=1; ord[indx++]=a->dest;
                q[(end++)%NV]=a->dest; //enqueue
            }
    }
    return indx;
}

int ordTop (GrafoL g, int ord[])
{
    int N = 0, v[NV] = {0};

    for (int i=0 ; i<NV ; i++)
        N += BFaux2(g, i, v, ord, N);

    return N;
}

int caminho (int L, int C, char *mapa[L], int ls, int cs, int lf, int cf)
{
    LAdj grafo[L*C];
    initGrafoL(grafo, L*C);

    for (int i=0 ; i<L ; i++)
            for (int j=0 ; j<C ; j++)
                if (mapa[i][j] == ' ') 
                {
                    if (i>0   && mapa[i-1][j] == ' ') addViz(grafo, i*C+j, (i-1)*C+j, 1);
                    if (i<L-1 && mapa[i+1][j] == ' ') addViz(grafo, i*C+j, (i+1)*C+j, 1);
                    if (j>0   && mapa[i][j-1] == ' ') addViz(grafo, i*C+j, i*C+(j-1), 1);
                    if (j<C-1 && mapa[i][j+1] == ' ') addViz(grafo, i*C+j, i*C+(j+1), 1);
                }                
    int v[L*C], p[L*C], l[L*C];
    BFcaminho(grafo, ls*C+cs, v, p, l, L*C);

    
    return l[lf*C+cf];
}

int main ()
{
    // GrafoM in = {{0,1,0,0,0,0},
    //              {0,1,0,1,1,0},
    //              {0,0,0,0,0,1},
    //              {1,0,0,0,1,0},
    //              {0,0,0,1,0,0},
    //              {0,0,0,0,0,0}};
    // GrafoM in2 = {{0,0,0,0,0,0},
    //               {0,1,1,0,0,1},
    //               {0,1,0,0,0,1},
    //               {0,0,0,0,1,0},
    //               {0,0,0,0,0,0},
    //               {0,0,1,0,0,0}};
    
    // GrafoL g, g2, g3;
    // initGrafoL(g);
    // initGrafoL(g2);
    // initGrafoL(g3);
    // fromMat(in,g);
    // fromMat(in2,g3);
    // inverte(g, g2);

    // int cor[NV] = {0, 1, 1, 2, 3, 0};
    // int f[NV] = {2, 1, 4, 5, 2, 3};
    // int p[NV];
    // for (int i = 0 ; i<NV ; i++)
    //     p[i] = -1;
    // int ord[NV];

    char *mapa [10] = {"##########"
                      ,"# #   #  #"
                      ,"# # # #  #"
                      ,"#   # #  #"
                      ,"##### #  #"
                      ,"#     #  #"
                      ,"## ####  #"
                      ,"#  #     #"
                      ,"#     #  #"
                      ,"##########"};

    printf("\n%d\n", caminho(10, 10, mapa, 1, 1, 1, 8));

    return 0;
}