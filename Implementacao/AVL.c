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

// Definindo a estrutura do nó da árvore (agora com height para AVL).
typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Função para criar um novo nó
Node *newNode(int key);

// Funções AVL
int heightNode(Node *n);
int maxValue(int a, int b);
Node *rightRotate(Node *y);
Node *leftRotate(Node *x);
int getBalance(Node *n);

// Função para inserir um elemento na árvore (AVL)
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
    if (!numeros)
    {
        printf("Erro ao carregar arquivo de construcao: %s\n", argv[1]);
        return 1;
    }
    int *numeros_consulta = loadEntriesData(argv[2], total_itens);
    if (!numeros_consulta)
    {
        printf("Erro ao carregar arquivo de consulta: %s\n", argv[2]);
        free(numeros);
        return 1;
    }

    Node *raiz = NULL;

    // Inserindo elementos.
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < total_itens; i++)
    {
        raiz = insertTree(raiz, numeros[i]);
    }
    QueryPerformanceCounter(&end);
    elapsed_build = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart; // Tempo de construcao.

    // Buscando elementos.
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int j = 0; j < total_itens; j++)
    {
        int result = searchTree(raiz, numeros_consulta[j]);
        (void)result; // não usado aqui
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

    // liberar memoria
    freeTree(raiz);
    free(numeros);
    free(numeros_consulta);

    return 0;
}

int *loadEntriesData(const char *arquivo_de_entrada, const int total_itens)
{
    FILE *arquivo = fopen(arquivo_de_entrada, "r");
    if (!arquivo)
    {
        // printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    int *vetor_numeros = malloc(total_itens * sizeof(int));
    if (!vetor_numeros)
    {
        // printf("Erro ao alocar memória!\n");
        fclose(arquivo);
        return NULL;
    }

    int valor = 0, index = 0;
    while (index < total_itens && fscanf(arquivo, "%d", &valor) == 1)
    {
        vetor_numeros[index] = valor;
        index++;
    }

    fclose(arquivo);

    // opcional: se arquivo tiver menos itens que total_itens, podemos ajustar,
    // mas para o seu caso presumimos que os arquivos têm exatamente total_itens.
    return vetor_numeros;
}

Node *newNode(int key)
{
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->key = key;
    novo->left = NULL;
    novo->right = NULL;
    novo->height = 1; // novo nó é folha -> altura 1
    return novo;
}

int heightNode(Node *n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

int maxValue(int a, int b)
{
    return (a > b) ? a : b;
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // Rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas
    y->height = 1 + max(heightNode(y->left), heightNode(y->right));
    x->height = 1 + max(heightNode(x->left), heightNode(x->right));

    // retorna nova raiz
    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // Rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    x->height = 1 + max(heightNode(x->left), heightNode(x->right));
    y->height = 1 + max(heightNode(y->left), heightNode(y->right));

    // retorna nova raiz
    return y;
}

int getBalance(Node *n)
{
    if (n == NULL)
        return 0;
    return heightNode(n->left) - heightNode(n->right);
}

Node *insertTree(Node *raiz, int key)
{
    // Inserção normal em BST com contagem de comparações
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
        else
        {
            // chave igual -> não inserir duplicata
            return raiz;
        }
    }

    // Atualiza altura deste nó
    raiz->height = 1 + max(heightNode(raiz->left), heightNode(raiz->right));

    // Verifica fator de balanceamento
    int balance = getBalance(raiz);

    // Caso 1 - Left Left
    if (balance > 1 && key < raiz->left->key)
    {
        return rightRotate(raiz);
    }

    // Caso 2 - Right Right
    if (balance < -1 && key > raiz->right->key)
    {
        return leftRotate(raiz);
    }

    // Caso 3 - Left Right
    if (balance > 1 && key > raiz->left->key)
    {
        raiz->left = leftRotate(raiz->left);
        return rightRotate(raiz);
    }

    // Caso 4 - Right Left
    if (balance < -1 && key < raiz->right->key)
    {
        raiz->right = rightRotate(raiz->right);
        return leftRotate(raiz);
    }

    return raiz; // sem alterações de raiz
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
