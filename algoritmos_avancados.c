#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Estrutura que representa uma sala da mansão
 * Cada sala possui um nome, uma pista opcional e ponteiros para salas adjacentes
 */
typedef struct Sala
{
    char nome[50];         // Nome do cômodo
    char pista[100];       // Pista encontrada na sala (vazia se não houver)
    struct Sala *esquerda; // Ponteiro para sala à esquerda
    struct Sala *direita;  // Ponteiro para sala à direita
} Sala;

/**
 * Estrutura que representa um nó da árvore BST de pistas
 * Armazena as pistas coletadas de forma ordenada alfabeticamente
 */
typedef struct PistaNode
{
    char pista[100];            // Conteúdo da pista
    struct PistaNode *esquerda; // Subárvore esquerda (pistas menores)
    struct PistaNode *direita;  // Subárvore direita (pistas maiores)
} PistaNode;

/**
 * Função: criarSala
 * Descrição: Cria uma nova sala dinamicamente com nome e pista opcional
 * Parâmetros:
 *   - nome: string com o nome da sala
 *   - pista: string com a pista (ou "" se não houver pista)
 * Retorno: ponteiro para a sala criada
 */
Sala *criarSala(const char *nome, const char *pista)
{
    // Aloca memória dinamicamente para uma nova sala
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));

    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL)
    {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }

    // Copia o nome e a pista para a estrutura
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);

    // Inicializa os ponteiros como NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * Função: inserirPista
 * Descrição: Insere uma pista na árvore BST de forma ordenada alfabeticamente
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore BST
 *   - pista: string com a pista a ser inserida
 * Retorno: ponteiro para a raiz atualizada da árvore
 */
PistaNode *inserirPista(PistaNode *raiz, const char *pista)
{
    // Caso base: se a árvore está vazia, cria um novo nó
    if (raiz == NULL)
    {
        PistaNode *novoNode = (PistaNode *)malloc(sizeof(PistaNode));

        if (novoNode == NULL)
        {
            printf("Erro ao alocar memória para pista!\n");
            exit(1);
        }

        strcpy(novoNode->pista, pista);
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;

        return novoNode;
    }

    // Compara a pista com a raiz para decidir onde inserir
    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0)
    {
        // Pista é menor alfabeticamente, insere à esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    }
    else if (comparacao > 0)
    {
        // Pista é maior alfabeticamente, insere à direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, a pista já existe, não insere duplicata

    return raiz;
}

/**
 * Função: exibirPistas
 * Descrição: Exibe todas as pistas em ordem alfabética (percurso in-order)
 * Parâmetro: raiz - ponteiro para a raiz da árvore BST de pistas
 * Retorno: void
 */
void exibirPistas(PistaNode *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    // Percurso in-order: esquerda -> raiz -> direita
    // Isso garante ordem alfabética na BST
    exibirPistas(raiz->esquerda);
    printf("   %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/**
 * Função: contarPistas
 * Descrição: Conta o número total de pistas coletadas
 * Parâmetro: raiz - ponteiro para a raiz da árvore BST de pistas
 * Retorno: número de pistas na árvore
 */
int contarPistas(PistaNode *raiz)
{
    if (raiz == NULL)
    {
        return 0;
    }
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

/**
 * Função: explorarSalasComPistas
 * Descrição: Permite navegação interativa pela mansão com coleta de pistas
 * Parâmetros:
 *   - salaAtual: ponteiro para a sala inicial
 *   - arvorePistas: ponteiro duplo para a raiz da árvore de pistas
 * Retorno: void
 */
void explorarSalasComPistas(Sala *salaAtual, PistaNode **arvorePistas)
{
    char escolha;
    int pistasColetadas = 0;

    // Loop de exploração
    while (salaAtual != NULL)
    {
        // Exibe a sala atual
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║   LOCAL: %-28s║\n", salaAtual->nome);
        printf("╚════════════════════════════════════════╝\n");

        // Verifica se há pista nesta sala
        if (strlen(salaAtual->pista) > 0)
        {
            printf("\n PISTA ENCONTRADA!\n");
            printf("   \"%s\"\n", salaAtual->pista);

            // Adiciona a pista à árvore BST
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
            pistasColetadas++;

            printf("\n✓ Pista coletada! Total: %d\n", contarPistas(*arvorePistas));
        }
        else
        {
            printf("\n Nenhuma pista encontrada nesta sala.\n");
        }

        // Verifica se é uma sala-folha
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL)
        {
            printf("\n  Esta sala não possui mais caminhos!\n");
            printf("Pressione [S] para sair e ver as pistas coletadas.\n");
        }

        // Mostra as opções disponíveis
        printf("\n┌─────────────────────────────────────┐\n");
        printf("│  ESCOLHA SEU PRÓXIMO MOVIMENTO:    │\n");
        printf("├─────────────────────────────────────┤\n");

        if (salaAtual->esquerda != NULL)
        {
            printf("│  [E] ← Ir para a esquerda          │\n");
        }
        if (salaAtual->direita != NULL)
        {
            printf("│  [D] → Ir para a direita           │\n");
        }
        printf("│  [S] ✕ Sair e ver pistas coletadas │\n");
        printf("└─────────────────────────────────────┘\n");
        printf("\nSua escolha: ");

        // Lê a escolha do jogador
        scanf(" %c", &escolha);

        // Converte para minúscula
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
                printf("\n  Não há caminho à esquerda!\n");
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
                printf("\n  Não há caminho à direita!\n");
            }
        }
        else if (escolha == 's')
        {
            printf("\n Saindo da exploração...\n");
            break;
        }
        else
        {
            printf("\n❌ Opção inválida! Tente novamente.\n");
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

    liberarArvore(sala->esquerda);
    liberarArvore(sala->direita);
    free(sala);
}

/**
 * Função: liberarArvorePistas
 * Descrição: Libera toda a memória alocada para a árvore BST de pistas
 * Parâmetro: raiz - ponteiro para a raiz da árvore
 * Retorno: void
 */
void liberarArvorePistas(PistaNode *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

/**
 * Função: main
 * Descrição: Cria o mapa da mansão com pistas e inicia a exploração
 * Retorno: 0 em caso de sucesso
 */
int main()
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║                                               ║\n");
    printf("║      DETECTIVE QUEST - AVENTUREIRO 🔍       ║\n");
    printf("║          Coletando Pistas na Mansão          ║\n");
    printf("║                                               ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");

    // Inicializa a árvore BST de pistas (vazia no início)
    PistaNode *arvorePistas = NULL;

    // Cria o mapa da mansão com pistas associadas
    // Nível 0 (raiz)
    Sala *hall = criarSala("Hall de Entrada", "Porta principal foi arrombada");

    // Nível 1
    hall->esquerda = criarSala("Sala de Estar", "");
    hall->direita = criarSala("Cozinha", "Faca desaparecida do bloco");

    // Nível 2 - Ramificações da Sala de Estar
    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro de venenos aberto na página 13");
    hall->esquerda->direita = criarSala("Sala de Música", "");

    // Nível 2 - Ramificações da Cozinha
    hall->direita->esquerda = criarSala("Despensa", "Garrafa de vinho vazia no chão");
    hall->direita->direita = criarSala("Jardim", "Pegadas levam ao gazebo");

    // Nível 3 - Ramificações da Biblioteca
    hall->esquerda->esquerda->esquerda = criarSala("Escritório Secreto",
                                                   "Carta ameaçadora no cofre");
    hall->esquerda->esquerda->direita = criarSala("Sala de Troféus",
                                                  "Troféu de prata manchado");

    // Nível 3 - Ramificações da Sala de Música
    hall->esquerda->direita->esquerda = criarSala("Piano Room",
                                                  "Partitura rasgada");

    // Nível 3 - Ramificações do Jardim
    hall->direita->direita->esquerda = criarSala("Estufa",
                                                 "Planta venenosa recém-cortada");
    hall->direita->direita->direita = criarSala("Gazebo",
                                                "Relógio parado às 23:47");

    printf("\n Bem-vindo, Detetive!\n");
    printf(" Sua missão: explorar a mansão e coletar todas as pistas.\n");
    printf(" As pistas serão organizadas automaticamente em ordem alfabética.\n");
    printf("\nPressione ENTER para começar...");
    getchar();

    // Inicia a exploração
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe relatório final
    printf("\n\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║            RELATÓRIO FINAL                  ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");

    int totalPistas = contarPistas(arvorePistas);

    if (totalPistas > 0)
    {
        printf("\n Você coletou %d pista(s)!\n", totalPistas);
        printf("\n PISTAS EM ORDEM ALFABÉTICA:\n");
        printf("─────────────────────────────────────────────\n");
        exibirPistas(arvorePistas);
    }
    else
    {
        printf("\n Nenhuma pista foi coletada.\n");
        printf("Tente explorar mais salas na próxima vez!\n");
    }

    // Libera toda a memória alocada
    liberarArvore(hall);
    liberarArvorePistas(arvorePistas);

    printf("\n\n Obrigado por jogar Detective Quest!\n");
    printf("═══════════════════════════════════════════════\n\n");

    return 0;
}