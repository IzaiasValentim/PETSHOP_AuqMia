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

/*
    int prioridade[] será o controle de prioridade, da maior para menor: 1 2 3 4 5 6 7 8.
    Logo, um animal no horário 1 será atendido na primeira consulta e assim por diante...
*/

typedef struct fila_Consultas
{
    int qtd;                             // Lotação da fila
    Consulta consulta[MAX_ATENDIMENTOS]; // Array com o tipo de dado armazenado na fila.
    int prioridade[MAX_ATENDIMENTOS];    // Prioridade atual da fila.
} Consultas;

Consultas *cria_FilaPrio(); // Cria a fila de prioridade com o array de consultas em estado de consultas vagas.
void libera_FilaPrio(Consultas *fila_Consultas);

int insere_FilaPrio(Consultas *fila_Consultas, int horario);      // Adiciona um horário de atendimento à fila.
void validarFila(Consultas *fila_Consultas, int ultimo_Elemento); // Função complementar de inserção.

void consulta_FilaPrio(Consultas *fila_Consultas);   // Retorna a próxima consulta.
void visualizar_FilaPrio(Consultas *fila_Consultas); // Retorna a lista total.

int remove_FilaPrio(Consultas *fila_Consultas);            // Finaliza a próxima consulta e realiza o checkin.
void rebaixarElemento(Consultas *fila_Consultas, int pai); // Função complementar de remoção.
int realizarCheckin(Consultas *fila_Consultas);            // Marca a próxima consulta da lista como Concluída e a Remove a prioridade. Mas a consulta ainda permance na lista.

int tamanho_FilaPrio(Consultas *fila_Consultas);   // Aux.
int estaCheia_FilaPrio(Consultas *fila_Consultas); // Aux.
int estaVazia_FilaPrio(Consultas *fila_Consultas); // Aux.

int marcarConsulta(Consultas *fila_Consultas);
int atualizarConsulta(Consultas *fila_Consultas);
int desmarcarConsulta(Consultas *fila_Consultas);
int main()
{
    Consultas *fila_Consultas = cria_FilaPrio();

    if (marcarConsulta(fila_Consultas))
    {
        // Pode ser usado para validar o cadastro de cada consulta.
    }

    if (atualizarConsulta(fila_Consultas))
    {
        // Pode ser usado para validar o cadastro de cada consulta.
    }
    desmarcarConsulta(fila_Consultas);

    consulta_FilaPrio(fila_Consultas);   // Retora a próxima consulta, vagas ou não.
    visualizar_FilaPrio(fila_Consultas); // Retorna a lista total de consultas.
    remove_FilaPrio(fila_Consultas);     // Remove a próxima consulta(apenas do array de prioridade) e realiza o chekin da mesma.

    libera_FilaPrio(fila_Consultas);
    fila_Consultas = NULL;
    return 0;
}

Consultas *cria_FilaPrio()
{
    Consultas *fp;
    fp = (Consultas *)malloc(sizeof(Consultas));
    if (fp != NULL)
    {
        fp->qtd = 0;

        // Preenche o array de consultas com valores padrões e horários corretos
        for (int i = 0; i < MAX_ATENDIMENTOS; i++)
        {
            strcpy(fp->consulta[i].usernameProfisional, "Vazio");
            strcpy(fp->consulta[i].usernameAtendente, "Vazio");
            strcpy(fp->consulta[i].nomeTutor, "Vazio");
            strcpy(fp->consulta[i].contatoResponsavel, "Vazio");
            strcpy(fp->consulta[i].enderecoResponsavel, "Vazio");
            strcpy(fp->consulta[i].nomeAnimal, "Vazio");
            fp->consulta[i].porteAnimal = 0; // Sem porte
            strcpy(fp->consulta[i].status, "Vago");
            fp->consulta[i].valor = 0.0;
            fp->consulta[i].consultaVeterinaria = 0;
            fp->consulta[i].banho = 0;
            fp->consulta[i].tosa = 0;
            strcpy(fp->consulta[i].detalhes, "Vazio");
            fp->consulta[i].horario = i + 1; // Horários de 1 a 8
            fp->consulta[i].concluida = 0;

            insere_FilaPrio(fp, i + 1);
        }
    }
    return fp;
}

void libera_FilaPrio(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        return; // Se a fila for nula, não há nada para liberar.
    }

    // Libera o array de consultas
    if (fila_Consultas->consulta != NULL)
    {
        free(fila_Consultas->consulta);
    }

    // Libera o array de prioridades
    if (fila_Consultas->prioridade != NULL)
    {
        free(fila_Consultas->prioridade);
    }

    // Libera a estrutura principal da fila
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

int insere_FilaPrio(Consultas *fila_Consultas, int horario)
{
    if (fila_Consultas == NULL)
        return 0;
    if (fila_Consultas->qtd == MAX_ATENDIMENTOS)
    {
        printf("\nEste dia de atendimentos esta lotado :c\n");
        return 0;
    }
    fila_Consultas->qtd++;
    fila_Consultas->prioridade[fila_Consultas->qtd - 1] = horario;

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

int marcarConsulta(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("Fila de consultas nao existe.\n");
        return 0;
    }

    // Exibe horários livres
    printf("\nHorarios Preenchidos: ");
    int i, horarioEscolhido = -1;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (strcmp(fila_Consultas->consulta[i].status, "Agendado") == 0)
        {
            printf(" %d ", fila_Consultas->consulta[i].horario);
        }
    }

    // Solicita ao usuário o horário para marcar
    printf("\nInforme o horario que deseja agendar (1 a 8): ");
    scanf("%d", &horarioEscolhido);

    // Verifica se o horário está disponível
    if (horarioEscolhido < 1 || horarioEscolhido > 8 || strcmp(fila_Consultas->consulta[horarioEscolhido - 1].status, "Agendado") == 0)
    {
        printf("Horario inválido ou já agendado.\n");
        return 0;
    }

    // Coleta os dados da consulta
    Consulta novaConsulta;
    novaConsulta.horario = horarioEscolhido;
    novaConsulta.concluida = 0; // Inicialmente, a consulta não está concluída

    printf("Informe o nome do profissional: ");
    scanf(" %[^\n]s", novaConsulta.usernameProfisional);

    printf("Informe o nome do atendente: ");
    scanf(" %[^\n]s", novaConsulta.usernameAtendente);

    printf("Informe o nome do tutor: ");
    scanf(" %[^\n]s", novaConsulta.nomeTutor);

    printf("Informe o telefone de contato: ");
    scanf(" %[^\n]s", novaConsulta.contatoResponsavel);

    printf("Informe o endereco do tutor: ");
    scanf(" %[^\n]s", novaConsulta.enderecoResponsavel);

    printf("Informe o nome do animal: ");
    scanf(" %[^\n]s", novaConsulta.nomeAnimal);

    printf("Informe o porte do animal (1 - Pequeno, 2 - Médio, 3 - Grande): ");
    scanf("%d", &novaConsulta.porteAnimal);

    printf("Informe o valor da consulta: ");
    scanf("%f", &novaConsulta.valor);

    printf("É uma consulta veterinaria? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.consultaVeterinaria);

    printf("É um banho? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.banho);

    printf("É uma tosa? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.tosa);

    printf("Informe os detalhes da consulta: ");
    scanf(" %[^\n]s", novaConsulta.detalhes);

    // Atualiza o status da consulta e insere na fila
    strcpy(novaConsulta.status, "Agendado");
    fila_Consultas->consulta[horarioEscolhido - 1] = novaConsulta;
    printf("Consulta agendada com sucesso no horário %d!\n", horarioEscolhido);

    return 1; // Sucesso
}

int atualizarConsulta(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("Fila de consultas nao existe.\n");
        return 0;
    }

    // Exibe horários livres
    printf("\nHorarios prenchidos: ");
    int i, horarioEscolhido = -1;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (strcmp(fila_Consultas->consulta[i].status, "Agendado") == 0)
        {
            printf(" %d ", fila_Consultas->consulta[i].horario);
        }
    }

    // Solicita ao usuário o horário para marcar
    printf("\nInforme o horario que deseja agendar (1 a 8): ");
    scanf("%d", &horarioEscolhido);

    // Verifica se o horário está disponível
    if (horarioEscolhido < 1 || horarioEscolhido > 8 || strcmp(fila_Consultas->consulta[horarioEscolhido - 1].status, "Agendado") != 0)
    {
        printf("Horario inválido, está livre ou concluído.\n");
        return 0;
    }

    // Coleta os novos dados da consulta
    Consulta novaConsulta;
    novaConsulta.horario = horarioEscolhido;
    novaConsulta.concluida = 0; // Inicialmente, a consulta não está concluída

    printf("Informe o nome do profissional: ");
    scanf(" %[^\n]s", novaConsulta.usernameProfisional);

    printf("Informe o nome do atendente: ");
    scanf(" %[^\n]s", novaConsulta.usernameAtendente);

    printf("Informe o nome do tutor: ");
    scanf(" %[^\n]s", novaConsulta.nomeTutor);

    printf("Informe o telefone de contato: ");
    scanf(" %[^\n]s", novaConsulta.contatoResponsavel);

    printf("Informe o endereco do tutor: ");
    scanf(" %[^\n]s", novaConsulta.enderecoResponsavel);

    printf("Informe o nome do animal: ");
    scanf(" %[^\n]s", novaConsulta.nomeAnimal);

    printf("Informe o porte do animal (1 - Pequeno, 2 - Médio, 3 - Grande): ");
    scanf("%d", &novaConsulta.porteAnimal);

    printf("Informe o valor da consulta: ");
    scanf("%f", &novaConsulta.valor);

    printf("É uma consulta veterinaria? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.consultaVeterinaria);

    printf("É um banho? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.banho);

    printf("É uma tosa? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.tosa);

    printf("Informe os detalhes da consulta: ");
    scanf(" %[^\n]s", novaConsulta.detalhes);

    // Atualiza o status da consulta e insere na fila
    strcpy(novaConsulta.status, "Agendado");
    fila_Consultas->consulta[horarioEscolhido - 1] = novaConsulta;
    printf("Consulta atualizada com sucesso no horario %d!\n", horarioEscolhido);

    return 1; // Sucesso
}

int desmarcarConsulta(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("\nFila de consultas nao existe.\n");
        return 0;
    }

    // Exibe horários livres
    printf("\nHorarios prenchidos:");
    int i, horarioEscolhido = -1;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (strcmp(fila_Consultas->consulta[i].status, "Agendado") == 0)
        {
            printf(" %d ", fila_Consultas->consulta[i].horario);
        }
    }

    // Solicita ao usuário o horário para marcar
    printf("\nInforme o horario que deseja desmarcar (1 a 8): ");
    scanf("%d", &horarioEscolhido);

    // Verifica se o horário está disponível
    if (horarioEscolhido < 1 || horarioEscolhido > 8 || strcmp(fila_Consultas->consulta[horarioEscolhido - 1].status, "Agendado") != 0)
    {
        printf("Horario inválido, está livre ou concluído.\n");
        return 0;
    }

    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].usernameProfisional, "Vazio");
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].usernameAtendente, "Vazio");
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].nomeTutor, "Vazio");
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].contatoResponsavel, "Vazio");
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].enderecoResponsavel, "Vazio");
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].nomeAnimal, "Vazio");
    fila_Consultas->consulta[horarioEscolhido - 1].porteAnimal = 0; // Sem porte
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].status, "Vago");
    fila_Consultas->consulta[horarioEscolhido - 1].valor = 0.0;
    fila_Consultas->consulta[horarioEscolhido - 1].consultaVeterinaria = 0;
    fila_Consultas->consulta[horarioEscolhido - 1].banho = 0;
    fila_Consultas->consulta[horarioEscolhido - 1].tosa = 0;
    strcpy(fila_Consultas->consulta[horarioEscolhido - 1].detalhes, "Vazio");
    fila_Consultas->consulta[horarioEscolhido - 1].horario = horarioEscolhido; // Horários de 1 a 8
    fila_Consultas->consulta[horarioEscolhido - 1].concluida = 0;
    printf("Consulta desmarcada com sucesso no horario %d!\n", horarioEscolhido);
    return 1; // Sucesso
}