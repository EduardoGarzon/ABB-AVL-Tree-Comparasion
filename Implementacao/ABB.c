#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Contadores globais.
LARGE_INTEGER frequency, start, end;
double elapsed_build = 0;
double elapsed_search = 0;

int comparacoes_insercao = 0;
int comparacoes_busca = 0;

// Função para ler os arquivos de entrada.
int *loadEntriesData(const char *arquivo_de_entrada, const int total_itens);

// Definindo a estrutura do nó da árvore.
typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
} Node;

// Função para criar um novo nó
Node *newNode(int key);

// Função para inserir um elemento na árvore.
Node *insertTree(Node *raiz, int key);

// Função para buscar um elemento na árvore.
int searchTree(Node *raiz, int key);

// Função para liberar memória da árvore.
void freeTree(Node *raiz);

// Percurso Pre-Ordem.
void PreOrderRoute(Node *raiz);

// Percurso In-Order.
void InOrderRoute(Node *raiz);

// Percurso Pos-Ordem.
void PosOrderRoute(Node *raiz);

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("\n\nFALHA AO EXECUTAR! MODO DE USO DOS PARAMETROS:\n\n");
        printf("<arquivo de construcao> <arquivo de consulta> <total de itens>\n");
        printf("<percuso>: <[0] pre-order | [1] in-order | [2] pos-order | [3] none\n\n");
        return 1;
    }

    int total_itens = atoi(argv[3]);

    // Carregando entrada.
    int *numeros = loadEntriesData(argv[1], total_itens);
    int *numeros_consulta = loadEntriesData(argv[2], total_itens);

    Node *raiz = NULL;

    // Inserindo elementos.
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < total_itens; i++)
    {
        raiz = insertTree(raiz, numeros[i]);
    }
    QueryPerformanceCounter(&end);
    elapsed_build = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    // printf("\n\n");
    // PreOrderRoute(raiz);
    // printf("\n\n");

    // Buscando elementos.
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int j = 0; j < total_itens; j++)
    {
        int result = searchTree(raiz, numeros_consulta[j]);
        // result == 1 ? printf("ELEMENTO %d ENCONTRADO.\n") : printf("ELEMENTO %d NAO ENCONTRADO.\n");
    }
    QueryPerformanceCounter(&end);
    elapsed_search = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("\n\n");
    printf("TOTAL DE ELEMENTOS: %d\n", total_itens);
    printf("TOTAL DE COMPARACOES NA INSERCAO: %d\n", comparacoes_insercao);
    printf("TOTAL DE COMPARACOES NA BUSCA: %d\n", comparacoes_busca);
    printf("TEMPO DE EXECUCAO NA CONSTRUCAO: %.8f segundos\n", elapsed_build);
    printf("TEMPO DE EXECUCAO NA CONSULTA: %.8f segundos\n", elapsed_search);

    fflush(stdout);

    freeTree(raiz);

    return 0;
}

int *loadEntriesData(const char *arquivo_de_entrada, const int total_itens)
{
    FILE *arquivo = fopen(arquivo_de_entrada, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    int *vetor_numeros = malloc(total_itens * sizeof(int));
    if (!vetor_numeros)
    {
        printf("Erro ao alocar memória!\n");
        return NULL;
    }

    int valor = 0, index = 0;
    while (fscanf(arquivo, "%d", &valor) == 1)
    {
        vetor_numeros[index] = valor;
        index++;
    }

    fclose(arquivo);

    return vetor_numeros;
}

Node *newNode(int key)
{
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->key = key;
    novo->left = NULL;
    novo->right = NULL;
    return novo;
}

Node *insertTree(Node *raiz, int key)
{
    if (raiz == NULL)
    {
        return newNode(key);
    }

    comparacoes_insercao++;
    if (key < raiz->key)
    {
        raiz->left = insertTree(raiz->left, key);
    }
    else
    {
        comparacoes_insercao++;
        if (key > raiz->key)
        {
            raiz->right = insertTree(raiz->right, key);
        }
        // Se key == raiz->key, não insere duplicatas
    }

    return raiz;
}

int searchTree(Node *raiz, int key)
{
    if (raiz == NULL)
    {
        return 0; // Não encontrado.
    }

    comparacoes_busca++;
    if (key == raiz->key)
    {
        return 1; // Encontrado.
    }

    comparacoes_busca++;
    if (key < raiz->key)
    {
        return searchTree(raiz->left, key);
    }
    else
    {
        return searchTree(raiz->right, key);
    }
}

void freeTree(Node *raiz)
{
    if (raiz != NULL)
    {
        freeTree(raiz->left);
        freeTree(raiz->right);
        free(raiz);
    }
}

void PreOrderRoute(Node *raiz)
{
    if (raiz)
    {
        printf("%d ", raiz->key);
        PreOrderRoute(raiz->left);
        PreOrderRoute(raiz->right);
    }
}

void InOrderRoute(Node *raiz)
{
    if (raiz)
    {
        InOrderRoute(raiz->left);
        printf("%d ", raiz->key);
        InOrderRoute(raiz->right);
    }
}

void PosOrderRoute(Node *raiz)
{
    if (raiz)
    {
        PosOrderRoute(raiz->left);
        PosOrderRoute(raiz->right);
        printf("%d ", raiz->key);
    }
}