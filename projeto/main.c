#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN "\e[32m"
#define RED "\e[31m"
#define YELLOW "\e[33m"
#define WHITE "\e[37m"
#define BLUE "\e[34m"
#define FIM_COR "\e[0m"

// Estruturas de Dados
typedef struct Produto {
    int id;
    char nome[50];
    float preco;
    struct Produto *next;
} Produto;

typedef struct Pedido {
    int id;
    int produtoId;
    int quantidade;
    float total;
    struct Pedido *next;
} Pedido;

typedef struct {
    Pedido *head, *tail;
} FilaPedidos;

typedef struct Historico {
    Pedido pedido;
    struct Historico *next;
} Historico;

// Protótipos de Funções
void cadastrarProduto(Produto **produtos, int *idCounter);
void listarProdutos(Produto *produtos);
Produto* buscarProduto(Produto *produtos, int id);

void adicionarPedido(FilaPedidos *fila, Produto *produtos, int *pedidoCounter);
void prepararPedido(FilaPedidos *fila, Historico **historico);
void listarPedidos(FilaPedidos *fila);

void exibirHistorico(Historico *historico);

void salvarDados(Produto *produtos, Historico *historico);
void carregarDados(Produto **produtos, Historico **historico);

// Função Principal
int main() {
    Produto *produtos = NULL;
    FilaPedidos fila = {NULL, NULL};
    Historico *historico = NULL;
    int produtoIdCounter = 1, pedidoIdCounter = 1;
    int opcao;

    carregarDados(&produtos, &historico);

    do {
        printf(BLUE"\t\t========= CONTROLE DE VENDAS =========\n\n"FIM_COR);
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Adicionar Pedido\n");
        printf("4. Preparar Pedido\n");
        printf("5. Exibir Historico de Vendas\n");
        printf("6. Salvar e Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarProduto(&produtos, &produtoIdCounter); break;
            case 2: listarProdutos(produtos); break;
            case 3: adicionarPedido(&fila, produtos, &pedidoIdCounter); break;
            case 4: prepararPedido(&fila, &historico); break;
            case 5: exibirHistorico(historico); break;
            case 6: salvarDados(produtos, historico); break;
            default: printf(RED"Opcao invalida!\n"FIM_COR);
        }
    } while (opcao != 6);

    return 0;
}

// Funções de Produtos
void cadastrarProduto(Produto **produtos, int *idCounter) {
    Produto *novo = (Produto *)malloc(sizeof(Produto));
    novo->id = (*idCounter)++;
    printf(BLUE"\nDigite o nome do produto: "FIM_COR);
    scanf(" %[^\n]", novo->nome);
    printf(BLUE"Digite o preco do produto: "FIM_COR);
    scanf("%f", &novo->preco);
    novo->next = *produtos;
    *produtos = novo;
    printf(GREEN"Produto cadastrado com sucesso!\n"FIM_COR);
}

void listarProdutos(Produto *produtos) {
    printf("\n=== Lista de Produtos ===\n");
    while (produtos) {
        printf("ID: %d | Nome: %s | Preco: R$ %.2f\n", produtos->id, produtos->nome, produtos->preco);
        produtos = produtos->next;
    }
}

Produto* buscarProduto(Produto *produtos, int id) {
    while (produtos) {
        if (produtos->id == id) return produtos;
        produtos = produtos->next;
    }
    return NULL;
}

// Funções de Pedidos
void adicionarPedido(FilaPedidos *fila, Produto *produtos, int *pedidoCounter) {
    Pedido *novo = (Pedido *)malloc(sizeof(Pedido));
    novo->id = (*pedidoCounter)++;
    printf(BLUE"Digite o ID do produto: "FIM_COR);
    int produtoId;
    scanf("%d", &produtoId);
    Produto *produto = buscarProduto(produtos, produtoId);
    if (!produto) {
        printf(RED"Produto nao encontrado!\n"FIM_COR);
        free(novo);
        return;
    }
    novo->produtoId = produtoId;
    printf(BLUE"Digite a quantidade: "FIM_COR);
    scanf("%d", &novo->quantidade);
    novo->total = produto->preco * novo->quantidade;
    novo->next = NULL;

    if (!fila->head) fila->head = fila->tail = novo;
    else {
        fila->tail->next = novo;
        fila->tail = novo;
    }

    printf(GREEN"Pedido adicionado com sucesso!\n"FIM_COR);
}

void listarPedidos(FilaPedidos *fila) {
    printf("\n=== Pedidos na Fila ===\n");
    Pedido *current = fila->head;
    while (current) {
        printf("ID Pedido: %d | Produto ID: %d | Quantidade: %d | Total: R$ %.2f\n",
               current->id, current->produtoId, current->quantidade, current->total);
        current = current->next;
    }
}

void prepararPedido(FilaPedidos *fila, Historico **historico) {
    if (!fila->head) {
        printf(RED"Nenhum pedido na fila!\n"FIM_COR);
        return;
    }

    Pedido *pedido = fila->head;
    fila->head = fila->head->next;
    if (!fila->head) fila->tail = NULL;

    Historico *novoHistorico = (Historico *)malloc(sizeof(Historico));
    novoHistorico->pedido = *pedido;
    novoHistorico->next = *historico;
    *historico = novoHistorico;

    free(pedido);
    printf("Pedido preparado e adicionado ao historico!\n");
}

// Funções de Histórico
void exibirHistorico(Historico *historico) {
    printf("\n=== Historico de Vendas ===\n");
    while (historico) {
        printf("ID Pedido: %d | Produto ID: %d | Quantidade: %d | Total: R$ %.2f\n",
               historico->pedido.id, historico->pedido.produtoId,
               historico->pedido.quantidade, historico->pedido.total);
        historico = historico->next;
    }
}

// Funções de Persistência
void salvarDados(Produto *produtos, Historico *historico) {
    FILE *file = fopen("produtos.dat", "wb");
    while (produtos) {
        fwrite(produtos, sizeof(Produto), 1, file);
        produtos = produtos->next;
    }
    fclose(file);

    file = fopen("historico.dat", "wb");
    while (historico) {
        fwrite(&(historico->pedido), sizeof(Pedido), 1, file);
        historico = historico->next;
    }
    fclose(file);

    printf(GREEN"Dados salvos com sucesso!\n"FIM_COR);
}

void carregarDados(Produto **produtos, Historico **historico) {
    FILE *file = fopen("produtos.dat", "rb");
    if (file) {
        Produto temp;
        while (fread(&temp, sizeof(Produto), 1, file)) {
            Produto *novo = (Produto *)malloc(sizeof(Produto));
            *novo = temp;
            novo->next = *produtos;
            *produtos = novo;
        }
        fclose(file);
    }

    file = fopen("historico.dat", "rb");
    if (file) {
        Pedido temp;
        while (fread(&temp, sizeof(Pedido), 1, file)) {
            Historico *novo = (Historico *)malloc(sizeof(Historico));
            novo->pedido = temp;
            novo->next = *historico;
            *historico = novo;
        }
        fclose(file);
    }
}
