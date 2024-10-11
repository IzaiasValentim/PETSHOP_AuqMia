#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    ** Info:
            A versão inicial da Heap/Heap Binária foi implementada seguindo os códigos
            disponibilizados no capítulo 7(Sete) do livro:

            BACKES, André R. Algoritmos e Estruturas de Dados em Linguagem C.
            Rio de Janeiro: LTC, 2023. E-book. ISBN 9788521638315.
            Disponível em: https://app.minhabiblioteca.com.br/#/books/9788521638315/.
            Acesso em: 10 out. 2024.

            Foram realizadas pequenas alterações para que a estrutura de dados lidasse com o nosso tipo de dado
            e também para o ajuste no tratamento de inserção por prioridade.
*/

#define MAX_ATENDIMENTOS 8 // Definimos um plantão de atendimentos como sendo 8hrs e média de 1 por hora.

typedef struct consulta
{
    char usernameProfisional[50]; // Veterinário ou Tosador
    char usernameAtendente[50];   // Quem marcou a consulta
    char nomeTutor[50];
    char contatoResponsavel[20]; // Telefone
    char enderecoResponsavel[100];
    char nomeAnimal[50];
    int porteAnimal;         // 1 pequeno 2 Médio 3 Grande
    char status[30];         // Não Realizado, Em andamento, Cancelado, Concluído.
    float valor;             // De acordo com o porte.
    int consultaVeterinaria; // Se é uma consulta.
    int banho;               // Se é banho
    int tosa;                // Se é tosa
    char detalhes[300];      // Detalhes ex: Animal inquieto e durante o banho mordeu o Tosador.
    int horario;
    int concluida;
} Consulta;

typedef struct fila_Consultas
{
    int qtd;                             // Lotação da fila
    Consulta consulta[MAX_ATENDIMENTOS]; // Array com o tipo de dado armazenado na fila.
    int prioridade[MAX_ATENDIMENTOS];    // Prioridade atual da fila.
} Consultas;
/*
    int prioridade[] será o controle de prioridade, da maior para menor: 1 2 3 4 5 6 7 8.
    Logo, um animal no horário 1 será atendido na primeira consulta e assim por diante...
*/


Consultas *cria_FilaPrio();
void libera_FilaPrio(Consultas *fila_Consultas);

int insere_FilaPrio(Consultas *fila_Consultas, Consulta consulta);
void validarFila(Consultas *fila_Consultas, int ultimo_Elemento); // Função complementar de inserção.

void consulta_FilaPrio(Consultas *fila_Consultas); // Retorna a próxima consulta.
void visualizar_FilaPrio(Consultas *fila_Consultas);

int remove_FilaPrio(Consultas *fila_Consultas);
void rebaixarElemento(Consultas *fila_Consultas, int pai); // Função complementar de remoção.

int tamanho_FilaPrio(Consultas *fila_Consultas); // Aux.
int estaCheia_FilaPrio(Consultas *fila_Consultas); // Aux.
int estaVazia_FilaPrio(Consultas *fila_Consultas);  // Aux.

int realizarCheckin(Consultas *fila_Consultas); // Marca a próxima consulta da lista como Concluída e a Remove a prioridade. Mas a consulta ainda permance na lista.

int main()
{
    Consultas *fila_Consultas = cria_FilaPrio();
    // Consultas exemplo. No projeto serão todas iniciadas como vagas. E cada posição será cadastrada pelo atendente.
    Consulta c1 = {
        "Dr. Ana",
        "Carlos",
        "Joao",
        "99999-9999",
        "Rua A, 123",
        "Rex",
        3, // Porte: Grande
        "Nao Realizado",
        120.50,
        1, // Consulta veterinária
        0, // Não é banho
        1, // Tosa
        "Animal muito agitado durante a tosa",
        1, // Horário da consulta
        0  // Não concluída
    };

    Consulta c2 = {
        "Dr. Pedro",
        "Mariana",
        "Vazio",
        "Vazio",
        "Vazio",
        "Vazio",
        0, // Porte: Médio
        "Vago",
        0,
        0, // Consulta veterinária
        0, // Banho
        0, // Tosa
        "Vazio",
        2, // Horário da consulta
        0  // Não concluída
    };

    Consulta c3 = {
        "Dr. Ana",
        "Lucas",
        "Carlos",
        "77777-7777",
        "Rua C, 789",
        "Max",
        1, // Porte: Pequeno
        "Concluido",
        100.00,
        1, // Consulta veterinária
        0, // Não é banho
        1, // Tosa
        "Tosa apos consulta de avaliacao",
        3, // Horário da consulta
        0  // Concluída
    };

    Consulta c4 = {
        "Dr. Pedro",
        "Mariana",
        "Vazio",
        "Vazio",
        "Vazio",
        "Vazio",
        0, // Porte: Médio
        "Vago",
        0,
        0, // Consulta veterinária
        0, // Banho
        0, // Tosa
        "Vazio",
        4, // Horário da consulta
        0  // Não concluída
    };

    Consulta c5 = {
        "Dr. Ana",
        "Carlos",
        "Marcelo",
        "55555-5555",
        "Rua E, 654",
        "Thor",
        2, // Porte: Médio
        "Nao Realizado",
        90.00,
        0, // Não é consulta veterinária
        1, // Banho
        0, // Não é tosa
        "Banho apos cirurgia recente",
        5, // Horário da consulta
        0  // Não concluída
    };

    Consulta c6 = {
        "Dr. Pedro",
        "Fernanda",
        "Julia",
        "44444-4444",
        "Rua F, 987",
        "Daisy",
        1, // Porte: Pequeno
        "Em andamento",
        70.00,
        1, // Consulta veterinária
        0, // Não é banho
        1, // Tosa
        "Tosa especial após exame veterinario",
        6, // Horário da consulta
        0  // Não concluída
    };

    Consulta c7 = {
        "Dr. Joao",
        "Lucas",
        "Roberto",
        "33333-3333",
        "Rua G, 246",
        "Fiona",
        3, // Porte: Grande
        "Nao Realizado",
        110.00,
        0, // Não é consulta veterinária
        1, // Banho
        1, // Tosa
        "Banho e tosa completa para Fiona",
        7, // Horário da consulta
        0  // Não concluída
    };

    Consulta c8 = {
        "Dr. Pedro",
        "Mariana",
        "Vazio",
        "Vazio",
        "Vazio",
        "Vazio",
        0, // Porte: Médio
        "Vago",
        0,
        0, // Consulta veterinária
        0, // Banho
        0, // Tosa
        "Vazio",
        8, // Horário da consulta
        0  // Não concluída
    };

    // Inserções
    insere_FilaPrio(fila_Consultas, c1);

    insere_FilaPrio(fila_Consultas, c2);

    insere_FilaPrio(fila_Consultas, c3);

    insere_FilaPrio(fila_Consultas, c4);

    insere_FilaPrio(fila_Consultas, c5);

    insere_FilaPrio(fila_Consultas, c6);

    insere_FilaPrio(fila_Consultas, c7);

    insere_FilaPrio(fila_Consultas, c8);

    // Exibir a fila de prioridade
    printf("Todas as consultas iniciais: \n\n");
    visualizar_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    consulta_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);
    remove_FilaPrio(fila_Consultas);

    visualizar_FilaPrio(fila_Consultas);
    fila_Consultas = NULL;
    return 0;
}

Consultas *cria_FilaPrio()
{
    Consultas *fp;
    fp = (Consultas *)malloc(sizeof(Consultas));
    if (fp != NULL)
        fp->qtd = 0;
    return fp;
}

void libera_FilaPrio(Consultas *fila_Consultas)
{
    free(fila_Consultas->consulta);
    free(fila_Consultas->prioridade);
    free(fila_Consultas);
}

int estaCheia_FilaPrio(Consultas *fila_Consultas)
{
    // -1 erro, 1 está cheia, 0 não está cheia.
    if (fila_Consultas == NULL)
    {
        return -1;
    }

    return (fila_Consultas->qtd == MAX_ATENDIMENTOS);
}

int estaVazia_FilaPrio(Consultas *fila_Consultas)
{
    // -1 erro, 1 está vazia, 0 possui elementos;
    if (fila_Consultas == NULL)
    {
        return -1;
    }
    return (fila_Consultas->qtd == 0);
}

void validarFila(Consultas *fila_Consultas, int ultimo_Elemento)
{
    int filho, pai;

    // Começamos do último elemento inserido e subimos até a raiz
    filho = ultimo_Elemento;
    pai = (filho - 2) / 2; // Cálculo do índice do pai

    // Enquanto o filho tiver prioridade MAIOR que o pai, trocamos e continuamos subindo
    while ((filho > 0) && fila_Consultas->prioridade[filho] < fila_Consultas->prioridade[pai])
    {
        // Troca os elementos
        Consulta consultaTemp = fila_Consultas->consulta[filho];
        int temp = fila_Consultas->prioridade[filho];

        fila_Consultas->consulta[filho] = fila_Consultas->consulta[pai];
        fila_Consultas->prioridade[filho] = fila_Consultas->prioridade[pai];

        fila_Consultas->consulta[pai] = consultaTemp;
        fila_Consultas->prioridade[pai] = temp;

        // Atualiza os índices para a próxima iteração
        filho = pai;
        pai = (filho - 2) / 2;
    }
}

int insere_FilaPrio(Consultas *fila_Consultas, Consulta consulta)
{
    if (fila_Consultas == NULL)
        return 0;
    if (fila_Consultas->qtd == MAX_ATENDIMENTOS)
    {
        printf("\nEste dia de atendimentos esta lotado :c\n");
        return 0;
    }
    fila_Consultas->qtd++;
    fila_Consultas->consulta[fila_Consultas->qtd - 1] = consulta;
    fila_Consultas->prioridade[fila_Consultas->qtd - 1] = consulta.horario;

    // strcpy(fp->dados[fp->qtd].nome, nome);
    // fp->dados[fp->qtd].prio = prio;
    validarFila(fila_Consultas, fila_Consultas->qtd - 1);
    // promoverElemento (fp, fp->qtd);
    return 1; // Sucesso.
}

void rebaixarElemento(Consultas *fp, int pai)
{
    int temp;
    int filho = 2 * pai + 1;
    while (filho < fp->qtd)
    {
        if (filho < fp->qtd - 1)
            if (fp->prioridade[filho] > fp->prioridade[filho + 1])
                filho++;
        if (fp->prioridade[pai] <= fp->prioridade[filho])
            break;

        temp = fp->prioridade[pai];
        fp->prioridade[pai] = fp->prioridade[filho];
        fp->prioridade[filho] = temp;

        pai = filho;
        filho = 2 * pai + 1;
    }
}

int remove_FilaPrio(Consultas *fp)
{
    if (fp == NULL)
        return 0;
    if (fp->qtd == 0)
        return 0;
    // Marca a consulta da prioridade atual da lista como concluido.
    if (realizarCheckin(fp))
    {
        fp->qtd--;
        fp->prioridade[0] = fp->prioridade[fp->qtd];
        rebaixarElemento(fp, 0);
        return 1;
    }
    return 0;
}

int realizarCheckin(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL || fila_Consultas->qtd == 0)
        return 0;
    int i;

    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (fila_Consultas->prioridade[0] == fila_Consultas->consulta[i].horario)
        {
            fila_Consultas->consulta[i].concluida = 1;
            strcpy(fila_Consultas->consulta[i].status, "Concluida");
            return 1;
        }
    }
    return 1;
}

void consulta_FilaPrio(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL || fila_Consultas->qtd == 0)
        return;
    int i;

    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (fila_Consultas->prioridade[0] == fila_Consultas->consulta[i].horario)
        {
            printf("Consulta %d - Horário: %d, Tutor: %s, Animal: %s, Status: %s, Concluida?: %d, Prio: %d\n",
                   fila_Consultas->consulta[i].horario,
                   fila_Consultas->consulta[i].horario,
                   fila_Consultas->consulta[i].nomeTutor,
                   fila_Consultas->consulta[i].nomeAnimal,
                   fila_Consultas->consulta[i].status,
                   fila_Consultas->consulta[i].concluida,
                   fila_Consultas->prioridade[0]);
        }
    }
}

void visualizar_FilaPrio(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("Fila de consultas esta vazia ou nao existe.\n");
        return;
    }

    printf("Lista de proximas Consultas (por prioridade de horarios):\n");
    int i;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        printf("Consulta %d - Horário: %d, Tutor: %s, Animal: %s, Status: %s, Concluida?: %d\n",
               i + 1,
               fila_Consultas->consulta[i].horario,
               fila_Consultas->consulta[i].nomeTutor,
               fila_Consultas->consulta[i].nomeAnimal,
               fila_Consultas->consulta[i].status,
               fila_Consultas->consulta[i].concluida);
    }
}