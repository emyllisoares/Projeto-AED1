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

// estruturas de Dados
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

typedef struct PilhaHistorico {
    Pedido pedido;
    struct PilhaHistorico *next;
} PilhaHistorico;

typedef struct {
    PilhaHistorico *top;
} Pilha;

// protótipos de Funções
void push(Pilha *pilha, Pedido pedido);
Pedido* pop(Pilha *pilha);
void desfazerPedido(Pilha *pilha, FilaPedidos *fila, Historico **historico);
void removerDoHistorico(Historico **historico, int pedidoId);

void cadastrarProduto(Produto **produtos, int *idCounter);
void listarProdutos(Produto *produtos);
Produto* buscarProduto(Produto *produtos, int id);

void adicionarPedido(FilaPedidos *fila, Produto *produtos, int *pedidoCounter);
void prepararPedido(FilaPedidos *fila, Historico **historico, Pilha *pilha);
void listarPedidos(FilaPedidos *fila);

void exibirHistorico(Historico *historico);

void salvarDados(Produto *produtos, Historico *historico);
void carregarDados(Produto **produtos, Historico **historico, int *produtoIdCounter, int *pedidoIdCounter);

void liberarProdutos(Produto *produtos);
void liberarHistorico(Historico *historico);
void liberarPedidos(FilaPedidos *fila);
void liberarPilha(Pilha *pilha);

// função Principal
int main() {
    Produto *produtos = NULL;
    FilaPedidos fila = {NULL, NULL};
    Historico *historico = NULL;
    Pilha pilha = {NULL}; // Pilha para desfazer pedidos
    int produtoIdCounter = 1, pedidoIdCounter = 1;
    int opcao;

    carregarDados(&produtos, &historico, &produtoIdCounter, &pedidoIdCounter);

    do {
        printf(BLUE"\t\t========= CONTROLE DE VENDAS =========\n\n"FIM_COR);
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Adicionar Pedido\n");
        printf("4. Preparar Pedido\n");
        printf("5. Exibir Historico de Vendas\n");
        printf("6. Desfazer ultimo praparo\n");
        printf("7. Salvar e Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf(RED"Entrada inválida! Por favor, insira um número.\n"FIM_COR);
            while (getchar() != '\n'); // Limpar buffer
            continue;
        }

        switch (opcao) {
            case 1: cadastrarProduto(&produtos, &produtoIdCounter); break;
            case 2: listarProdutos(produtos); break;
            case 3: adicionarPedido(&fila, produtos, &pedidoIdCounter); break;
            case 4: prepararPedido(&fila, &historico, &pilha); break;
            case 5: exibirHistorico(historico); break;
            case 6: desfazerPedido(&pilha, &fila, &historico); break; //Nova função
            case 7: salvarDados(produtos, historico); break;
            default: printf(RED"Opcao invalida!\n"FIM_COR);
        }
    } while (opcao != 7);

    liberarProdutos(produtos);
    liberarPedidos(&fila);
    liberarHistorico(historico);
    liberarPilha(&pilha);

    return 0;
}

// funções de Produtos
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
        printf("ID: %d | Preco: R$ %.2f | Nome: %s \n", produtos->id, produtos->preco, produtos->nome);
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

// funções de Pedidos
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

void prepararPedido(FilaPedidos *fila, Historico **historico, Pilha *pilha) {
    if (!fila->head) {
        printf(RED"Nenhum pedido na fila!\n"FIM_COR);
        return;
    }

    Pedido *pedido = fila->head;
    fila->head = fila->head->next;
    if (!fila->head) fila->tail = NULL;

    Historico *novoHistorico = (Historico *)malloc(sizeof(Historico));
    if (!novoHistorico) {
        printf(RED"Erro ao alocar memoria para o historico.\n"FIM_COR);
        fila->head = pedido; // reverter a mudança
        return;
    }

    novoHistorico->pedido = *pedido;
    novoHistorico->next = *historico;
    *historico = novoHistorico;

    push(pilha, *pedido);  // adiciona à pilha para possível desfazer

    free(pedido);
    printf(GREEN"Pedido preparado e adicionado ao historico!\n"FIM_COR);
}

//implementação da Pilha 
void push(Pilha *pilha, Pedido pedido) {
    PilhaHistorico *novo = (PilhaHistorico *)malloc(sizeof(PilhaHistorico));
    if (!novo) {
        printf(RED"Erro ao alocar memoria para a pilha.\n"FIM_COR);
        return;
    }

    novo->pedido = pedido;
    novo->next = pilha->top;
    pilha->top = novo;
}

Pedido* pop(Pilha *pilha) {
    if (!pilha->top) {
        printf(RED"Nenhum pedido para desfazer!\n"FIM_COR);
        return NULL;
    }

    PilhaHistorico *topo = pilha->top;
    Pedido *pedido = (Pedido *)malloc(sizeof(Pedido));
    if(!pedido) {
        printf(RED"Erro ao alocar memoria para o pedido.\n"FIM_COR);
        return NULL;
    }
    *pedido = topo->pedido;

    pilha ->top = topo->next;
    free(topo);

    return pedido;
}

//desfazer útimo pedido
void desfazerPedido(Pilha *pilha, FilaPedidos *fila, Historico **historico) {
    Pedido *pedido = pop(pilha);
    if (!pedido) return;

    // remover do histórico
    removerDoHistorico(historico, pedido->id);

    // adiciona o pedido de volta na fila
    pedido->next = NULL;
    if (!fila->head) {
        fila->head = fila->tail = pedido;
    } else {
        fila->tail->next = pedido;
        fila->tail = pedido;
    }

    printf(GREEN"Ultimo pedido desfeito! Pedido ID %d retornado a fila.\n"FIM_COR, pedido->id);
}


void exibirHistorico(Historico *historico) {
    if (!historico) {
        printf(YELLOW"Nenhum histórico de vendas disponivel.\n"FIM_COR);
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

void removerDoHistorico(Historico **historico, int pedidoId) {
    Historico *atual = *historico;
    Historico *anterior = NULL;

    while (atual) {
        if (atual->pedido.id == pedidoId) {
            // remover o nó
            if (anterior) {
                anterior->next = atual->next;
            } else {
                *historico = atual->next;
            }
            free(atual);
            printf(GREEN"Pedido ID %d removido do historico com sucesso.\n"FIM_COR, pedidoId);
            return;
        }
        anterior = atual;
        atual = atual->next;
    }

    printf(YELLOW"Pedido ID %d nao encontrado no histórico.\n"FIM_COR, pedidoId);
}



// funções de Persistência
void salvarDados(Produto *produtos, Historico *historico) {
    FILE *file = fopen(PRODUTOS_FILE, "wb");
    if (!file) {
        printf(RED"Erro ao salvar os produtos.\n"FIM_COR);
        return;
    }
    while (produtos) {
        fwrite(produtos, sizeof(Produto), 1, file);
        produtos = produtos->next;
    }
    fclose(file);

    file = fopen(HISTORICO_FILE, "wb");
    if (!file) {
        printf(RED"Erro ao salvar o histórico.\n"FIM_COR);
        return;
    }
    while (historico) {
        fwrite(&(historico->pedido), sizeof(Pedido), 1, file);
        historico = historico->next;
    }
    fclose(file);

    printf(GREEN"Dados salvos com sucesso!\n"FIM_COR);
}

void carregarDados(Produto **produtos, Historico **historico, int *produtoIdCounter, int *pedidoIdCounter) {
    FILE *file = fopen(PRODUTOS_FILE, "rb");
    if (file) {
        Produto temp;
        int maiorProdutoId = 0;
        while (fread(&temp, sizeof(Produto), 1, file)) {
            Produto *novo = (Produto *)malloc(sizeof(Produto));
            if (!novo) break;
            *novo = temp;
            novo->next = *produtos;
            *produtos = novo;

            if(temp.id > maiorProdutoId) {
                maiorProdutoId = temp.id;
            }
        }
        fclose(file);

        *produtoIdCounter = maiorProdutoId + 1;
    }

    int maiorPedidoId = 0;
    file = fopen(HISTORICO_FILE, "rb");
    if (file) {
        Pedido temp;
        while (fread(&temp, sizeof(Pedido), 1, file)) {
            Historico *novo = (Historico *)malloc(sizeof(Historico));
            if (!novo) break;
            novo->pedido = temp;
            novo->next = *historico;
            *historico = novo;

            if(temp.id > maiorPedidoId) {
                maiorPedidoId = temp.id;
            }
        }
        fclose(file);

        *pedidoIdCounter = maiorPedidoId + 1;
    }
}

// funções para liberar memória
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

void liberarPilha(Pilha *pilha) {
    while (pilha->top) {
        PilhaHistorico *temp = pilha->top;
        pilha->top = pilha->top->next;
        free(temp);
    }
}
