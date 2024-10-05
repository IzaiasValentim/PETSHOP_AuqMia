#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    ** Info:
            A versão inicial da Árvore Binária foi implementada seguindo os códigos
            disponibilizados no capítulo 12(doze) do livro: 

            BACKES, André R. Algoritmos e Estruturas de Dados em Linguagem C.
            Rio de Janeiro: LTC, 2023. E-book. ISBN 9788521638315.
            Disponível em: https://app.minhabiblioteca.com.br/#/books/9788521638315/.
            Acesso em: 05 out. 2024.

            Foram realizadas pequenas alterações para que a estrutura de dados lidasse com o nosso tipo de dado.
*/

typedef struct consulta
{
    char usernameProfisional[50];
    char nomeResponsavel[50];
    char contatoResponsavel[100];
    int porteAnimal;
    char status[30];
    float valor;
    int consultaVeterinaria;
    int banho;
    int tosa;
    char detalhes[300];
} Consulta;

typedef struct atendimento
{
    char usernameProfisional[50];
    int dia;
    int mes;
    int ano;
    int qntHorarios;
    // No momento apenas demontrativo. Futuramente será uma heap-binária.
    // A prioridade será de acordo com o horário 1(maior) - 5(menor);
    Consulta consultas[5];
} Atendimento;

// Cada usuário terá sua árvore binária com seus atendimentos. Assim não acontece conflito.
typedef struct NO *Atendimentos;

struct NO
{
    // A inserção será ponderada de acordo com a data;
    Atendimento atendimento;
    struct NO *esq;
    struct NO *dir;
};

// Funções da nossa árvore binária.
Atendimentos *cria_ArvBin();
void libera_ArvBin(Atendimentos *raiz);
int insere_ArvBin(Atendimentos *raiz, Atendimento atendimento);
int remove_ArvBin(Atendimentos *raiz, int valor); // faltando
int estaVazia_ArvBin(Atendimentos *raiz);
int totalNO_ArvBin(Atendimentos *raiz);
int consulta_ArvBin(Atendimentos *raiz, Atendimento *atendimento);
void preordem_ArvBin(Atendimentos *raiz);

// Funções auxiliares
int comparaDatas(Atendimento a1, Atendimento a2);

int main()
{
    Atendimento atendimentos[] = {
        {"Izaias at 1", 12, 9, 2024, 5, {0}},
        {"Izaias at 2", 14, 9, 2024, 5, {0}},
        {"Izaias at 3", 13, 9, 2024, 5, {0}},
        {"Izaias at 4", 15, 9, 2024, 5, {0}},
        {"Izaias at 5", 11, 9, 2024, 5, {0}}};

    Atendimentos *arvore = cria_ArvBin();
    insere_ArvBin(arvore, atendimentos[0]);
    insere_ArvBin(arvore, atendimentos[1]);
    insere_ArvBin(arvore, atendimentos[1]);
    insere_ArvBin(arvore, atendimentos[2]);
    insere_ArvBin(arvore, atendimentos[3]);
    insere_ArvBin(arvore, atendimentos[4]);
    printf("Percruso em pre ordem:\n");
    // Saída esperada: 1 5 2 3 4;
    preordem_ArvBin(arvore);

    printf("\nQuantidade de nos: %d\n", totalNO_ArvBin(arvore));

    // Busca:
    Atendimento pesq[] = {{"Izaias at 2", 14, 9, 2024, 5, {0}}};
    printf("Busca na arvore: %s - %d/%d/%d", pesq->usernameProfisional, pesq[0].dia, pesq[0].mes, pesq[0].ano);

    if (consulta_ArvBin(arvore, pesq))
    {
        printf("\nFoi encontrado %s %d/%d/%d\n", pesq->usernameProfisional, pesq[0].dia, pesq[0].mes, pesq[0].ano);
    }
    else
    {
        printf("\nNenhuma consulta foi encontrada neste dia.");
    }
    return 0;
}

Atendimentos *cria_ArvBin()
{
    Atendimentos *raiz = (Atendimentos *)malloc(sizeof(Atendimentos));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_NO(struct NO *no)
{
    if (no == NULL)
        return;

    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return;

    libera_NO(*raiz); // libera cada nó
    free(raiz);       // libera a raiz
}

int estaVazia_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return 1;
    if (*raiz == NULL)
        return 1;
    return 0;
}

// Pode ser utilizado para ver quantos dias de atendimento o usuário possui.
int totalNO_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int total_esq = totalNO_ArvBin(&((*raiz)->esq));
    int total_dir = totalNO_ArvBin(&((*raiz)->dir));
    return (total_esq + total_dir + 1);
}

void preordem_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return;
    if (*raiz != NULL)
    {
        printf("%s\n", (*raiz)->atendimento.usernameProfisional);
        preordem_ArvBin(&((*raiz)->esq));
        preordem_ArvBin(&((*raiz)->dir));
    }
}

int comparaDatas(Atendimento a1, Atendimento a2)
{
    // -1 se a1 é no passado em relação a a2;
    //  1 se a1 é no futuro em relação a a2;
    //  0 Datas iguais;
    if (a1.ano < a2.ano)
        return -1;
    else if (a1.ano > a2.ano)
        return 1;
    else
    {
        if (a1.mes < a2.mes)
            return -1;
        else if (a1.mes > a2.mes)
            return 1;
        else
        {
            if (a1.dia < a2.dia)
                return -1;
            else if (a1.dia > a2.dia)
                return 1;
            else
                return 0; // Datas iguais
        }
    }
}

int insere_ArvBin(Atendimentos *raiz, Atendimento atendimento)
{
    if (raiz == NULL)
        return 0;
    struct NO *novo;
    novo = (struct NO *)malloc(sizeof(struct NO));
    if (novo == NULL)
        return 0;

    novo->atendimento = atendimento;
    novo->dir = NULL;
    novo->esq = NULL;
    if (*raiz == NULL)
        *raiz = novo;
    else
    {
        struct NO *atual = *raiz;
        struct NO *ant = NULL;
        while (atual != NULL)
        {
            ant = atual;
            int comparacao = comparaDatas(atendimento, atual->atendimento);
            // -1 se a1 é no passado em relação a a2;
            //  1 se a1 é no futuro em relação a a2;
            //  0 Datas iguais;
            if (comparacao == 0)
            {
                printf("Ja existe um atendimento neste dia!\n");
                printf("Realize a alteracao ou a remocao para inserir.\n");
                free(novo);
                return 0; // Elemento já existe, não insere duplicado
            }
            if (comparacao > 0)
                atual = atual->dir;
            else
                atual = atual->esq;
        }

        if (comparaDatas(atendimento, ant->atendimento) > 0)
            ant->dir = novo;
        else
            ant->esq = novo;
    }
    return 1;
}

int consulta_ArvBin(Atendimentos *raiz, Atendimento *atendimento)
{
    if (raiz == NULL)
        return 0; // Árvore inválida ou vazia

    struct NO *atual = *raiz;

    while (atual != NULL)
    {
        // 0 as datas são iguais.
        int comparacao = comparaDatas(*atendimento, atual->atendimento);

        if (comparacao == 0)
        {
            *atendimento = atual->atendimento; // Atualiza com o dado encontrado
            return 1;                          // Atendimento encontrado
        }
        if (comparacao > 0)
            atual = atual->dir; // Vai para o lado direito se a data é maior
        else
            atual = atual->esq; // Vai para o lado esquerdo se a data é menor
    }

    return 0; // Atendimento não encontrado
}
