#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 20 // Tamanho da tabela hash

/**
 * Estrutura que representa uma sala da mansão
 * Cada sala possui um nome e ponteiros para salas adjacentes
 */
typedef struct Sala
{
    char nome[50];         // Nome do cômodo
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
    struct PistaNode *esquerda; // Subárvore esquerda
    struct PistaNode *direita;  // Subárvore direita
} PistaNode;

/**
 * Estrutura que representa um nó na tabela hash
 * Associa uma pista a um suspeito (encadeamento para tratar colisões)
 */
typedef struct HashNode
{
    char pista[100];          // Chave: pista
    char suspeito[50];        // Valor: nome do suspeito
    struct HashNode *proximo; // Próximo nó (tratamento de colisões)
} HashNode;

/**
 * Estrutura da tabela hash
 * Array de ponteiros para listas encadeadas
 */
typedef struct TabelaHash
{
    HashNode *tabela[TAM_HASH];
} TabelaHash;

/**
 * Função: funcaoHash
 * Descrição: Calcula o índice hash para uma string
 * Parâmetro: chave - string para calcular o hash
 * Retorno: índice na tabela hash (0 a TAM_HASH-1)
 */
int funcaoHash(const char *chave)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *chave++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % TAM_HASH;
}

/**
 * Função: inicializarHash
 * Descrição: Inicializa a tabela hash com NULL
 * Parâmetro: tabela - ponteiro para a tabela hash
 * Retorno: void
 */
void inicializarHash(TabelaHash *tabela)
{
    for (int i = 0; i < TAM_HASH; i++)
    {
        tabela->tabela[i] = NULL;
    }
}

/**
 * Função: inserirNaHash
 * Descrição: Insere uma associação pista-suspeito na tabela hash
 * Parâmetros:
 *   - tabela: ponteiro para a tabela hash
 *   - pista: chave (pista encontrada)
 *   - suspeito: valor (nome do suspeito)
 * Retorno: void
 */
void inserirNaHash(TabelaHash *tabela, const char *pista, const char *suspeito)
{
    int indice = funcaoHash(pista);

    // Cria novo nó
    HashNode *novoNode = (HashNode *)malloc(sizeof(HashNode));
    if (novoNode == NULL)
    {
        printf("Erro ao alocar memória para hash!\n");
        exit(1);
    }

    strcpy(novoNode->pista, pista);
    strcpy(novoNode->suspeito, suspeito);
    novoNode->proximo = NULL;

    // Insere no início da lista encadeada (tratamento de colisão)
    if (tabela->tabela[indice] == NULL)
    {
        tabela->tabela[indice] = novoNode;
    }
    else
    {
        novoNode->proximo = tabela->tabela[indice];
        tabela->tabela[indice] = novoNode;
    }
}

/**
 * Função: encontrarSuspeito
 * Descrição: Busca o suspeito associado a uma pista na tabela hash
 * Parâmetros:
 *   - tabela: ponteiro para a tabela hash
 *   - pista: chave de busca
 * Retorno: ponteiro para o nome do suspeito (ou NULL se não encontrado)
 */
char *encontrarSuspeito(TabelaHash *tabela, const char *pista)
{
    int indice = funcaoHash(pista);
    HashNode *atual = tabela->tabela[indice];

    // Percorre a lista encadeada no índice
    while (atual != NULL)
    {
        if (strcmp(atual->pista, pista) == 0)
        {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }

    return NULL;
}

/**
 * Função: criarSala
 * Descrição: Cria uma nova sala dinamicamente
 * Parâmetro: nome - string com o nome da sala
 * Retorno: ponteiro para a sala criada
 */
Sala *criarSala(const char *nome)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));

    if (novaSala == NULL)
    {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * Função: obterPistaDaSala
 * Descrição: Retorna a pista associada a uma sala específica (regra fixa)
 * Parâmetro: nomeSala - nome da sala
 * Retorno: ponteiro para string com a pista (ou NULL se não houver)
 */
const char *obterPistaDaSala(const char *nomeSala)
{
    // Mapeamento fixo: sala -> pista
    if (strcmp(nomeSala, "Hall de Entrada") == 0)
    {
        return "Porta principal foi arrombada";
    }
    else if (strcmp(nomeSala, "Cozinha") == 0)
    {
        return "Faca desaparecida do bloco";
    }
    else if (strcmp(nomeSala, "Biblioteca") == 0)
    {
        return "Livro de venenos aberto na página 13";
    }
    else if (strcmp(nomeSala, "Despensa") == 0)
    {
        return "Garrafa de vinho vazia no chão";
    }
    else if (strcmp(nomeSala, "Jardim") == 0)
    {
        return "Pegadas levam ao gazebo";
    }
    else if (strcmp(nomeSala, "Escritório Secreto") == 0)
    {
        return "Carta ameaçadora no cofre";
    }
    else if (strcmp(nomeSala, "Sala de Troféus") == 0)
    {
        return "Troféu de prata manchado";
    }
    else if (strcmp(nomeSala, "Piano Room") == 0)
    {
        return "Partitura rasgada";
    }
    else if (strcmp(nomeSala, "Estufa") == 0)
    {
        return "Planta venenosa recém-cortada";
    }
    else if (strcmp(nomeSala, "Gazebo") == 0)
    {
        return "Relógio parado às 23:47";
    }

    return NULL; // Sala sem pista
}

/**
 * Função: inserirPista
 * Descrição: Insere uma pista na árvore BST de forma ordenada
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore BST
 *   - pista: string com a pista a ser inserida
 * Retorno: ponteiro para a raiz atualizada da árvore
 */
PistaNode *inserirPista(PistaNode *raiz, const char *pista)
{
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

    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0)
    {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    }
    else if (comparacao > 0)
    {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

/**
 * Função: exibirPistas
 * Descrição: Exibe todas as pistas em ordem alfabética
 * Parâmetro: raiz - ponteiro para a raiz da árvore BST de pistas
 * Retorno: void
 */
void exibirPistas(PistaNode *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    exibirPistas(raiz->esquerda);
    printf("   %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/**
 * Função: contarPistasPorSuspeito
 * Descrição: Conta quantas pistas apontam para um suspeito específico
 * Parâmetros:
 *   - raiz: raiz da árvore BST de pistas
 *   - tabela: tabela hash pista-suspeito
 *   - nomeSuspeito: nome do suspeito a procurar
 * Retorno: número de pistas que apontam para o suspeito
 */
int contarPistasPorSuspeito(PistaNode *raiz, TabelaHash *tabela, const char *nomeSuspeito)
{
    if (raiz == NULL)
    {
        return 0;
    }

    int count = 0;
    char *suspeito = encontrarSuspeito(tabela, raiz->pista);

    if (suspeito != NULL && strcmp(suspeito, nomeSuspeito) == 0)
    {
        count = 1;
    }

    return count + contarPistasPorSuspeito(raiz->esquerda, tabela, nomeSuspeito) +
           contarPistasPorSuspeito(raiz->direita, tabela, nomeSuspeito);
}

/**
 * Função: listarPistasDeSuspeito
 * Descrição: Lista todas as pistas que apontam para um suspeito
 * Parâmetros:
 *   - raiz: raiz da árvore BST de pistas
 *   - tabela: tabela hash pista-suspeito
 *   - nomeSuspeito: nome do suspeito
 * Retorno: void
 */
void listarPistasDeSuspeito(PistaNode *raiz, TabelaHash *tabela, const char *nomeSuspeito)
{
    if (raiz == NULL)
    {
        return;
    }

    listarPistasDeSuspeito(raiz->esquerda, tabela, nomeSuspeito);

    char *suspeito = encontrarSuspeito(tabela, raiz->pista);
    if (suspeito != NULL && strcmp(suspeito, nomeSuspeito) == 0)
    {
        printf("     • %s\n", raiz->pista);
    }

    listarPistasDeSuspeito(raiz->direita, tabela, nomeSuspeito);
}

/**
 * Função: explorarSalas
 * Descrição: Permite navegação interativa pela mansão com coleta de pistas
 * Parâmetros:
 *   - salaAtual: ponteiro para a sala inicial
 *   - arvorePistas: ponteiro duplo para a raiz da árvore de pistas
 *   - tabela: ponteiro para a tabela hash
 * Retorno: void
 */
void explorarSalas(Sala *salaAtual, PistaNode **arvorePistas, TabelaHash *tabela)
{
    char escolha;

    while (salaAtual != NULL)
    {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║   LOCAL: %-28s║\n", salaAtual->nome);
        printf("╚════════════════════════════════════════╝\n");

        // Obtém a pista desta sala
        const char *pista = obterPistaDaSala(salaAtual->nome);

        if (pista != NULL)
        {
            printf("\n PISTA ENCONTRADA!\n");
            printf("   \"%s\"\n", pista);

            // Adiciona à árvore BST
            *arvorePistas = inserirPista(*arvorePistas, pista);

            // Mostra o suspeito associado
            char *suspeito = encontrarSuspeito(tabela, pista);
            if (suspeito != NULL)
            {
                printf("    Suspeito relacionado: %s\n", suspeito);
            }

            printf("\n✓ Pista coletada!\n");
        }
        else
        {
            printf("\n Nenhuma pista encontrada nesta sala.\n");
        }

        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL)
        {
            printf("\n  Esta sala não possui mais caminhos!\n");
        }

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
        printf("│  [S] ✕ Finalizar e acusar culpado  │\n");
        printf("└─────────────────────────────────────┘\n");
        printf("\nSua escolha: ");

        scanf(" %c", &escolha);

        if (escolha >= 'A' && escolha <= 'Z')
        {
            escolha = escolha + 32;
        }

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
            printf("\n Finalizando exploração...\n");
            break;
        }
        else
        {
            printf("\n Opção inválida!\n");
        }
    }
}

/**
 * Função: verificarSuspeitoFinal
 * Descrição: Conduz a fase de acusação e verifica se há evidências suficientes
 * Parâmetros:
 *   - arvorePistas: raiz da árvore de pistas coletadas
 *   - tabela: tabela hash pista-suspeito
 * Retorno: void
 */
void verificarSuspeitoFinal(PistaNode *arvorePistas, TabelaHash *tabela)
{
    char acusado[50];

    printf("\n\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║            FASE DE ACUSAÇÃO ⚖️              ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");

    printf("\n PISTAS COLETADAS:\n");
    printf("─────────────────────────────────────────────\n");
    exibirPistas(arvorePistas);

    printf("\n\n SUSPEITOS DISPONÍVEIS:\n");
    printf("─────────────────────────────────────────────\n");
    printf("  • Lady Elizabeth\n");
    printf("  • Professor Harrington\n");
    printf("  • Chef Marcel\n");
    printf("  • Mordomo James\n");

    printf("\n  Quem você acusa do crime?\n");
    printf("Digite o nome completo: ");
    getchar(); // Limpa buffer
    fgets(acusado, sizeof(acusado), stdin);
    acusado[strcspn(acusado, "\n")] = 0; // Remove newline

    // Conta pistas que apontam para o acusado
    int numPistas = contarPistasPorSuspeito(arvorePistas, tabela, acusado);

    printf("\n\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║               ANÁLISE DE EVIDÊNCIAS         ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");

    printf("\n Acusado: %s\n", acusado);
    printf(" Pistas encontradas: %d\n", numPistas);

    if (numPistas > 0)
    {
        printf("\n EVIDÊNCIAS CONTRA %s:\n", acusado);
        printf("─────────────────────────────────────────────\n");
        listarPistasDeSuspeito(arvorePistas, tabela, acusado);
    }

    printf("\n");
    printf("═══════════════════════════════════════════════\n");

    if (numPistas >= 2)
    {
        printf("║   CASO RESOLVIDO!                          ║\n");
        printf("═══════════════════════════════════════════════\n");
        printf("\n Parabéns, Detetive!\n");
        printf("\nVocê reuniu evidências suficientes (%d pistas)\n", numPistas);
        printf("para provar que %s é o culpado!\n", acusado);
        printf("\nO júri aceita sua acusação. Caso encerrado! ⚖️\n");
    }
    else
    {
        printf("║   EVIDÊNCIAS INSUFICIENTES                 ║\n");
        printf("═══════════════════════════════════════════════\n");
        printf("\n Infelizmente...\n");
        printf("\nVocê só encontrou %d pista(s) contra %s.\n", numPistas, acusado);
        printf("São necessárias pelo menos 2 pistas para uma acusação válida.\n");
        printf("\nO suspeito foi liberado por falta de provas. 🚪\n");
        printf("Tente novamente e explore mais a mansão!\n");
    }
}

/**
 * Função: liberarArvore
 * Descrição: Libera memória da árvore de salas
 */
void liberarArvore(Sala *sala)
{
    if (sala == NULL)
        return;
    liberarArvore(sala->esquerda);
    liberarArvore(sala->direita);
    free(sala);
}

/**
 * Função: liberarArvorePistas
 * Descrição: Libera memória da árvore BST de pistas
 */
void liberarArvorePistas(PistaNode *raiz)
{
    if (raiz == NULL)
        return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

/**
 * Função: liberarHash
 * Descrição: Libera memória da tabela hash
 */
void liberarHash(TabelaHash *tabela)
{
    for (int i = 0; i < TAM_HASH; i++)
    {
        HashNode *atual = tabela->tabela[i];
        while (atual != NULL)
        {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

/**
 * Função: main
 * Descrição: Inicializa o jogo, cria estruturas e gerencia o fluxo principal
 */
int main()
{
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║                                               ║\n");
    printf("║        DETECTIVE QUEST - MESTRE           ║\n");
    printf("║        Resolva o Mistério da Mansão          ║\n");
    printf("║                                               ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");

    // Inicializa estruturas
    PistaNode *arvorePistas = NULL;
    TabelaHash tabelaHash;
    inicializarHash(&tabelaHash);

    // Popula a tabela hash (pista -> suspeito)
    inserirNaHash(&tabelaHash, "Porta principal foi arrombada", "Mordomo James");
    inserirNaHash(&tabelaHash, "Faca desaparecida do bloco", "Chef Marcel");
    inserirNaHash(&tabelaHash, "Livro de venenos aberto na página 13", "Professor Harrington");
    inserirNaHash(&tabelaHash, "Garrafa de vinho vazia no chão", "Lady Elizabeth");
    inserirNaHash(&tabelaHash, "Pegadas levam ao gazebo", "Mordomo James");
    inserirNaHash(&tabelaHash, "Carta ameaçadora no cofre", "Lady Elizabeth");
    inserirNaHash(&tabelaHash, "Troféu de prata manchado", "Professor Harrington");
    inserirNaHash(&tabelaHash, "Partitura rasgada", "Lady Elizabeth");
    inserirNaHash(&tabelaHash, "Planta venenosa recém-cortada", "Professor Harrington");
    inserirNaHash(&tabelaHash, "Relógio parado às 23:47", "Mordomo James");

    // Cria o mapa da mansão
    Sala *hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Cozinha");

    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Sala de Música");

    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita = criarSala("Jardim");

    hall->esquerda->esquerda->esquerda = criarSala("Escritório Secreto");
    hall->esquerda->esquerda->direita = criarSala("Sala de Troféus");

    hall->esquerda->direita->esquerda = criarSala("Piano Room");

    hall->direita->direita->esquerda = criarSala("Estufa");
    hall->direita->direita->direita = criarSala("Gazebo");

    printf("\n Bem-vindo, Detetive!\n");
    printf(" Um crime foi cometido na mansão.\n");
    printf(" Explore os cômodos, colete pistas e descubra o culpado.\n");
    printf("  Você precisará de pelo menos 2 pistas para acusar alguém.\n");
    printf("\nPressione ENTER para começar...");
    getchar();

    // Fase de exploração
    explorarSalas(hall, &arvorePistas, &tabelaHash);

    // Fase de acusação
    verificarSuspeitoFinal(arvorePistas, &tabelaHash);

    // Libera memória
    liberarArvore(hall);
    liberarArvorePistas(arvorePistas);
    liberarHash(&tabelaHash);

    printf("\n\n Obrigado por jogar Detective Quest!\n");
    printf("═══════════════════════════════════════════════\n\n");

    return 0;
}