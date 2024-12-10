# Controle de Vendas de Restaurante

## 📝 **Descrição**
Este projeto é um sistema que implementa um controle de vendas para um restaurante, utilizando estruturas de dados como listas, pilhas e filas, permitindo gerenciar produtos, pedidos e o histórico de vendas de forma eficiente.

Ele foi desenvolvido como projeto final para a disciplina Algoritmo e Estrutura de Dados 1 do curso de Ciência da Computação - UFG.

## 🌐 **Instalação**

### Linux
No terminal, siga os comandos: 
```bash
git clone 'https://github.com/emyllisoares/Projeto-AED1.git'
cd Projeto-AED1
make
```
### Windows 
Compile o programa utilizando um compilador C:
```bash
Download Zip
Abrir main.c
```

## 🌟 **Funcionalidades**

1. **Gerenciamento de Produtos**:
   - Cadastrar produtos com nome e preço.
   - Listar todos os produtos cadastrados.
   - Buscar um produto pelo ID.

2. **Gerenciamento de Pedidos**:
   - Adicionar novos pedidos à fila.
   - Preparar pedidos pendentes, transferindo-os para o histórico de vendas.

3. **Histórico de Vendas**:
   - Exibir o histórico de pedidos processados.
   - Remover um pedido do histórico (durante a operação de desfazer).

4. **Desfazer Operações**:
   - Utiliza uma pilha para desfazer o último preparo de pedido, reintroduzindo-o na fila.

5. **Persistência de Dados**:
   - Salvar produtos e histórico de vendas em arquivos binários.
   - Carregar os dados salvos ao iniciar o programa.

## ⚙️ Estrutura do Código

- **Estruturas**:
  - `Produto`: Representa os produtos cadastrados.
  - `Pedido`: Representa os pedidos realizados.
  - `FilaPedidos`: Gerencia os pedidos pendentes.
  - `Historico`: Armazena o histórico de vendas.
  - `Pilha`: Suporta a funcionalidade de desfazer operações.

- **Funções**:
  - Cadastro, busca e listagem de produtos.
  - Adicionar, preparar e listar pedidos.
  - Operações de desfazer, utilizando pilha.
  - Persistência de dados em arquivos binários.

## 📋 Exemplo de Uso

1. **Cadastrar Produtos**:
   - Insira os detalhes do produto (nome e preço).

2. **Adicionar Pedidos**:
   - Selecione um produto pelo ID e insira a quantidade desejada.

3. **Preparar Pedidos**:
   - Processa o próximo pedido na fila e o transfere para o histórico.

4. **Desfazer Pedido**:
   - Desfaz o último preparo, removendo-o do histórico e retornando-o à fila.

5. **Salvar e Sair**:
   - Salva os dados de produtos e histórico para uso posterior.

## ⚒️ Estruturas de Dados Utilizadas

### 1. **Lista**
   - **Descrição**: Estrutura encadeada utilizada para armazenar dados de forma dinâmica.
   - **Aplicações no programa**:
     - **Produtos**: 
       - A lista de produtos é encadeada pela estrutura `Produto`, com o campo `next` apontando para o próximo item.
       - Permite operações como cadastro e busca de produtos.
     - **Histórico de Vendas**:
       - O histórico de pedidos preparados é armazenado como uma lista encadeada (`Historico`).
       - Suporta a consulta de pedidos processados.

### 2. **Fila**
   - **Descrição**: Estrutura de dados que segue o princípio FIFO (First In, First Out).
   - **Aplicação no programa**:
     - Gerencia os pedidos pendentes por meio da estrutura `FilaPedidos`, que contém ponteiros `head` (início) e `tail` (fim).
     - Operações:
       - **Adicionar Pedido**: Insere um novo pedido no final da fila.
       - **Preparar Pedido**: Remove o pedido no início da fila para processá-lo.

### 3. **Pilha**
   - **Descrição**: Estrutura de dados que segue o princípio LIFO (Last In, First Out).
   - **Aplicação no programa**:
     - Implementada para suportar a funcionalidade de desfazer operações de preparo de pedidos.
     - Estrutura:
       - A pilha é gerenciada pela estrutura `Pilha`, que contém o ponteiro `top` apontando para o elemento mais recente.
     - Operações:
       - **Push**: Insere o último pedido preparado no topo da pilha.
       - **Pop**: Remove o pedido mais recente do topo para desfazer a operação, reintroduzindo-o na fila.

## ❗ Observações

- O código utiliza ponteiros para gerenciar estruturas dinâmicas.
- É importante garantir que todos os recursos alocados sejam liberados corretamente para evitar vazamentos de memória.

## Autores

1. Emylli Soares
2. Vitória Sofia

<div align="center">
  <a align="center" href="https://github.com/emyllisoares/Projeto-AED1/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=emyllisoares/Projeto-AED1" />
  </a>
</div>

