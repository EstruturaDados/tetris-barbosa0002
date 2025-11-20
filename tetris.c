#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da estrutura de uma peça
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[5];  // Array com capacidade para 5 peças
    int frente;     // Índice da frente da fila
    int tras;       // Índice do final da fila
    int tamanho;    // Quantidade atual de elementos
} FilaCircular;

// Estrutura da pilha
typedef struct {
    Peca pecas[3];  // Array com capacidade para 3 peças
    int topo;       // Índice do topo da pilha (-1 indica pilha vazia)
} Pilha;

// Variável global para controlar o ID das peças
int proximoId = 0;

// Função para gerar uma nova peça aleatória
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Seleciona um tipo aleatório
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

// Inicializa a fila circular com 5 peças
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    
    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        fila->pecas[fila->tras] = gerarPeca();
        fila->tras = (fila->tras + 1) % 5;
        fila->tamanho++;
    }
}

// Inicializa a pilha vazia
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;  // Pilha vazia
}

// Verifica se a fila está vazia
int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

// Verifica se a fila está cheia
int filaCheia(FilaCircular *fila) {
    return fila->tamanho == 5;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == 2;
}

// Remove e retorna a peça da frente da fila
Peca dequeue(FilaCircular *fila) {
    Peca peca = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % 5;
    fila->tamanho--;
    return peca;
}

// Adiciona uma peça no final da fila
void enqueue(FilaCircular *fila, Peca peca) {
    fila->pecas[fila->tras] = peca;
    fila->tras = (fila->tras + 1) % 5;
    fila->tamanho++;
}

// Adiciona uma peça no topo da pilha
void push(Pilha *pilha, Peca peca) {
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
}

// Remove e retorna a peça do topo da pilha
Peca pop(Pilha *pilha) {
    Peca peca = pilha->pecas[pilha->topo];
    pilha->topo--;
    return peca;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("\n========================================\n");
    printf("Estado atual:\n");
    printf("========================================\n");
    
    // Exibe a fila
    printf("Fila de pecas\t");
    if (filaVazia(fila)) {
        printf("[vazia]");
    } else {
        int idx = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[idx].nome, fila->pecas[idx].id);
            idx = (idx + 1) % 5;
        }
    }
    printf("\n");
    
    // Exibe a pilha
    printf("Pilha de reserva\t(Topo -> base): ");
    if (pilhaVazia(pilha)) {
        printf("[vazia]");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n========================================\n");
}

// Opção 1: Jogar peça da frente da fila
void jogarPeca(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("\n>>> Erro: A fila esta vazia!\n");
        return;
    }
    
    Peca removida = dequeue(fila);
    printf("\n>>> Acao: Peca [%c %d] foi jogada e removida do jogo!\n", removida.nome, removida.id);
    
    // Gera uma nova peça para manter a fila cheia
    Peca nova = gerarPeca();
    enqueue(fila, nova);
    printf(">>> Nova peca [%c %d] gerada automaticamente para manter a fila.\n", nova.nome, nova.id);
}

// Opção 2: Enviar peça da fila para a pilha de reserva
void reservarPeca(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n>>> Erro: A fila esta vazia!\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("\n>>> Erro: A pilha de reserva esta cheia! (Capacidade maxima: 3 pecas)\n");
        return;
    }
    
    Peca removida = dequeue(fila);
    push(pilha, removida);
    printf("\n>>> Acao: Peca [%c %d] enviada para a pilha de reserva!\n", 
           removida.nome, removida.id);
    
    // Gera uma nova peça para manter a fila cheia
    Peca nova = gerarPeca();
    enqueue(fila, nova);
    printf(">>> Nova peca [%c %d] gerada automaticamente para manter a fila.\n", nova.nome, nova.id);
}

// Opção 3: Usar peça da pilha de reserva
void usarPecaReservada(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("\n>>> Erro: A pilha de reserva esta vazia!\n");
        return;
    }
    
    Peca usada = pop(pilha);
    printf("\n>>> Acao: Peca reservada [%c %d] foi usada e removida do jogo!\n", usada.nome, usada.id);
}

// Opção 4: Trocar peça da frente da fila com o topo da pilha
void trocarPecaAtual(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n>>> Erro: A fila esta vazia!\n");
        return;
    }
    
    if (pilhaVazia(pilha)) {
        printf("\n>>> Erro: A pilha de reserva esta vazia!\n");
        return;
    }
    
    // Armazena informações antes da troca
    Peca pecaFila = fila->pecas[fila->frente];
    Peca pecaPilha = pilha->pecas[pilha->topo];
    
    // Troca a peça da frente da fila com o topo da pilha
    Peca temp = fila->pecas[fila->frente];
    fila->pecas[fila->frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
    
    printf("\n>>> Acao: Peca [%c %d] da fila trocada com peca [%c %d] da pilha!\n", 
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
}

// Opção 5: Trocar os 3 primeiros da fila com as 3 peças da pilha
void trocaMultipla(FilaCircular *fila, Pilha *pilha) {
    // Verifica se a fila tem pelo menos 3 peças
    if (fila->tamanho < 3) {
        printf("\nErro: A fila precisa ter pelo menos 3 pecas!\n");
        return;
    }
    
    // Verifica se a pilha tem exatamente 3 peças
    if (pilha->topo != 2) {
        printf("\nErro: A pilha precisa ter exatamente 3 pecas!\n");
        return;
    }
    
    // Array temporário para armazenar as 3 primeiras peças da fila
    Peca temp[3];
    int idx = fila->frente;
    
    // Copia as 3 primeiras peças da fila
    for (int i = 0; i < 3; i++) {
        temp[i] = fila->pecas[idx];
        idx = (idx + 1) % 5;
    }
    
    // Substitui as 3 primeiras peças da fila pelas da pilha
    idx = fila->frente;
    for (int i = 2; i >= 0; i--) {
        fila->pecas[idx] = pilha->pecas[i];
        idx = (idx + 1) % 5;
    }
    
    // Substitui as peças da pilha pelas da fila
    for (int i = 0; i < 3; i++) {
        pilha->pecas[2 - i] = temp[i];
    }
    
    printf("\nAcao: Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\n========================================\n");
    printf("Opcoes disponiveis:\n");
    printf("========================================\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
    printf("========================================\n");
    printf("Opcao escolhida: ");
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Declara e inicializa as estruturas
    FilaCircular fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    int opcao;
    
    printf("========================================\n");
    printf("  GERENCIADOR DE PECAS\n");
    printf("  Fila Circular + Pilha de Reserva\n");
    printf("========================================\n");
    
    // Loop principal do programa
    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                printf("Obrigado por usar o Gerenciador de Pecas!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}