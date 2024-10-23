#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MAX_ATENDIMENTOS 8

typedef struct consulta
{
    char usernameProfisional[50];
    char usernameAtendente[50];
    char nomeTutor[50];
    char contatoResponsavel[20];
    char enderecoResponsavel[100];
    char nomeAnimal[50];
    int porteAnimal;
    char status[30];
    float valor;
    int consultaVeterinaria;
    int banho;
    int tosa;
    char detalhes[300];
    int horario;
    int concluida;
} Consulta;

typedef struct fila_Consultas
{
    int qtd;
    Consulta consulta[MAX_ATENDIMENTOS];
    int prioridade[MAX_ATENDIMENTOS];
} Consultas;

typedef struct atendimento
{
    char usernameProfisional[50];
    int dia;
    int mes;
    int ano;
    int qntHorarios;
    Consultas *consultas;
} Atendimento;


typedef struct NO *Atendimentos;

struct NO
{
    Atendimento atendimento;
    struct NO *esq;
    struct NO *dir;
};

typedef struct Usuario
{
    char email[50];
    char username[50];
    char senha[20];
    int vendedor;
    int veterinario;
    int tosador;
    int gerente;
    struct Usuario *prox;
    Atendimentos *arvoreAtendimentos;
} Usuario;

typedef struct hash
{
    int quantidade, TAM_TAB;
    Usuario **usuarios;
} Hash;

void boasVindas();
void menuVendedor(Hash *usuarios, Usuario *logado, Usuario *consultores, int quantidadeConsultores);
void menuVeterinario(Hash *usuarios, Usuario *logado);
void menuTosador(Hash *usuarios, Usuario *logado);
void menuGerente(Hash *usuarios, Usuario *gerenteLogado, Usuario * consultores, int quantidadeConsultores);
int login(Hash *usuarios, Usuario *usuarioLogado);

// Funções iniciais do Hash
Hash *criaHash(int TABLE_SIZE);
int chaveDivisao(int chave, int TABLE_SIZE);
int chaveTabelaPorUsername(char *str);
int inserirUsuario(Hash *ha, Usuario usuario);
int buscaUsuarioPorUsername(Hash *ha, char *nome, Usuario *usuario);
void liberarHash(Hash *ha);

// Funções CRUD Usuário:
int cadastrarUsuario(Hash *ha);
int validarEmail(Hash *ha, char *email);
int verificaSeUsuarioExiste(Hash *ha, int posicao, char *username);
int deletarUsuario(Hash *ha, Usuario *gerenteLogado);
int deletarUsuarioSimplificado(Hash *ha, Usuario usuario);
void visualizarTodosUsuarios(Hash *usuarios);
int atualizarUsuario(Hash *ha, Usuario *usuarioAntigo);
int atualizarCargoDeFuncionario(Hash *ha, char *username);
Usuario *usuariosVeterinariosTosadores(Hash *tabela, int *qtd_resultados);
Usuario selecionarProfissional(Usuario *usuarios, int quantidade, int tosador, int veterinario);

// Funções da nossa Árvore Binária e Atendimentos.
Atendimentos *cria_ArvBin();

// Funções auxiliares para inserçã|consulta de um atendimento
int comparaDatas(Atendimento a1, Atendimento a2);
void setarDataNoAtendimento(Atendimento *atendimento);

int insere_ArvBin(Atendimentos *raiz, Atendimento atendimento);
int cadastrarAtendimento(Atendimentos *raiz, Usuario *usuario);
void preordem_ArvBin(Atendimentos *raiz);
int totalNO_ArvBin(Atendimentos *raiz);
void libera_ArvBin(Atendimentos *raiz);
int estaVazia_ArvBin(Atendimentos *raiz);
int consulta_ArvBin(Atendimentos *raiz, Atendimento *atendimento);
void solicitarDiaDeAtendimento(Atendimento *atendimento);
int remove_ArvBin(Atendimentos *raiz, int valor);
float calculaRelatorioMensalTotal(Atendimentos *raiz, int mes, int ano, int escopo);

// Funções Heap|Consultas
Consultas *cria_FilaPrio(); 
void libera_FilaPrio(Consultas *fila_Consultas);
int insere_FilaPrio(Consultas *fila_Consultas, int horario);      
void validarFila(Consultas *fila_Consultas, int ultimo_Elemento); 
int marcarConsulta(Consultas *fila_Consultas, char * usernameAtendente, char * usernameProfissional);
void consulta_FilaPrio(Consultas *fila_Consultas);  
void visualizar_FilaPrio(Consultas *fila_Consultas); 
int atualizarConsulta(Consultas *fila_Consultas);
int remove_FilaPrio(Consultas *fila_Consultas);            
void rebaixarElemento(Consultas *fila_Consultas, int pai); 
int realizarCheckin(Consultas *fila_Consultas);            
int desmarcarConsulta(Consultas *fila_Consultas);
int estaCheia_FilaPrio(Consultas *fila_Consultas); 
int estaVazia_FilaPrio(Consultas *fila_Consultas); 

void limparConsole(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausa(){
    #ifdef _WIN32
        system("pause");
    #else
        printf("Pressione uma tecla para continuar!");
        getchar();
    #endif
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    // Lista de usuários cadastrados (para exemplo)
    Usuario usuarios[] = {
        {"vendedor@petshop.com", "Atendente", "12345", 1, 0, 0, 0, NULL, NULL},
        {"veterinario@petshop.com", "Veterinario", "12345", 0, 1, 0, 0, NULL, NULL},
        {"tosador@petshop.com", "Tosador", "12345", 0, 0, 1, 0, NULL, NULL},
        {"gerente@petshop.com", "Gerente", "12345", 0, 0, 0, 1, NULL, NULL}};

    int totalUsuarios = 17;

    Hash *Usuarios = criaHash(totalUsuarios);
    inserirUsuario(Usuarios, usuarios[0]);
    inserirUsuario(Usuarios, usuarios[1]);
    inserirUsuario(Usuarios, usuarios[2]);
    inserirUsuario(Usuarios, usuarios[3]);

    Usuario usuarioLogado;
    int continuar = 1; // Variável para controlar o loop do sistema

    // Loop principal do sistema
    while (continuar)
    {
        boasVindas();
        // Processo de login
        if (login(Usuarios, &usuarioLogado))
        {
            printf("\nLogin bem-sucedido!\n");

            // Verifica o tipo de usuário e exibe o menu correspondente
            if (usuarioLogado.vendedor)
            {
                int qntConsultores = 0;
                Usuario *consultores = usuariosVeterinariosTosadores(Usuarios, &qntConsultores);
                menuVendedor(Usuarios, &usuarioLogado, consultores, qntConsultores);
            }
            else if (usuarioLogado.veterinario)
            {
                menuVeterinario(Usuarios, &usuarioLogado);
            }
            else if (usuarioLogado.tosador)
            {
                menuTosador(Usuarios, &usuarioLogado);
            }
            else if (usuarioLogado.gerente)
            {
                int qntConsultores = 0;
                Usuario *consultores = usuariosVeterinariosTosadores(Usuarios, &qntConsultores);
                menuGerente(Usuarios, &usuarioLogado, consultores, qntConsultores);
            }
            else
            {
                printf("Usuário inválido, confira suas credênciais.\n");
            }
        }
        else
        {
            printf("\nEmail ou senha incorretos!\n");
        }

        // Pergunta se o usuário deseja continuar ou sair do sistema
        printf("\nDeseja continuar no sistema? (1 - Sim / 0 - Nao): ");
        scanf("%d", &continuar);
        limparConsole();
    }
    liberarHash(Usuarios);    
    printf("Obrigado por usar o sistema Pet Shop. Até mais!\n");
    return 0;
}

void boasVindas()
{
    Sleep(1000);
    printf("        へ   AuqMia System  ╱|\n");
    printf("    ૮ -  ՛)        ♡       (` - 7\n");
    printf("    /  ⁻ ៸|                |、⁻〵\n");
    printf(" 乀(_,ل ل   --^_--^-       じし_,)ノ \n");
    printf("--^___--^--          _^--^--_--__^^--\n");
    Sleep(1000);
    printf("====    Bem-vindo ao Pet Shop    ====\n\n");
    Sleep(400);
    printf("====      Realize o login :      ====\n\n");
}

void menuVendedor(Hash *usuarios, Usuario *logado, Usuario *consultores, int quantidadeConsultores)
{
    int opcao;
    do
    {
        printf("\n=== Menu Atendente ===\n");
        printf("1. Atendimento veterinário\n");
        printf("2. Banho/Tosa\n");
        printf("3. Atualizar minhas informações \n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            opcao = 0;
            printf("-> Atendimento veterinário: \n");
            printf("1. Horarios disponiveis.\n");
            printf("2. Marcar atendimento.\n");
            printf("3. Desmarcar atendimento.\n");
            printf("4. Alterar atendimento.\n");
            printf("5. voltar\n");
            scanf("%d", &opcao);
            if (opcao == 1)
            {
                printf("Horários disponíveis: \n");
                Usuario vet = selecionarProfissional(consultores, quantidadeConsultores, 0, 1);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(vet.arvoreAtendimentos, &atendimentoBusca))
                {
                    visualizar_FilaPrio(atendimentoBusca.consultas);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }
                opcao = 0;
                break;
            }
            else if (opcao == 2)
            {
                printf("Horários disponíveis: \n");
                Usuario vet = selecionarProfissional(consultores, quantidadeConsultores, 0, 1);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(vet.arvoreAtendimentos, &atendimentoBusca))
                {
                    //strcpy(usuarioAtualizado.email, usuarioAntigo->email);
                    marcarConsulta(atendimentoBusca.consultas, logado->username, vet.username);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }
                opcao = 0;
                break;
            }
            else if (opcao == 3)
            {
                printf("Horários disponíveis: \n");
                Usuario vet = selecionarProfissional(consultores, quantidadeConsultores, 0, 1);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(vet.arvoreAtendimentos, &atendimentoBusca))
                {
                    desmarcarConsulta(atendimentoBusca.consultas);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }

                opcao = 0;
                break;
            }
            else if (opcao == 4)
            {
                printf("Horários disponíveis: \n");
                Usuario vet = selecionarProfissional(consultores, quantidadeConsultores, 0, 1);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(vet.arvoreAtendimentos, &atendimentoBusca))
                {
                    atualizarConsulta(atendimentoBusca.consultas);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }

                opcao = 0;
                break;
            }
            else if (opcao == 5)
            {
                opcao = 0;
                break;
            }
            break;
        case 2:
            opcao = 0;
            printf("-> Banho e Tosa: \n");
            printf("1. Horários disponíveis.\n");
            printf("2. Marcar Banho/Tosa.\n");
            printf("3. Desmarcar Banho/Tosa.\n");
            printf("4. Alterar Banho/Tosa.\n");
            printf("5. voltar\n");
            scanf("%d", &opcao);
            if (opcao == 1)
            {
                Usuario tosador = selecionarProfissional(consultores, quantidadeConsultores, 1, 0);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(tosador.arvoreAtendimentos, &atendimentoBusca))
                {
                    visualizar_FilaPrio(atendimentoBusca.consultas);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }
                opcao = 0;
                break;
            }
            else if (opcao == 2)
            {
                Usuario tosador = selecionarProfissional(consultores, quantidadeConsultores, 1, 0);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(tosador.arvoreAtendimentos, &atendimentoBusca))
                {
                    marcarConsulta(atendimentoBusca.consultas, logado->username, tosador.username);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }
                opcao = 0;
                break;
            }
            else if (opcao == 3)
            {
                printf("Horários disponíveis: \n");
                Usuario tosador = selecionarProfissional(consultores, quantidadeConsultores, 1, 0);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(tosador.arvoreAtendimentos, &atendimentoBusca))
                {
                    desmarcarConsulta(atendimentoBusca.consultas);
                    opcao = 0;
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }

                break;
            }
            else if (opcao == 4)
            {
                printf("Horários disponíveis: \n");
                Usuario tosador = selecionarProfissional(consultores, quantidadeConsultores, 1, 0);
                Atendimento atendimentoBusca;
                solicitarDiaDeAtendimento(&atendimentoBusca);
                if (consulta_ArvBin(tosador.arvoreAtendimentos, &atendimentoBusca))
                {
                    atualizarConsulta(atendimentoBusca.consultas);
                }
                else
                {
                    printf("\nNão existe um atendimento neste dia!\n");
                }
                opcao = 0;
                break;
            }
            else if (opcao == 5)
            {
                opcao = 0;
                break;
            }
            break;
        case 3:
            if (atualizarUsuario(usuarios, logado))
            {
                printf("Atualização realizada com sucesso!\n");
            }
            else
            {
                printf("Erro ao realizar atualização!\n");
            }
            opcao = 0;
            break;
        case 4:
            printf("Saindo do menu vendedor...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
        pausa();
        limparConsole();
    } while (opcao != 4);
}

void menuVeterinario(Hash *usuarios, Usuario *logado)
{
    int opcao;
    do
    {
        printf("=== Menu Veterinário ===\n");
        printf("1. Cadastrar dia de atendimento\n");
        printf("2. Realizar checkin\n"); 
        printf("3. Visualizar atendimentos\n");
        printf("4. Visualizar consultas do dia\n");
        printf("5. Consultar consultas por dia\n");
        printf("6. Atualizar minhas informações.\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        Atendimento *atendimentoBusca = (Atendimento *)malloc(sizeof(Atendimento));
        switch (opcao)
        {
        case 1:
            cadastrarAtendimento(logado->arvoreAtendimentos, logado);
            break;
        case 2:
            setarDataNoAtendimento(atendimentoBusca);
            if (consulta_ArvBin(logado->arvoreAtendimentos, atendimentoBusca))
            {
                remove_FilaPrio(atendimentoBusca->consultas);
            }
            else
            {
                printf("\nNão existe um atendimento neste dia!\n");
            }
            break;
        case 3:
            printf("Visualizar atendimentos...\n");
            preordem_ArvBin(logado->arvoreAtendimentos);
            break;
        case 4:
            setarDataNoAtendimento(atendimentoBusca);
            if (consulta_ArvBin(logado->arvoreAtendimentos, atendimentoBusca))
            {
                visualizar_FilaPrio(atendimentoBusca->consultas);
            }
            else
            {
                printf("\nNão existe um atendimento neste dia!\n");
            }
            break;
        case 5:
            solicitarDiaDeAtendimento(atendimentoBusca);
            if (consulta_ArvBin(logado->arvoreAtendimentos, atendimentoBusca))
            {
                visualizar_FilaPrio(atendimentoBusca->consultas);
            }
            else
            {
                printf("\nNão existe um atendimento neste dia!\n");
            }
            break;
        case 6:
            if (atualizarUsuario(usuarios, logado))
            {
                printf("Atualização realizada com sucesso!\n");
            }
            else
            {
                printf("Erro ao realizar atualização!\n");
            }
            opcao = 0;
            break;
        case 7:
            printf("Saindo do menu veterinário...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
        free(atendimentoBusca);
        pausa();
        limparConsole();
    } while (opcao != 7);
}

void menuTosador(Hash *usuarios, Usuario *logado)
{
    int opcao;
    do
    {
        printf("=== Menu Tosador ===\n");
        printf("1. Cadastrar dia de atendimento\n");
        printf("2. Realizar checkin\n");
        printf("3. Visualizar atendimentos\n");
        printf("4. Visualizar consultas do dia\n");
        printf("5. Consultar consultas por dia\n");
        printf("6. Atualizar minhas informações.\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        Atendimento *atendimentoBusca = (Atendimento *)malloc(sizeof(Atendimento));
        switch (opcao)
        {
        case 1:
            cadastrarAtendimento(logado->arvoreAtendimentos, logado);
            break;
        case 2:
            setarDataNoAtendimento(atendimentoBusca);
            if (consulta_ArvBin(logado->arvoreAtendimentos, atendimentoBusca))
            {
                remove_FilaPrio(atendimentoBusca->consultas);
            }
            else
            {
                printf("\nNão existe um atendimento neste dia!\n");
            }
            break;
        case 3:
            printf("Visualizar atendimentos...\n");
            preordem_ArvBin(logado->arvoreAtendimentos);
            break;
        case 4:
            setarDataNoAtendimento(atendimentoBusca);
            if (consulta_ArvBin(logado->arvoreAtendimentos, atendimentoBusca))
            {
                visualizar_FilaPrio(atendimentoBusca->consultas);
            }
            else
            {
                printf("Não existe um atendimento neste dia!\n");
            }
            break;
        case 5:
            solicitarDiaDeAtendimento(atendimentoBusca);
            if (consulta_ArvBin(logado->arvoreAtendimentos, atendimentoBusca))
            {
                visualizar_FilaPrio(atendimentoBusca->consultas);
            }
            else
            {
                printf("\nNão existe um atendimento neste dia!\n");
            }
            break;
        case 6:
            if (atualizarUsuario(usuarios, logado))
            {
                printf("Atualização realizada com sucesso!\n");
            }
            else
            {
                printf("Erro ao realizar atualização!\n");
            }
            opcao = 0;
            break;
        case 7:
            printf("Saindo do menu banho/tosa...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
        free(atendimentoBusca);
        pausa();
        limparConsole();
    } while (opcao != 7);
}

void menuGerente(Hash *usuarios, Usuario *gerenteLogado, Usuario * consultores, int quantidadeConsultores)
{
    int opcao;
    do
    {
        printf("=== Menu Gerente ===\n");
        printf("1. Gerenciar Funcionários\n");
        printf("2. Ver Relatórios\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Gerenciando funcionários...\n");
            opcao = 0;
            printf("-> Gerenciar Funcionários.\n");
            printf("1. Cadastrar Usuário.\n");
            printf("2. Deletar Usuário.\n");
            printf("3. Visualizar Usuários.\n");
            printf("4. Atualizar minhas informações.\n");
            printf("5. Atualizar cargo de um funcionário.\n");
            printf("6. voltar\n");
            scanf("%d", &opcao);
            if (opcao == 1)
            {
                cadastrarUsuario(usuarios);
                opcao = 0;
                break;
            }
            else if (opcao == 2)
            {
                deletarUsuario(usuarios, gerenteLogado);
                opcao = 0;
                break;
            }
            else if (opcao == 3)
            {
                visualizarTodosUsuarios(usuarios);
            }
            else if (opcao == 4)
            {
                if (atualizarUsuario(usuarios, gerenteLogado))
                {
                    printf("Atualização realizada com sucesso!\n");
                }
                else
                {
                    printf("Erro ao realizar atualização!\n");
                }
                opcao = 0;
                break;
            }
            else if (opcao == 5)
            {
                visualizarTodosUsuarios(usuarios);
                char username[50];
                printf("Informe o username: ");
                scanf("%s", username);
                if (atualizarCargoDeFuncionario(usuarios, username))
                {
                    printf("Cargo atualizado com sucesso!\n");
                }
                else
                {
                    printf("Erro ao atualizar cargo!\n");
                }
            }
            else if (opcao == 6)
            {
                opcao = 0;
                break;
            }
            opcao = 0;
            break;
        case 2:
            opcao = 0;
            printf("-> Visualização de relatórios:\n");
            printf("1. Relatório mensal.\n");
            printf("2. voltar\n");
            scanf("%d", &opcao);
            if (opcao == 1)
            {
                int mes, ano, escopo;
                float balancoTotal = 0.0;
                printf("Informe o mês(xx) e ano(xxxx) de interesse de interesse:");
                scanf("%d %d",&mes, &ano);
                if(mes>=1 && mes<=12 && ano > 0){
                    printf("Escolha uma opção de relatório:\n");
                    printf("0 - Incluir todas as consultas\n");
                    printf("1 - Incluir apenas as consultas Veterinárias\n");
                    printf("2 - Incluir apenas Banho/Tosa\n");
                    printf("Opção: ");
                    scanf("%d",&escopo);
                    if(escopo>=0 && escopo <=2){
                        int q;
                        for(q=0;q<quantidadeConsultores;q++){
                            balancoTotal += calculaRelatorioMensalTotal(consultores[q].arvoreAtendimentos, mes, ano, escopo);
                        }
                         printf("O resultado para %d/%d foi R$ %.2f\n", mes, ano, balancoTotal);
                    }else{
                        printf("Opção de relatório inválida, tente novamente.\n");
                    }
                }else{
                    printf("Informe um mes/ano válidos\n");
                }
                break;
            }
            else if (opcao == 2)
            {
                opcao = 0;
                break;
            }
            opcao = 0;
            break;
        case 3:
            printf("Saindo do menu gerente...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
        pausa();
        limparConsole();
    } while (opcao != 3);
}

int login(Hash *usuarios, Usuario *usuarioLogado)
{
    char username[50], senha[20];
    printf("Digite seu username: ");
    scanf("%s", username);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    Usuario user;
    int buscaUsuario = buscaUsuarioPorUsername(usuarios, username, &user);

    if (buscaUsuario == 0)
    {
        return 0; // Falha no login, usuário não encontrado.
    }
    else
    {
        if (strcmp(user.username, username) == 0 && strcmp(user.senha, senha) == 0)
        {
            *usuarioLogado = user;
            return 1; // Login bem-sucedido.
        }
        return 0; // Senha errada.
    }
}

/**
 * Função: criaHash
 * -----------------------
 * Cria e inicializa uma nova tabela hash para armazenar usuários.
 * Aloca memória para a estrutura Hash e para o array de ponteiros 
 * de usuários. Inicializa a quantidade de usuários para zero e 
 * define todos os ponteiros do array como NULL.
 *
 * @param TABLE_SIZE: o tamanho da tabela hash a ser criada.
 * @return: um ponteiro para a nova estrutura Hash, ou NULL se a alocação falhar.
 */
Hash *criaHash(int TABLE_SIZE)
{
    Hash *ha = (Hash *)malloc(sizeof(Hash));
    if (ha != NULL)
    {
        int i;
        ha->TAM_TAB = TABLE_SIZE;
        ha->usuarios = (Usuario **)malloc(TABLE_SIZE * sizeof(Usuario *));
        if (ha->usuarios == NULL)
        {
            free(ha);
            return NULL;
        }
        ha->quantidade = 0;
        for (i = 0; i < ha->TAM_TAB; i++)
            ha->usuarios[i] = NULL;
    }
    return ha;
}

/**
 * Função: chaveDivisao
 * --------------------
 * Realiza o cálculo do índice para inserção em uma tabela hash usando o método da divisão.
 * A função aplica um "and" bitwise com 0x7FFFFFFF para garantir um valor não negativo
 * e, em seguida, calcula o módulo da chave com o tamanho da tabela.
 *
 * @param chave: valor inteiro da chave a ser inserida.
 * @param TABLE_SIZE: tamanho da tabela hash.
 * @return: índice calculado para inserção na tabela hash.
 */
int chaveDivisao(int chave, int TABLE_SIZE)
{
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

/**
 * Função: chaveTabelaPorUsername
 * ------------------------------
 * Calcula o valor hash baseado em uma string (como o nome de usuário).
 * Usa uma função de espalhamento simples multiplicando o valor acumulado por 31
 * e somando o valor ASCII de cada caractere da string.
 *
 * @param str: ponteiro para uma string contendo o nome de usuário.
 * @return: valor inteiro representando o hash calculado da string.
 */
int chaveTabelaPorUsername(char *str)
{
    int i, valor = 7;
    int tam = strlen(str);
    for (i = 0; i < tam; i++)
        valor = 31 * valor + (int)str[i];
    return valor;
}

/**
 * Função: inserirUsuario
 * ----------------------
 * Insere um novo usuário em uma tabela hash, verificando se o usuário já existe e
 * gerenciando a alocação de memória e criação da árvore de atendimentos, se necessário.
 * A função utiliza o método da divisão para calcular a posição na tabela e garante
 * que o usuário não seja duplicado.
 *
 * @param ha: ponteiro para a tabela hash onde o usuário será inserido.
 * @param usuario: estrutura do tipo Usuario a ser inserida.
 * @return: 1 se o usuário for inserido com sucesso, 0 em caso de falha ou se o usuário já existir.
 */
int inserirUsuario(Hash *ha, Usuario usuario)
{
    if (ha == NULL || ha->quantidade == ha->TAM_TAB)
        return 0;
    if (usuario.tosador || usuario.veterinario)
    {
        usuario.arvoreAtendimentos = cria_ArvBin();
    }
    int chave = chaveTabelaPorUsername(usuario.username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    if (verificaSeUsuarioExiste(ha, pos, usuario.username))
    {
        return 0;
    }

    Usuario *novo = (Usuario *)malloc(sizeof(Usuario));
    if (novo == NULL)
        return 0;

    *novo = usuario;
    novo->prox = ha->usuarios[pos];
    ha->usuarios[pos] = novo;
    ha->quantidade++;
    return 1;
}

/**
 * Função: buscaUsuarioPorUsername
 * -------------------------------
 * Realiza a busca de um usuário na tabela hash a partir de seu nome de usuário.
 * A função calcula o índice da tabela hash com base no nome e percorre a lista
 * de usuários nessa posição até encontrar uma correspondência.
 *
 * @param ha: ponteiro para a tabela hash onde os usuários estão armazenados.
 * @param nome: ponteiro para uma string contendo o nome de usuário a ser buscado.
 * @param usuario: ponteiro para a estrutura Usuario onde os dados do usuário encontrado serão armazenados.
 * @return: 1 se o usuário for encontrado, 0 em caso de falha ou se o usuário não existir.
 */
int buscaUsuarioPorUsername(Hash *ha, char *nome, Usuario *usuario)
{
    if (ha == NULL)
        return 0;

    int chave = chaveTabelaPorUsername(nome);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    Usuario *atual = ha->usuarios[pos];

    while (atual != NULL && strcmp(atual->username, nome) != 0)
    {
        atual = atual->prox;
    }

    if (atual == NULL)
        return 0;

    *usuario = *atual;
    return 1;
}

/**
 * Função: liberarHash
 * -------------------
 * Libera a memória alocada para a tabela hash e todos os usuários nela armazenados.
 * A função percorre cada posição da tabela, liberando todos os usuários associados
 * a cada índice, e, ao final, libera a própria estrutura da tabela hash.
 *
 * @param ha: ponteiro para a tabela hash a ser liberada.
 */
void liberarHash(Hash *ha)
{
    if (ha != NULL)
    {
        int i;
        for (i = 0; i < ha->TAM_TAB; i++)
        {
            if (ha->usuarios[i] != NULL)
            {
                Usuario *atual;
                while (ha->usuarios[i] != NULL)
                {
                    atual = ha->usuarios[i];
                    ha->usuarios[i] = ha->usuarios[i]->prox;
                    libera_ArvBin(atual->arvoreAtendimentos);
                    free(atual);
                }
            }
        }
        free(ha->usuarios);
        free(ha);
    }
}

/**
 * Função: cadastrarUsuario
 * ------------------------
 * Realiza o cadastro de um novo usuário no sistema. A função solicita dados como email,
 * nome de usuário, senha e cargo, validando se o email ou username já estão cadastrados
 * e determinando o cargo do usuário com base em uma escolha numérica.
 * Após validar e preencher os dados, tenta inserir o usuário na tabela hash.
 *
 * @param ha: ponteiro para a tabela hash onde os usuários são armazenados.
 * @return: 1 se o usuário for cadastrado com sucesso, 0 em caso de erro ou validação falha.
 */
int cadastrarUsuario(Hash *ha)
{
    Usuario novoUsuario;

    printf("=== Cadastro de Usuário ===\n");
    printf("Email: ");
    scanf("%s", novoUsuario.email);

    if (validarEmail(ha, novoUsuario.email))
    {
        printf("Realize o cadastro novamente!\n");
        return 0;
    }

    printf("Username: ");
    scanf("%s", novoUsuario.username);
    Usuario mock;
    if (buscaUsuarioPorUsername(ha, novoUsuario.username, &mock))
    {
        printf("Username já existente na base de dados, realize o cadastro novamente!\n");
        return 0;
    }

    printf("Senha: ");
    scanf("%s", novoUsuario.senha);
    printf("Cargo (1 - Vendedor, 2 - Veterinario, 3 - Tosador, 4 - Gerente): ");
    int cargo;
    scanf("%d", &cargo);

    novoUsuario.vendedor = novoUsuario.veterinario = novoUsuario.tosador = novoUsuario.gerente = 0;

    switch (cargo)
    {
    case 1:
        novoUsuario.vendedor = 1;
        break;
    case 2:
        novoUsuario.veterinario = 1;
        break;
    case 3:
        novoUsuario.tosador = 1;
        break;
    case 4:
        novoUsuario.gerente = 1;
        break;
    default:
        printf("Cargo inválido!\n");
        return 0;
    }

    if (inserirUsuario(ha, novoUsuario))
    {
        printf("Usuário cadastrado com sucesso!\n");
        return 1;
    }
    else
    {
        printf("Erro ao cadastrar usuário!\n");
        return 0;
    }
}

/**
 * Função: validarEmail
 * ---------------------
 * Valida um email fornecido, verificando se ele contém o caractere '@',
 * se seu comprimento é inferior a 80 caracteres e se já existe um cadastro
 * com esse email na tabela hash. A função percorre todas as listas de usuários
 * na tabela para verificar se o email já está em uso.
 *
 * @param ha: ponteiro para a tabela hash onde os usuários estão armazenados.
 * @param email: ponteiro para a string contendo o email a ser validado.
 * @return: 1 se o email for inválido ou já estiver cadastrado, 0 se for válido.
 */
int validarEmail(Hash *ha, char *email)
{

    // Verifica se o email contém o caractere '@'
    char *arroba = strchr(email, '@');
    if (arroba == NULL)
    {
        printf("O email deve ter o caractere @.\n");
        return 1;
    }

    // Verifica se o comprimento do email é menor que 80 caracteres
    if (strlen(email) >= 80)
    {
        printf("O email deve ser menor que 80 caracteres.\n");
        return 1;
    }
    int i;
    for (i = 0; i < ha->TAM_TAB; i++)
    {
        Usuario *atual = ha->usuarios[i]; // Aponta para o primeiro usuário na posição i
        while (atual != NULL)
        {
            if (strcmp(atual->email, email) == 0)
            {
                printf("Já existe um cadastro com este e-mail.\n");
                return 1; // Email já cadastrado
            }
            atual = atual->prox; // Avança para o próximo usuário na lista encadeada
        }
    }

    return 0; // Email não encontrado
}

/**
 * Função: verificaSeUsuarioExiste
 * ---------------------------------
 * Verifica se um usuário com um determinado nome de usuário já existe na tabela hash.
 * A função percorre a lista de usuários na posição especificada e compara os nomes
 * de usuário. Se encontrar uma correspondência, informa que o usuário já existe.
 *
 * @param ha: ponteiro para a tabela hash onde os usuários estão armazenados.
 * @param posicao: índice na tabela hash onde a busca será realizada.
 * @param username: ponteiro para a string contendo o nome de usuário a ser verificado.
 * @return: 1 se o usuário já existir, 0 se não existir.
 */
int verificaSeUsuarioExiste(Hash *ha, int posicao, char *username)
{

    if (ha == NULL)
        return 0; // Tabela hash inválida

    Usuario *atual = ha->usuarios[posicao]; // Aponta para o primeiro usuário na posição
    while (atual != NULL)
    {
        if (strcmp(atual->username, username) == 0)
        {
            printf("Já existe um usuario com o username: %s\n", username);
            printf("Tente novamente com outro username.\n");
            return 1; // Usuário encontrado
        }
        atual = atual->prox; // Avança para o próximo usuário na lista encadeada
    }

    return 0; // Usuário não encontrado
}

/**
 * Função: visualizarTodosUsuarios
 * --------------------------------
 * Exibe a lista de todos os usuários cadastrados na tabela hash.
 * A função percorre cada posição da tabela hash e, para cada usuário
 * encontrado, imprime seu nome de usuário e cargo correspondente.
 *
 * @param usuarios: ponteiro para a tabela hash que contém os usuários a serem exibidos.
 */
void visualizarTodosUsuarios(Hash *usuarios)
{
    int i;
    printf("Lista de Usuários Cadastrados:\n\n");
    for (i = 0; i < usuarios->TAM_TAB; i++)
    {
        Usuario *atual = usuarios->usuarios[i];
        while (atual != NULL)
        {
            printf("%s - ", atual->username);

            if (atual->vendedor)
                printf("Vendedor\n");
            else if (atual->veterinario)
                printf("Veterinário\n");
            else if (atual->tosador)
                printf("Tosador\n");
            else if (atual->gerente)
                printf("Gerente\n");
            else
                printf("Função desconhecida\n");

            atual = atual->prox;
        }
    }
}

/**
 * Função: atualizarUsuario
 * --------------------------
 * Atualiza as informações de um usuário na tabela hash.
 * A função permite que o usuário atualize seu email, username e senha.
 * Se um campo não for desejado para atualização, o usuário deve inserir '0'.
 * Se o novo email for inválido ou o novo username já existir, a função
 * informa o usuário e não realiza a atualização.
 *
 * @param ha: ponteiro para a tabela hash que contém os usuários.
 * @param usuarioAntigo: ponteiro para o usuário cujas informações serão atualizadas.
 * @return: 1 se a atualização for bem-sucedida, 0 em caso de erro.
 */
int atualizarUsuario(Hash *ha, Usuario *usuarioAntigo)
{
    if (ha == NULL)
        return 0; // Tabela hash inválida

    Usuario usuarioAtualizado = *usuarioAntigo;

    printf("=== Atualização de Usuário ===\n");
    printf("Email, 0 se não quiser atualizar: ");
    scanf("%s", usuarioAtualizado.email);

    if (strcmp(usuarioAtualizado.email, "0") == 0)
    {
        strcpy(usuarioAtualizado.email, usuarioAntigo->email);
    }
    else
    {
        if (validarEmail(ha, usuarioAtualizado.email))
        {
            printf("Realize o atulização com um e-mail válido.\n");
            return 0;
        }
    }
    /*
    printf("Username, 0 se não quiser atualizar: ");
    scanf("%s", usuarioAtualizado.username);
    Usuario mock;
    if (strcmp(usuarioAtualizado.username, "0") == 0)
    {
        strcpy(usuarioAtualizado.username, usuarioAntigo->username);
    }
    else
    {
        if (buscaUsuarioPorUsername(ha, usuarioAtualizado.username, &mock) && strcmp(usuarioAtualizado.username, "0") != 0)
        {
            printf("Username já existente na base de dados, realize o cadastro novamente!\n");
            return 0;
        }
    }
    */
    printf("Senha: ");
    scanf("%s", usuarioAtualizado.senha);

    int chave = chaveTabelaPorUsername(usuarioAntigo->username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    Usuario *atual = ha->usuarios[pos]; // Aponta para o primeiro usuário na posição

    // Percorre a lista encadeada de usuários na posição calculada
    while (atual != NULL)
    {
        // Verifica se o usuário foi encontrado pelo username
        if (strcmp(atual->username, usuarioAntigo->username) == 0)
        {
            *atual = usuarioAtualizado;
            return 1; // Cargo atualizado com sucesso
        }
        atual = atual->prox; // Avança para o próximo usuário na lista encadeada
    }
    return 0;
}

/**
 * Função: atualizarCargoDeFuncionario
 * -------------------------------------
 * Atualiza o cargo de um funcionário na tabela hash com base no seu nome de usuário.
 * A função localiza o usuário na tabela e permite que o administrador defina um novo
 * cargo. Os cargos disponíveis são: Vendedor, Veterinário, Tosador e Gerente.
 *
 * @param ha: ponteiro para a tabela hash que contém os usuários.
 * @param username: ponteiro para a string com o nome de usuário do funcionário
 *                  cujo cargo será atualizado.
 * @return: 1 se o cargo foi atualizado com sucesso, 0 se o usuário não foi encontrado
 *          ou se houve um erro.
 */
int atualizarCargoDeFuncionario(Hash *ha, char *username)
{
    if (ha == NULL)
        return 0; // Tabela hash inválida

    // Calcula a chave e a posição na tabela hash com base no username
    int chave = chaveTabelaPorUsername(username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    Usuario *atual = ha->usuarios[pos]; // Aponta para o primeiro usuário na posição

    // Percorre a lista encadeada de usuários na posição calculada
    while (atual != NULL)
    {
        // Verifica se o usuário foi encontrado pelo username
        if (strcmp(atual->username, username) == 0)
        {
            printf("Informe o novo cargo de %s\n", atual->username);
            printf("Cargo (1 - Vendedor, 2 - Veterinario, 3 - Tosador, 4 - Gerente): ");
            int cargo;
            scanf("%d", &cargo);

            atual->vendedor = atual->veterinario = atual->tosador = atual->gerente = 0;

            switch (cargo)
            {
            case 1:
                atual->vendedor = 1;
                break;
            case 2:
                atual->veterinario = 1;
                break;
            case 3:
                atual->tosador = 1;
                break;
            case 4:
                atual->gerente = 1;
                break;
            default:
                printf("Cargo inválido!\n");
                return 0;
            }
            return 1; // Cargo atualizado com sucesso
        }
        atual = atual->prox; // Avança para o próximo usuário na lista encadeada
    }

    return 0; // Usuário não encontrado
}

/**
 * Função: deletarUsuario
 * ------------------------
 * Remove um usuário da tabela hash com base no nome de usuário fornecido.
 * A função verifica se o gerente logado está tentando excluir a si mesmo
 * e se o usuário a ser removido existe na tabela. Se a remoção for confirmada,
 * o usuário é removido da lista encadeada.
 *
 * @param ha: ponteiro para a tabela hash que contém os usuários.
 * @param gerenteLogado: ponteiro para o usuário que está realizando a exclusão,
 *                       para verificar se ele está tentando excluir a si mesmo.
 * @return: 1 se o usuário foi removido com sucesso, 0 se o usuário não foi encontrado
 *          ou se houve uma tentativa de exclusão do próprio gerente.
 */
int deletarUsuario(Hash *ha, Usuario *gerenteLogado)
{
    char username[50];
    printf("Digite o username do usuário que deseja remover: ");
    scanf("%s", username);

    // Checa se o gerente está tentando excluir ele mesmo
    if (strcmp(gerenteLogado->username, username) == 0)
    {
        printf("Você não pode excluir a si mesmo!\n");
        return 0;
    }

    Usuario *anterior = NULL;
    int chave = chaveTabelaPorUsername(username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    Usuario *atual = ha->usuarios[pos];

    // Procura o usuário na lista encadeada
    while (atual != NULL && strcmp(atual->username, username) != 0)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
    {
        printf("Usuário não encontrado!\n");
        return 0;
    }

    // Confirmação de exclusão
    char confirmacao;
    printf("Tem certeza que deseja remover o usuário %s? (s/n): ", username);
    scanf(" %c", &confirmacao);

    if (confirmacao == 's' || confirmacao == 'S')
    {
        // Remove o usuário da lista encadeada
        if (anterior == NULL)
        {
            ha->usuarios[pos] = atual->prox;
        }
        else
        {
            anterior->prox = atual->prox;
        }
        free(atual);
        ha->quantidade--;
        printf("Usuário %s removido com sucesso!\n", username);
        return 1;
    }
    else
    {
        printf("Operação de remoção cancelada.\n");
        return 0;
    }
}

/**
 * Função: deletarUsuarioSimplificado
 * -------------------------------------
 * Remove um usuário da tabela hash, verificando o username e o email.
 * A função percorre a lista encadeada na posição calculada para encontrar
 * o usuário correspondente e, em caso de sucesso, o remove.
 *
 * @param ha: um ponteiro para a tabela hash onde o usuário está armazenado.
 * @param usuario: a estrutura Usuario que contém as informações do usuário a ser removido.
 * @return: 1 se o usuário foi deletado com sucesso, 0 se o usuário não foi encontrado ou a tabela hash for inválida.
 */
int deletarUsuarioSimplificado(Hash *ha, Usuario usuario)
{
    if (ha == NULL)
        return 0; // Tabela hash inválida

    // Calcula a chave e a posição na tabela hash com base no username do usuário
    int chave = chaveTabelaPorUsername(usuario.username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    Usuario *atual = ha->usuarios[pos];
    Usuario *anterior = NULL;

    // Percorre a lista encadeada na posição calculada
    while (atual != NULL)
    {
        // Verifica se o usuário atual é o que deve ser deletado
        if (strcmp(atual->username, usuario.username) == 0 && strcmp(atual->email, usuario.email) == 0)
        {
            if (anterior == NULL)
            {
                // O usuário a ser deletado é o primeiro na lista
                ha->usuarios[pos] = atual->prox;
            }
            else
            {
                // O usuário a ser deletado está no meio ou no final da lista
                anterior->prox = atual->prox;
            }

            free(atual);      // Libera a memória do usuário deletado
            ha->quantidade--; // Diminui a quantidade de usuários na hash
            return 1;         // Usuário deletado com sucesso
        }

        // Avança para o próximo usuário na lista encadeada
        anterior = atual;
        atual = atual->prox;
    }

    return 0; // Usuário não encontrado
}

/**
 * Função: usuariosVeterinariosTosadores
 * ---------------------------------------
 * Busca usuários que são veterinários ou tosadores na tabela hash
 * e os armazena em um array dinâmico. A função aloca espaço para os resultados
 * e aumenta a capacidade conforme necessário. O número total de resultados
 * é retornado através do parâmetro `qtd_resultados`.
 *
 * @param tabela: ponteiro para a tabela hash que contém os usuários.
 * @param qtd_resultados: ponteiro para um inteiro que armazenará a quantidade
 *                        de usuários encontrados (veterinários ou tosadores).
 * @return: ponteiro para um array de `Usuario` contendo os usuários encontrados.
 *          Se nenhum usuário for encontrado, o array estará vazio.
 */
Usuario *usuariosVeterinariosTosadores(Hash *tabela, int *qtd_resultados)
{
    int capacidade = 10;
    *qtd_resultados = 0;
    Usuario *resultados = malloc(capacidade * sizeof(Usuario));

    int i;
    for (i = 0; i < tabela->TAM_TAB; i++)
    {
        Usuario *atual = tabela->usuarios[i];
        while (atual != NULL)
        {
            if (atual->veterinario || atual->tosador)
            {
                if (*qtd_resultados >= capacidade)
                {
                    capacidade *= 2;
                    resultados = realloc(resultados, capacidade * sizeof(Usuario));
                }
                resultados[*qtd_resultados] = *atual; // Copia o usuário para o array
                (*qtd_resultados)++;
            }
            atual = atual->prox;
        }
    }
    return resultados;
}

/**
 * Função: selecionarProfissional
 * -------------------------------
 * Exibe uma lista de profissionais (veterinários ou tosadores) com base 
 * na quantidade fornecida e nas flags indicadoras de cada tipo de profissional.
 * O usuário pode selecionar um profissional a partir da lista exibida.
 *
 * @param usuarios: array de `Usuario` contendo os profissionais disponíveis.
 * @param quantidade: número total de usuários no array.
 * @param tosador: flag (booleano) que indica se os tosadores devem ser incluídos na lista.
 * @param veterinario: flag (booleano) que indica se os veterinários devem ser incluídos na lista.
 * @return: um `Usuario` correspondente à escolha do usuário. Retorna um usuário vazio se a escolha for inválida.
 */
Usuario selecionarProfissional(Usuario *usuarios, int quantidade, int tosador, int veterinario)
{
    printf("Nome dos profissionais: \n");
    int i;
    for (i = 0; i < quantidade; i++)
    {
        if (tosador && usuarios[i].tosador)
        {
            printf("%d - Tosador - %s\n", i + 1, usuarios[i].username);
        }
        if (veterinario && usuarios[i].veterinario)
        {
            printf("%d - Veterinário - %s\n", i + 1, usuarios[i].username);
        }
    }
    int escolha = 0;
    printf("Informe a escolha: ");
    scanf("%d", &escolha);
    // Retorna um usuário vazio se nenhum profissional for encontrado
    return usuarios[escolha - 1];
}

/**
 * Função: cria_ArvBin
 * -----------------------
 * Cria e inicializa uma nova árvore binária para armazenar atendimentos.
 * Aloca memória para a estrutura Atendimentos e inicializa o ponteiro
 * da raiz como NULL.
 *
 * @return: um ponteiro para a nova estrutura Atendimentos, ou NULL se a alocação falhar.
 */
Atendimentos *cria_ArvBin()
{
    Atendimentos *raiz = (Atendimentos *)malloc(sizeof(Atendimentos));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

/**
 * Função: comparaDatas
 * -----------------------
 * Compara duas datas representadas pela estrutura Atendimento.
 * Retorna -1 se a primeira data (a1) é anterior à segunda (a2),
 * 1 se a primeira data é posterior à segunda, e 0 se as datas são iguais.
 *
 * @param a1: a primeira data a ser comparada.
 * @param a2: a segunda data a ser comparada.
 * @return: -1 se a1 é no passado em relação a a2, 
 *          1 se a1 é no futuro em relação a a2, 
 *          0 se as datas são iguais.
 */
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

/**
 * Função: setarDataNoAtendimento
 * -----------------------
 * Define a data atual no atendimento passado como parâmetro.
 * A data é obtida a partir do sistema e é armazenada nas 
 * variáveis dia, mês e ano da estrutura Atendimento.
 *
 * @param atendimento: um ponteiro para a estrutura Atendimento
 *                    onde a data será definida.
 */
void setarDataNoAtendimento(Atendimento *atendimento)
{

    time_t t = time(NULL);              // Obtém o tempo atual
    struct tm *tm_info = localtime(&t); // Converte para a estrutura tm

    atendimento->dia = tm_info->tm_mday;
    atendimento->mes = tm_info->tm_mon + 1;
    atendimento->ano = tm_info->tm_year + 1900;
}

/**
 * Função: insere_ArvBin
 * -----------------------
 * Insere um novo atendimento na árvore binária de atendimentos.
 * Se a raiz da árvore estiver vazia, o novo atendimento se torna 
 * a raiz. Caso contrário, o novo atendimento é comparado com os 
 * existentes para determinar sua posição na árvore. Se já existir 
 * um atendimento com a mesma data, a inserção não é realizada.
 *
 * @param raiz: um ponteiro para a raiz da árvore de atendimentos.
 * @param atendimento: a estrutura Atendimento a ser inserida.
 * @return: 1 se a inserção for bem-sucedida, 0 se não for.
 */
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
                printf("Já existe um atendimento neste dia!\n");
                printf("Realize a alteração ou a remoção para inserir.\n");
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

/**
 * Função: cadastrarAtendimento
 * ------------------------------
 * Cadastra um novo atendimento na árvore binária de atendimentos.
 * Solicita os dados do atendimento ao usuário, incluindo a data e o 
 * profissional responsável. Se o cadastro for bem-sucedido, 
 * confirma a operação ao usuário.
 *
 * @param raiz: um ponteiro para a raiz da árvore de atendimentos.
 * @param usuario: um ponteiro para o usuário responsável pelo atendimento.
 * @return: 1 se o cadastro for bem-sucedido, 0 se não for.
 */
int cadastrarAtendimento(Atendimentos *raiz, Usuario *usuario)
{
    if (raiz == NULL)
    {
        printf("Árvore inválida.\n");
        return 0; // Falha ao cadastrar
    }

    Atendimento novoAtendimento;

    // Solicita os dados do atendimento ao usuário
    printf("=== Cadastro de Atendimento ===\n");
    strcpy(novoAtendimento.usernameProfisional, usuario->username);

    printf("Digite a data do atendimento (dia mes ano): ");
    scanf("%d %d %d", &novoAtendimento.dia, &novoAtendimento.mes, &novoAtendimento.ano);

    // Você pode adicionar mais campos aqui conforme necessário.
    // Exemplo: novoAtendimento.descricao, novoAtendimento.hora, etc.
    novoAtendimento.consultas = cria_FilaPrio();
    // Insere o novo atendimento na árvore binária
    if (insere_ArvBin(raiz, novoAtendimento))
    {
        printf("Atendimento cadastrado com sucesso!\n");
        return 1;
    }
    else
    {
        printf("Erro ao cadastrar o atendimento.\n");
        return 0; // Falha ao cadastrar
    }
}

/**
 * Função: totalNO_ArvBin
 * ------------------------
 * Calcula o total de nós em uma árvore binária de atendimentos.
 * A função percorre recursivamente a árvore, contando os nós
 * nas subárvores esquerda e direita, e retorna a soma total
 * dos nós.
 *
 * @param raiz: um ponteiro para a raiz da árvore de atendimentos.
 * @return: o número total de nós na árvore.
 */
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

/**
 * Função: preordem_ArvBin
 * ------------------------
 * Realiza uma travessia em pré-ordem na árvore binária de atendimentos.
 * Imprime o usuário responsável e a data de cada atendimento no formato 
 * "username - dia/mês/ano". A função visita o nó atual antes de 
 * percorrer as subárvores esquerda e direita.
 *
 * @param raiz: um ponteiro para a raiz da árvore de atendimentos.
 */
void preordem_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return;
    if (*raiz != NULL)
    {
        printf("%s - %d/%d/%d\n", (*raiz)->atendimento.usernameProfisional, (*raiz)->atendimento.dia, (*raiz)->atendimento.mes, (*raiz)->atendimento.ano);
        preordem_ArvBin(&((*raiz)->esq));
        preordem_ArvBin(&((*raiz)->dir));
    }
}

/**
 * Função: libera_NO
 * ------------------
 * Libera a memória ocupada por um nó da árvore binária e suas subárvores.
 * A função é chamada recursivamente para liberar os nós da subárvore esquerda 
 * e da subárvore direita antes de liberar o nó atual.
 *
 * @param no: um ponteiro para o nó da árvore a ser liberado.
 */
void libera_NO(struct NO *no)
{
    if (no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    libera_FilaPrio(no->atendimento.consultas);
    free(no);
    no = NULL;
}

/**
 * Função: libera_ArvBin
 * ----------------------
 * Libera a memória ocupada por uma árvore binária de atendimentos.
 * A função chama libera_NO para liberar todos os nós da árvore e, em seguida,
 * libera a memória da raiz da árvore.
 *
 * @param raiz: um ponteiro para a raiz da árvore a ser liberada.
 */
void libera_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return;

    libera_NO(*raiz); // libera cada nó
    free(raiz);       // libera a raiz
}

/**
 * Função: estaVazia_ArvBin
 * --------------------------
 * Verifica se a árvore binária de atendimentos está vazia.
 * Retorna 1 se a árvore estiver vazia e 0 caso contrário.
 *
 * @param raiz: um ponteiro para a raiz da árvore binária de atendimentos.
 * @return: 1 se a árvore estiver vazia, 0 caso contrário.
 */
int estaVazia_ArvBin(Atendimentos *raiz)
{
    if (raiz == NULL)
        return 1;
    if (*raiz == NULL)
        return 1;
    return 0;
}

/**
 * Função: solicitarDiaDeAtendimento
 * -----------------------------------
 * Solicita ao usuário a data do atendimento e a armazena
 * na estrutura Atendimento fornecida.
 *
 * @param atendimento: um ponteiro para a estrutura Atendimento onde
 *                    a data será armazenada.
 */
void solicitarDiaDeAtendimento(Atendimento *atendimento)
{
    printf("\nDigite a data do atendimento (dia mes ano): ");
    scanf("%d %d %d", &atendimento->dia, &atendimento->mes, &atendimento->ano);
}

/**
 * Função: consulta_ArvBin
 * ------------------------
 * Realiza a busca de um atendimento na árvore binária com base na
 * data fornecida. Se encontrado, atualiza a estrutura Atendimento
 * com os dados do atendimento encontrado.
 *
 * @param raiz: um ponteiro para a raiz da árvore binária de atendimentos.
 * @param atendimento: um ponteiro para a estrutura Atendimento que contém
 *                    a data a ser buscada e será atualizada com os dados
 *                    do atendimento encontrado.
 * @return: 1 se o atendimento for encontrado, 0 caso contrário.
 */
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

/**
 * Função: cria_FilaPrio
 * -----------------------
 * Cria e inicializa uma nova fila de consultas com capacidade para um
 * número máximo de atendimentos. Aloca memória para a estrutura Consultas,
 * inicializa os campos de cada consulta com valores padrão e horários corretos,
 * e insere cada consulta na fila de prioridade.
 *
 * @return: um ponteiro para a nova estrutura Consultas, ou NULL se a alocação falhar.
 */
Consultas *cria_FilaPrio()
{
    Consultas *fp;
    fp = (Consultas *)malloc(sizeof(Consultas));
    if (fp != NULL)
    {
        fp->qtd = 0;
        int i;
        // Preenche o array de consultas com valores padrões e horários corretos
        for (i = 0; i < MAX_ATENDIMENTOS; i++)
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

/**
 * Função: libera_FilaPrio
 * -----------------------
 * Libera a memória alocada para a fila de consultas, incluindo o array de
 * consultas e o array de prioridades, se existirem. A função verifica se a
 * fila é nula antes de tentar liberar a memória.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas a ser liberada.
 */
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

/**
 * Função: insere_FilaPrio
 * -----------------------
 * Insere um novo horário na fila de consultas, verificando se a fila não
 * está cheia. Caso a fila esteja cheia, uma mensagem é exibida e a função
 * retorna um erro. Se a inserção for bem-sucedida, o horário é adicionado
 * ao array de prioridades, e a função valida a fila após a inserção.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas onde o horário
 *                        será inserido.
 * @param horario: o horário a ser inserido na fila de consultas.
 * @return: 1 se a inserção for bem-sucedida, 0 se a fila estiver cheia ou
 *          se a fila for nula.
 */
int insere_FilaPrio(Consultas *fila_Consultas, int horario)
{
    if (fila_Consultas == NULL)
        return 0;
    if (fila_Consultas->qtd == MAX_ATENDIMENTOS)
    {
        printf("\nEste dia de atendimentos está lotado :c\n");
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

/**
 * Função: validarFila
 * -----------------------
 * Realiza a validação da fila de prioridades, garantindo que a propriedade
 * da heap seja mantida. A função compara a prioridade do último elemento
 * inserido com seus pais e, se necessário, realiza trocas para manter
 * a ordem correta, subindo na árvore até a raiz.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila.
 * @param ultimo_Elemento: índice do último elemento inserido na fila.
 */
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

/**
 * Função: marcarConsulta
 * -----------------------
 * Marca uma consulta na fila de atendimentos. A função exibe os horários
 * preenchidos e solicita ao usuário que escolha um horário para agendar.
 * Após verificar se o horário está disponível, coleta os dados da consulta
 * e atualiza o status para "Agendado". Os dados coletados incluem informações
 * do profissional, atendente, tutor, animal, porte, valor, e detalhes da consulta.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 * 
 * @param usernameAtendente: ponteiro para uma cadeia de caracteres com o username
 *                           do atendente
 * 
 * @param usernameProfissional: ponteiro para uma cadeia de caracteres com o username
 *                           do profissional 
 *
 * @return 1 se a consulta foi agendada com sucesso, 0 se houve erro.
 */
int marcarConsulta(Consultas *fila_Consultas, char * usernameAtendente, char * usernameProfissional)
{
    if (fila_Consultas == NULL)
    {
        printf("Fila de consultas não existe.\n");
        return 0;
    }

    // Exibe horários livres
    printf("\nHorários Preenchidos: ");
    int i, horarioEscolhido = -1;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (strcmp(fila_Consultas->consulta[i].status, "Agendado") == 0 || fila_Consultas->consulta[i].concluida == 1)
        {
            printf(" %d ", fila_Consultas->consulta[i].horario);
        }
    }
    // Solicita ao usuário o horário para marcar
    printf("\nInforme o horário que deseja agendar (1 a 8): ");
    scanf("%d", &horarioEscolhido);

    // Verifica se o horário está disponível
    if (horarioEscolhido < 1 || horarioEscolhido > 8 || strcmp(fila_Consultas->consulta[horarioEscolhido - 1].status, "Agendado") == 0 || fila_Consultas->consulta[horarioEscolhido - 1].concluida)
    {
        printf("Horário inválido ou já agendado.\n");
        return 0;
    }

    // Coleta os dados da consulta
    Consulta novaConsulta;
    novaConsulta.horario = horarioEscolhido;
    novaConsulta.concluida = 0; // Inicialmente, a consulta não está concluída

    printf("Nome do profissional: %s\n", usernameProfissional);

    printf("Informe o nome do atendente: %s\n", usernameAtendente);

    printf("Informe o nome do tutor: ");
    scanf(" %[^\n]s", novaConsulta.nomeTutor);

    printf("Informe o telefone de contato: ");
    scanf(" %[^\n]s", novaConsulta.contatoResponsavel);

    printf("Informe o endereço do tutor: ");
    scanf(" %[^\n]s", novaConsulta.enderecoResponsavel);

    printf("Informe o nome do animal: ");
    scanf(" %[^\n]s", novaConsulta.nomeAnimal);

    printf("Informe o porte do animal (1 - Pequeno, 2 - Médio, 3 - Grande): ");
    scanf("%d", &novaConsulta.porteAnimal);

    printf("É uma consulta veterinaria? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.consultaVeterinaria);

    printf("É um banho? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.banho);

    printf("É uma tosa? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.tosa);

    printf("Informe o valor da consulta: ");
    scanf("%f", &novaConsulta.valor);

    printf("Informe os detalhes da consulta: ");
    scanf(" %[^\n]s", novaConsulta.detalhes);

    // Atualiza o status da consulta e insere na fila
    strcpy(novaConsulta.status, "Agendado");
    fila_Consultas->consulta[horarioEscolhido - 1] = novaConsulta;
    printf("Consulta agendada com sucesso no horário %d!\n", horarioEscolhido);

    return 1; // Sucesso
}

/**
 * Função: consulta_FilaPrio
 * ---------------------------
 * Exibe as informações da consulta mais prioritária na fila de atendimentos.
 * A função verifica se a fila de consultas é válida e se há consultas
 * registradas. Em seguida, procura a consulta que corresponde à prioridade
 * mais alta (menor horário) e imprime os detalhes da consulta, incluindo
 * o horário, nome do tutor, nome do animal, status da consulta, se foi
 * concluída, e a prioridade.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 */
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

/**
 * Função: visualizar_FilaPrio
 * -----------------------------
 * Exibe a lista de próximas consultas na fila de atendimentos, mostrando
 * os detalhes de cada consulta, incluindo horário, nome do tutor, nome do
 * animal, status e se a consulta foi concluída. A função verifica se a
 * fila de consultas é válida e imprime uma mensagem de erro se estiver vazia
 * ou não existir.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 */
void visualizar_FilaPrio(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("Fila de consultas está vazia ou no existe.\n");
        return;
    }

    printf("Lista de próximas Consultas (por prioridade de horários):\n");
    int i;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        printf("Consulta %d - Horário: %d, Tutor: %s, Animal: %s, Status: %s, Concluída?: %d\n",
               i + 1,
               fila_Consultas->consulta[i].horario,
               fila_Consultas->consulta[i].nomeTutor,
               fila_Consultas->consulta[i].nomeAnimal,
               fila_Consultas->consulta[i].status,
               fila_Consultas->consulta[i].concluida);
    }
}

/**
 * Função: atualizarConsulta
 * ---------------------------
 * Permite ao usuário atualizar os dados de uma consulta já agendada na fila de atendimentos.
 * Exibe os horários agendados e solicita que o usuário escolha um horário para atualizar.
 * Coleta os novos dados da consulta, como nome do profissional, atendente, tutor, entre outros,
 * e atualiza a consulta na fila. Verifica se o horário selecionado é válido e se a consulta
 * está agendada antes de realizar a atualização.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 *
 * @return: retorna 1 se a atualização foi bem-sucedida e 0 se a fila de
 *          consultas não existe ou se o horário é inválido.
 */
int atualizarConsulta(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("Fila de consultas não existe.\n");
        return 0;
    }

    // Exibe horários livres
    printf("\nHorários agendados: ");
    int i, horarioEscolhido = -1;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (strcmp(fila_Consultas->consulta[i].status, "Agendado") == 0)
        {
            printf(" %d ", fila_Consultas->consulta[i].horario);
        }
    }

    // Solicita ao usuário o horário para marcar
    printf("\nInforme o horário que deseja agendar (1 a 8): ");
    scanf("%d", &horarioEscolhido);

    // Verifica se o horário está disponível
    if (horarioEscolhido < 1 || horarioEscolhido > 8 || strcmp(fila_Consultas->consulta[horarioEscolhido - 1].status, "Agendado") != 0)
    {
        printf("Horário inválido, está livre ou concluído.\n");
        return 0;
    }

    // Coleta os novos dados da consulta
    Consulta novaConsulta;
    novaConsulta.horario = horarioEscolhido;
    novaConsulta.concluida = 0; // Inicialmente, a consulta não está concluída

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

    printf("É uma consulta veterinária? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.consultaVeterinaria);

    printf("É um banho? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.banho);

    printf("É uma tosa? (1 - Sim, 0 - Não): ");
    scanf("%d", &novaConsulta.tosa);

    printf("Informe os detalhes da consulta: ");
    scanf(" %[^\n]s", novaConsulta.detalhes);

    printf("Informe o valor da consulta: ");
    scanf("%f", &novaConsulta.valor);

    // Atualiza o status da consulta e insere na fila
    strcpy(novaConsulta.status, "Agendado");
    fila_Consultas->consulta[horarioEscolhido - 1] = novaConsulta;
    printf("Consulta atualizada com sucesso no horário %d!\n", horarioEscolhido);

    return 1; // Sucesso
}

/**
 * Função: remove_FilaPrio
 * -------------------------
 * Remove a consulta com a maior prioridade da fila de atendimentos
 * e realiza o check-in. A consulta é marcada como concluída, a quantidade
 * de consultas na fila é decrementada, e o elemento mais prioritário
 * é rebaixado na estrutura de dados.
 * 
 * @param fp: ponteiro para a estrutura Consultas que contém os dados
 *            da fila de consultas.
 * 
 * @return: retorna 1 se o check-in foi realizado com sucesso, 
 *          e 0 se a fila está vazia, não existe, ou se ocorreu um erro 
 *          durante o check-in.
 */
int remove_FilaPrio(Consultas *fp)
{
    if (fp == NULL)
        return 0;
    if (fp->qtd == 0)
    {
        printf("Todas as consultas do dia foram realizadas!");
        return 0;
    }

    // Marca a consulta da prioridade atual da lista como concluido.
    if (realizarCheckin(fp))
    {
        fp->qtd--;
        fp->prioridade[0] = fp->prioridade[fp->qtd];
        rebaixarElemento(fp, 0);
        printf("Checkin realizado!\n");
        return 1;
    }
    printf("Erro ao realizar checkin!\n");
    return 0;
}

/**
 * Função: rebaixarElemento
 * -------------------------
 * Rebaixa um elemento na fila de prioridades, garantindo que a propriedade
 * da heap mínima seja mantida. A função troca o elemento pai com o
 * filho de menor prioridade até que a posição do pai esteja correta
 * ou até que o elemento não tenha mais filhos.
 * 
 * @param fp: ponteiro para a estrutura Consultas que contém os dados
 *            da fila de consultas.
 * @param pai: índice do elemento pai que deve ser rebaixado na fila.
 * 
 * @return: Nenhum. A função modifica a estrutura da fila diretamente.
 */
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

/**
 * Função: realizarCheckin
 * -------------------------
 * Realiza o check-in da consulta com o horário de maior prioridade,
 * marcando-a como concluída e atualizando seu status para "Concluída".
 * A função percorre a lista de consultas para encontrar a consulta
 * correspondente ao horário de prioridade mais alta e modifica seu
 * estado.
 * 
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 * 
 * @return: 1 se o check-in for realizado com sucesso; 0 se a fila
 *          de consultas estiver vazia ou nula.
 */
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

/**
 * Função: desmarcarConsulta
 * ---------------------------
 * Desmarca uma consulta agendada, definindo todos os campos
 * da consulta correspondente ao horário escolhido como "Vazio"
 * e atualizando seu status para "Vago". A função exibe
 * os horários agendados e solicita ao usuário que informe o
 * horário que deseja desmarcar.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 *
 * @return: 1 se a consulta for desmarcada com sucesso; 0 se a fila
 *          de consultas estiver vazia ou nula, ou se o horário
 *          informado for inválido.
 */
int desmarcarConsulta(Consultas *fila_Consultas)
{
    if (fila_Consultas == NULL)
    {
        printf("\nFila de consultas não existe.\n");
        return 0;
    }

    // Exibe horários livres
    printf("\nHorários agendados:");
    int i, horarioEscolhido = -1;
    for (i = 0; i < MAX_ATENDIMENTOS; i++)
    {
        if (strcmp(fila_Consultas->consulta[i].status, "Agendado") == 0)
        {
            printf(" %d ", fila_Consultas->consulta[i].horario);
        }
    }

    // Solicita ao usuário o horário para marcar
    printf("\nInforme o horário que deseja desmarcar (1 a 8): ");
    scanf("%d", &horarioEscolhido);

    // Verifica se o horário está disponível
    if (horarioEscolhido < 1 || horarioEscolhido > 8 || strcmp(fila_Consultas->consulta[horarioEscolhido - 1].status, "Agendado") != 0)
    {
        printf("Horário inválido, está livre ou concluído.\n");
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
    printf("Consulta desmarcada com sucesso no horário %d!\n", horarioEscolhido);
    return 1; // Sucesso
}

/**
 * Função: estaCheia_FilaPrio
 * ---------------------------
 * Verifica se a fila de consultas está cheia.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 *
 * @return: -1 se a fila de consultas for nula (erro);
 *          1 se a fila estiver cheia;
 *          0 se a fila não estiver cheia.
 */
int estaCheia_FilaPrio(Consultas *fila_Consultas)
{
    // -1 erro, 1 está cheia, 0 não está cheia.
    if (fila_Consultas == NULL)
    {
        return -1;
    }

    return (fila_Consultas->qtd == MAX_ATENDIMENTOS);
}

/**
 * Função: estaVazia_FilaPrio
 * ---------------------------
 * Verifica se a fila de consultas está vazia.
 *
 * @param fila_Consultas: ponteiro para a estrutura Consultas que contém
 *                        os dados da fila de consultas.
 *
 * @return: -1 se a fila de consultas for nula (erro);
 *          1 se a fila estiver vazia;
 *          0 se a fila contiver elementos.
 */
int estaVazia_FilaPrio(Consultas *fila_Consultas)
{
    // -1 erro, 1 está vazia, 0 possui elementos;
    if (fila_Consultas == NULL)
    {
        return -1;
    }
    return (fila_Consultas->qtd == 0);
}

/**
 * @brief Calcula o valor total das consultas de um determinado mês e ano, de acordo com o escopo especificado.
 *
 * Esta função percorre uma árvore binária de atendimentos, somando os valores das consultas concluídas 
 * que atendem aos critérios de escopo fornecidos:
 * - Escopo 0: Soma geral de todas as consultas concluídas.
 * - Escopo 1: Soma apenas de consultas veterinárias concluídas.
 * - Escopo 2: Soma apenas de consultas de banho ou tosa concluídas.
 *
 * @param raiz Ponteiro para a raiz da árvore de atendimentos.
 * @param mes Mês para o qual o relatório será calculado.
 * @param ano Ano para o qual o relatório será calculado.
 * @param escopo Define o escopo do cálculo (0 = geral, 1 = consultas veterinárias, 2 = banho/tosa).
 * @return A soma dos valores das consultas no mês e ano especificados, de acordo com o escopo.
 */
float calculaRelatorioMensalTotal(Atendimentos *raiz, int mes, int ano, int escopo) {
    if (raiz == NULL || *raiz == NULL)
        return 0.0;

    float somaValores = 0.0;
    // Escopo 0 : Soma geral.
    // Escopo 1 : Consulta Beterinária.
    // Escopo 2 : Consulta Banho/Tosa.

    if ((*raiz)->atendimento.mes == mes && (*raiz)->atendimento.ano == ano) {
        // Verifica as consultas do atendimento no mês e ano solicitados
        int i;
        for (i = 0; i < (*raiz)->atendimento.consultas->qtd; i++) {
            Consulta consulta = (*raiz)->atendimento.consultas->consulta[i];
            // Verifica se a consulta foi concluída
            if ( consulta.concluida == 1 && consulta.consultaVeterinaria == 1 && escopo == 1) {
                somaValores += consulta.valor;
            }else if(consulta.concluida == 1 && (consulta.banho == 1 || consulta.tosa ==1) && escopo == 2){
                somaValores += consulta.valor;
            }
            else if(consulta.concluida == 1 && escopo == 0){
                somaValores += consulta.valor;
            }
        }
    }

    // Soma os valores das subárvores à esquerda e à direita
    somaValores += calculaRelatorioMensalTotal(&((*raiz)->esq), mes, ano, escopo);
    somaValores += calculaRelatorioMensalTotal(&((*raiz)->dir), mes, ano, escopo);

    return somaValores;
}
