# Controle de Vendas de Restaurante

Projeto desenvolvido para a disciplina Algoritmo e Estrutura de Dados 1. Este sistema implementa um controle de vendas para um restaurante, utilizando estruturas de dados como listas, pilhas e filas.

O programa permite gerenciar produtos, pedidos e o histórico de vendas de forma eficiente.

## 📋 Como Usar
### Menu Principal
### 1. Cadastrar Produto
Insira o nome e o preço do produto.
O sistema gera automaticamente um ID único para o produto.

### 2. Listar Produtos
Exibe todos os produtos cadastrados, incluindo detalhes como nome, código e preço.

### 3. Adicionar Pedido

Insiro o ID do produto e a quantidade desejada.
O pedido é adicionado à fila de pedidos.

### 4. Preparar Pedido
Remove o próximo pedido da fila e marca-o como concluído. Os pedidos concluídos são registrados no histórico de vendas.

### 5. Exibir Histórico de Vendas
Mostra os pedidos já concluídos, incluindo detalhes como produtos vendidos, quantidades e valores totais.

### 6. Desfazer Último Preparo
Remove o último pedido do histórico e o retorna à fila de pedidos pendentes.

### 7. Salvar e Sair
Salva todas as informações em arquivos para que possam ser recuperadas posteriormente e encerra o programa.

## 🛠️ Estruturas de Dados Utilizadas
Lista: Utilizada para gerenciar o cadastro de produtos.

Fila: Implementada para organizar os pedidos em ordem de chegada.

Pilha: Usada para exibir o histórico de vendas no formato de "último a sair".


