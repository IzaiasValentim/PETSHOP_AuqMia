#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

// Estrutura para armazenar os dados do usuário
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
} Usuario;

typedef struct hash
{
    int quantidade, TAM_TAB;
    Usuario **usuarios;
} Hash;

void boasVindas();
void menuVendedor();
void menuVeterinario();
void menuTosador();
void menuGerente(Hash *usuarios, Usuario *gerenteLogado);
int login(Usuario usuarios[], int totalUsuarios, Usuario *usuarioLogado);

// Funções iniciais do Hash
Hash *criaHash(int TABLE_SIZE);
int chaveDivisao(int chave, int TABLE_SIZE);
int chaveTabelaPorUsername(char *str);
int inserirUsuario(Hash *ha, Usuario usuario);
int buscaUsuarioPorUsername(Hash *ha, char *nome, Usuario *usuario);
void liberarHash(Hash *ha);

// Funções CRUD Usuário:
int cadastrarUsuario(Hash *ha);
int deletarUsuario(Hash *ha, Usuario *gerenteLogado);
void visualizarTodosUsuarios(Hash *usuarios);

// Função apenas para fins de desenvolvimento, remover antes da entrega.
void exibeUsuarios(Hash *ha) {
    for (int i = 0; i < ha->TAM_TAB; i++) {
        printf("Posição %d:\n", i);
        Usuario *atual = ha->usuarios[i];  // Pega o primeiro usuário na posição i
        
        if (atual == NULL) {
            printf("  [Vazio]\n");
        } else {
            // Percorre a lista encadeada de usuários na posição i
            while (atual != NULL) {
                printf("  Usuário: %s, Email: %s\n", atual->username, atual->email);
                atual = atual->prox;  // Avança para o próximo usuário na lista encadeada
            }
        }
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    // Lista de usuários cadastrados (para exemplo)
    Usuario usuarios[] = {
        {"vendedor@petshop.com", "Vendedor", "12345", 1, 0, 0, 0, NULL},
        {"veterinario@petshop.com", "Veterinario", "12345", 0, 1, 0, 0, NULL},
        {"tosador@petshop.com", "Tosador", "12345", 0, 0, 1, 0, NULL},
        {"izaias@petshop.com", "Luis Izaias", "12345", 0, 0, 1, 0, NULL},
        {"izaias2@petshop.com", "Izaias Luis", "12345", 0, 0, 1, 0, NULL},
        {"Tulio@petshop.com", "Tulio", "12345", 0, 0, 1, 0, NULL},
        {"tosador@petshop.com", "Pablo", "12345", 0, 0, 1, 0, NULL},
        {"gerente@petshop.com", "Gerente", "12345", 0, 0, 0, 1, NULL}};
        
    int totalUsuarios = 11;

    Hash *Usuarios = criaHash(totalUsuarios);
    inserirUsuario(Usuarios, usuarios[0]);
    inserirUsuario(Usuarios, usuarios[1]);
    inserirUsuario(Usuarios, usuarios[2]);
    inserirUsuario(Usuarios, usuarios[3]);
    inserirUsuario(Usuarios, usuarios[4]);
    inserirUsuario(Usuarios, usuarios[5]);
    inserirUsuario(Usuarios, usuarios[6]);
    inserirUsuario(Usuarios, usuarios[7]);

    Usuario usuarioLogado;

    int continuar = 1; // Variável para controlar o loop do sistema

    // Loop principal do sistema
    while (continuar)
    {
        boasVindas();
        // Processo de login
        if (login(usuarios, totalUsuarios, &usuarioLogado))
        {
            printf("\nLogin bem-sucedido!\n");

            // Verifica o tipo de usuário e exibe o menu correspondente
            if (usuarioLogado.vendedor)
            {
                menuVendedor();
            }
            else if (usuarioLogado.veterinario)
            {
                menuVeterinario();
            }
            else if (usuarioLogado.tosador)
            {
                menuTosador();
            }
            else if (usuarioLogado.gerente)
            {
                menuGerente(Usuarios, &usuarioLogado);
            }
            else
            {
                printf("Usuario invalido, confira suas credenciais.\n");
            }
        }
        else
        {
            printf("\nEmail ou senha incorretos!\n");
        }

        // Pergunta se o usuário deseja continuar ou sair do sistema
        printf("\nDeseja continuar no sistema? (1 - Sim / 0 - Nao): ");
        scanf("%d", &continuar);
    }

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

void menuVendedor()
{
    int opcao;
    do
    {
        printf("=== Menu Vendedor ===\n");
        printf("1. Realizar Venda\n");
        printf("2. Agendar Serviço\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Realizando venda...\n");
            // Lógica para realizar venda
            break;
        case 2:
            printf("Agendando serviço...\n");
            // Lógica para agendar serviço
            break;
        case 3:
            printf("Saindo do menu vendedor...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 3);
}

void menuVeterinario()
{
    int opcao;
    do
    {
        printf("=== Menu Veterinário ===\n");
        printf("1. Realizar Atendimento\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Realizando atendimento...\n");
            // Lógica para realizar atendimento
            break;
        case 2:
            printf("Saindo do menu veterinário...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 2);
}

void menuTosador()
{
    int opcao;
    do
    {
        printf("=== Menu Tosador ===\n");
        printf("1. Realizar Tosa\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Realizando tosa...\n");
            // Lógica para realizar tosa
            break;
        case 2:
            printf("Saindo do menu tosador...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 2);
}

void menuGerente(Hash *usuarios, Usuario *gerenteLogado)
{
    int opcao;
    do
    {
        printf("=== Menu Gerente ===\n");
        printf("1. Gerenciar Funcionários\n");
        printf("2. Ver Relatórios\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Gerenciando funcionários...\n");
            opcao = 0;
            printf("-> Gerenciar Funcionários\n");
            printf("1. Cadastrar Usuário\n");
            printf("2. Deletar Usuário\n");
            printf("3. Visualizar Usuários\n");
            printf("5. voltar\n");
            scanf("%d", &opcao);
            if (opcao == 1) {
                cadastrarUsuario(usuarios);
            } else if (opcao == 2) {
                deletarUsuario(usuarios, gerenteLogado);
                opcao=0;
                break;
            } else if(opcao == 3){
                visualizarTodosUsuarios(usuarios);
            }else if (opcao == 5) {
                opcao = 0;
                break;
            }
            opcao = 0;
            break;
        case 2:
            printf("Visualizando relatórios...\n");
            // Lógica para ver relatórios
            break;
        case 3:
            printf("Saindo do menu gerente...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 3);
}

// Função de login
int login(Usuario usuarios[], int totalUsuarios, Usuario *usuarioLogado)
{
    char email[50], senha[20];

    printf("Email: ");
    scanf("%s", email);
    printf("Senha: ");
    scanf("%s", senha);

    for (int i = 0; i < totalUsuarios; i++)
    {
        if (strcmp(usuarios[i].email, email) == 0 && strcmp(usuarios[i].senha, senha) == 0)
        {
            *usuarioLogado = usuarios[i]; // Preenche o usuário logado
            return 1;                     // Login bem-sucedido
        }
    }
    return 0; // Falha no login
}

// Implementação de hash para usuários
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

int chaveDivisao(int chave, int TABLE_SIZE)
{
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

int chaveTabelaPorUsername(char *str)
{
    int i, valor = 7;
    int tam = strlen(str);
    for (i = 0; i < tam; i++)
        valor = 31 * valor + (int)str[i];
    return valor;
}

int inserirUsuario(Hash *ha, Usuario usuario)
{
    if (ha == NULL || ha->quantidade == ha->TAM_TAB)
        return 0;

    int chave = chaveTabelaPorUsername(usuario.username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);

    Usuario *novo = (Usuario *)malloc(sizeof(Usuario));
    if (novo == NULL)
        return 0;

    *novo = usuario;
    novo->prox = ha->usuarios[pos];
    ha->usuarios[pos] = novo;
    ha->quantidade++;
    return 1;
}

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
                    free(atual);
                }
            }
        }
        free(ha->usuarios);
        free(ha);
    }
}

int cadastrarUsuario(Hash *ha)
{
    Usuario novoUsuario;

    printf("=== Cadastro de Usuário ===\n");
    printf("Email: ");
    scanf("%s", novoUsuario.email);
    printf("Username: ");
    scanf("%s", novoUsuario.username);
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