#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN "\e[32m"
#define RED "\e[31m"
#define YELLOW "\e[33m"
#define WHITE "\e[37m"
#define BLUE "\e[34m"
#define FIM_COR "\e[0m"

#define PRODUTOS_FILE "produtos.dat"
#define HISTORICO_FILE "historico.dat"

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

void liberarProdutos(Produto *produtos);
void liberarHistorico(Historico *historico);
void liberarPedidos(FilaPedidos *fila);

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
        if (scanf("%d", &opcao) != 1) {
            printf(RED"Entrada invalida! Por favor, insira um número.\n"FIM_COR);
            while (getchar() != '\n'); //Limpar buffer
            continue;
        }

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

    liberarProdutos(produtos);
    liberarPedidos(&fila);
    liberarHistorico(historico);

    return 0;
}

// Funções de Produtos
void cadastrarProduto(Produto **produtos, int *idCounter) {
    Produto *novo = (Produto *)malloc(sizeof(Produto));
    if (!novo) {
        printf(RED"Erro ao alocar memória para o produto.\n"FIM_COR);
        return;
    }

    novo->id = (*idCounter)++;
    printf(BLUE"\nDigite o nome do produto: "FIM_COR);
    scanf(" %[^\n]", novo->nome);
    printf(BLUE"Digite o preco do produto: "FIM_COR);
    if (scanf("%f", &novo->preco) != 1 || novo->preco < 0) {
        printf(RED"Preço inválido!\n"FIM_COR);
        free(novo);
        return;
    }
    novo->next = *produtos;
    *produtos = novo;
    printf(GREEN"Produto cadastrado com sucesso!\n"FIM_COR);
}

void listarProdutos(Produto *produtos) {
    if (!produtos) {
        printf(YELLOW"Nenhum produto cadastrado.\n"FIM_COR);
        return;
    }
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
    if (!novo) {
        printf(RED"Erro ao alocar memória para o pedido.\n"FIM_COR);
        return;
    }

    novo->id = (*pedidoCounter)++;
    printf(BLUE"Digite o ID do produto: "FIM_COR);
    int produtoId;
    if (scanf("%d", &produtoId) != 1) {
        printf(RED"ID inválido!\n"FIM_COR);
        free(novo);
        return;
    }

    Produto *produto = buscarProduto(produtos, produtoId);
    if (!produto) {
        printf(RED"Produto nao encontrado!\n"FIM_COR);
        free(novo);
        return;
    }
    novo->produtoId = produtoId;
    printf(BLUE"Digite a quantidade: "FIM_COR);
    if (scanf("%d", &novo->quantidade) != 1 || novo->quantidade <= 0) {
        printf(RED"Quantidade inválida!\n"FIM_COR);
        free(novo);
        return;
    }
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
    if (!novoHistorico) {
        printf(RED"Erro ao alocar memória para o histórico.\n"FIM_COR);
        fila->head = pedido; // Reverter a mudança
        return;
    }

    novoHistorico->pedido = *pedido;
    novoHistorico->next = *historico;
    *historico = novoHistorico;

    free(pedido);
    printf(GREEN"Pedido preparado e adicionado ao historico!\n"FIM_COR);
}

// Funções de Histórico
void exibirHistorico(Historico *historico) {
    if (!historico) {
        printf(YELLOW"Nenhum histórico de vendas disponível.\n"FIM_COR);
        return;
    }
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

// Funções para liberar memória
void liberarProdutos(Produto *produtos) {
    while (produtos) {
        Produto *temp = produtos;
        produtos = produtos->next;
        free(temp);
    }
}

void liberarPedidos(FilaPedidos *fila) {
    Pedido *atual = fila->head;
    while (atual) {
        Pedido *temp = atual;
        atual = atual->next;
        free(temp);
    }
    fila->head = fila->tail = NULL;
}

void liberarHistorico(Historico *historico) {
    while (historico) {
        Historico *temp = historico;
        historico = historico->next;
        free(temp);
    }
}