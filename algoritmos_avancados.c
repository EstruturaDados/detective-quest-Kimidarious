#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Estrutura que representa uma sala da mansão
 * Cada sala possui um nome e ponteiros para as salas à esquerda e direita
 */
typedef struct Sala
{
    char nome[50];         // Nome do cômodo
    struct Sala *esquerda; // Ponteiro para sala à esquerda
    struct Sala *direita;  // Ponteiro para sala à direita
} Sala;

/**
 * Função: criarSala
 * Descrição: Cria uma nova sala dinamicamente e inicializa seus campos
 * Parâmetro: nome - string com o nome da sala
 * Retorno: ponteiro para a sala criada
 */
Sala *criarSala(const char *nome)
{
    // Aloca memória dinamicamente para uma nova sala
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));

    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL)
    {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    // Copia o nome para a estrutura
    strcpy(novaSala->nome, nome);

    // Inicializa os ponteiros como NULL (sem caminhos inicialmente)
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * Função: explorarSalas
 * Descrição: Permite ao jogador navegar interativamente pela mansão
 * Parâmetro: salaAtual - ponteiro para a sala onde o jogador está
 * Retorno: void
 */
void explorarSalas(Sala *salaAtual)
{
    char escolha;

    // Loop de exploração - continua até chegar em uma sala sem saídas
    while (salaAtual != NULL)
    {
        // Exibe a sala atual
        printf("\n========================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("========================================\n");

        // Verifica se é uma sala-folha (sem mais caminhos)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL)
        {
            printf("\nEsta sala não possui mais caminhos!\n");
            printf("Fim da exploração.\n");
            break;
        }

        // Mostra as opções disponíveis
        printf("\nEscolha seu caminho:\n");
        if (salaAtual->esquerda != NULL)
        {
            printf("  [E] - Ir para a esquerda\n");
        }
        if (salaAtual->direita != NULL)
        {
            printf("  [D] - Ir para a direita\n");
        }
        printf("  [S] - Sair da exploração\n");
        printf("\nSua escolha: ");

        // Lê a escolha do jogador
        scanf(" %c", &escolha);

        // Converte para minúscula para facilitar comparação
        if (escolha >= 'A' && escolha <= 'Z')
        {
            escolha = escolha + 32;
        }

        // Processa a escolha
        if (escolha == 'e')
        {
            if (salaAtual->esquerda != NULL)
            {
                salaAtual = salaAtual->esquerda;
            }
            else
            {
                printf("\nNão há caminho à esquerda!\n");
            }
        }
        else if (escolha == 'd')
        {
            if (salaAtual->direita != NULL)
            {
                salaAtual = salaAtual->direita;
            }
            else
            {
                printf("\nNão há caminho à direita!\n");
            }
        }
        else if (escolha == 's')
        {
            printf("\nVocê decidiu sair da exploração.\n");
            break;
        }
        else
        {
            printf("\nOpção inválida! Tente novamente.\n");
        }
    }
}

/**
 * Função: liberarArvore
 * Descrição: Libera toda a memória alocada para a árvore de salas
 * Parâmetro: sala - ponteiro para a raiz da árvore
 * Retorno: void
 */
void liberarArvore(Sala *sala)
{
    if (sala == NULL)
    {
        return;
    }

    // Libera recursivamente a subárvore esquerda
    liberarArvore(sala->esquerda);

    // Libera recursivamente a subárvore direita
    liberarArvore(sala->direita);

    // Libera a sala atual
    free(sala);
}

/**
 * Função: main
 * Descrição: Cria o mapa inicial da mansão e inicia a exploração
 * Retorno: 0 em caso de sucesso
 */
int main()
{
    printf("╔════════════════════════════════════════╗\n");
    printf("║     DETECTIVE QUEST - MANSION MAP     ║\n");
    printf("╚════════════════════════════════════════╝\n");

    // Cria o mapa da mansão (árvore binária)
    // Nível 0 (raiz) - Hall de entrada
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Cozinha");

    // Nível 2 - Ramificações da Sala de Estar
    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Sala de Música");

    // Nível 2 - Ramificações da Cozinha
    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita = criarSala("Jardim");

    // Nível 3 - Ramificações da Biblioteca
    hall->esquerda->esquerda->esquerda = criarSala("Escritório Secreto");
    hall->esquerda->esquerda->direita = criarSala("Sala de Troféus");

    // Nível 3 - Ramificações do Jardim
    hall->direita->direita->esquerda = criarSala("Estufa");
    hall->direita->direita->direita = criarSala("Gazebo");

    printf("\nBem-vindo ao Detective Quest!\n");
    printf("Explore a mansão em busca de pistas...\n");

    // Inicia a exploração a partir do Hall
    explorarSalas(hall);

    // Libera toda a memória alocada
    liberarArvore(hall);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}