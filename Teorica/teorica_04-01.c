
#define NV 10 // Valor qualquer
typedef struct aresta 
{
    int dest; int custo;
    struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GraphM [NV][NV];

// Programação Dinâmica

/* Fecho transitivo de um grafo orientado

Se (i,k), (k,j) ∈ E ^ (i,j) ∉ E, entao (i,j) é adicionado a E

*/

void warshall (GraphM G, GraphM R, int n) // este algoritmo nao foi obtido por programação dinâmica
{
    int i, j, k;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
            R[i][j] = G[i][j];

    for (k=0 ; k<n; k++)
        for (i=0 ; i<n ; i++)
            for (j=0 ; j<n ; j++)
                if (R[i][k] && R[k][j]) R[i][j] = 1;
}



/* Sequencia de Fibonacci

    A implementaçao através da definição:

        fib(1) = 1
        fib(2) = 1
        fib(n) = fib(n-1) + fib(n-2)

    É má porque efetua muitos calculos repetidos (expandindo fib(n-1)):

        fib(n) = 2*fib(n-2) + fib(n-3)
    
    E tem um tempo de execução T(n) ∈ O(exp(n))

    Através de um cálculo bottom-up, armazenando (memoizing) os resultados num vetor permite um tempo O(n)
*/

int fibonacci (int n)
{
    int k ,  fib[n];  // devia ser n+1 ou os inidices começarem em 0 ao inves de 1

    fib[1] = 1;
    fib[2] = 1;
    for (k=3 ; k<=n ; k++)
        fib[k] = fib[k-1] + fib[k-2];
    return fib[n];
}

// no entanto apenas precisamos dos 2 ultimos valores, logo 

int Fibonacci (int n)
{
    int k, a=1, b=1, t;
    for (k=3 ; k<=n ; k++)
    {
        t = a+b;
        b=a;
        a=t;
    }
    return a;
}

// permitindo apenas usar espaço O(1)

// Programação dinâmica basicamente começa por reduzir o problema usando alguma estrutura auxiliar e depois diminuir o espaço utilizado como auxilio

int min (int i, int j)
{
    return (i<j) ?i :j;
}

/* Distância entre vértices de um gráfo

    d0(i,j) = wij
    dk(1,j) = min (dk(i,j), dk(i,k) + dk(k,j))

    através da memoization:
*/

void memoDistances (GraphM G, GraphM D[NV], int n)
{
    int i, j, k;

    for (i=0; i<n; i++)
        for (j=0; j<n ; j++)
            D[0][i][j] = G[i][j];

    for (k=0 ; k<n ; k++)
        for (i=0 ; i<n ; i++)
            for (j=0 ; j<n ; j++)
                D[k+1][i][j] = min(D[k][i][j], D[k][i][k] + D[k][k][j]);
}

// passando para a programação dinamica

void dynDistances (GraphM G, GraphM D, int n)
{
    int i, j, k;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
            D[i][j] = G[i][j];

    for (k=0 ; k<n; k++)
        for (i=0 ; i<n ; i++)
            for (j=0 ; j<n ; j++)
                D[i][j] = min(D[i][j], D[i][k]+D[k][j]);
}

// otimização para permitir saber o caminho para chegar de um vértice a outro para alem da distancia do caminho

void FloydWarshall (GraphM G, GraphM D, int P[NV][NV], int n)
{
    int i, j, k;
    for (i=0; i<n; i++)
        for (j=0; j<n; j++)
        {
            D[i][j] = G[i][j];
            P[i][j] = -11;
        }

    for (k=0 ; k<n; k++)
        for (i=0 ; i<n ; i++)
            for (j=0 ; j<n ; j++)
                if (D[i][k] + D[k][j] < D[i][j])
                {
                    D[i][j] = D[i][k]+D[k][j];
                    P[i][j] = k;                // k é um vértice intermédio presente no caminho mais curto de i para j
                }
}