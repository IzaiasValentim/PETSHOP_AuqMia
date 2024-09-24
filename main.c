#include <stdio.h>
#include <string.h>
#include <windows.h>

// Estrutura para armazenar os dados do usuário
typedef struct
{
    char email[50];
    char username[50];
    char senha[20];
    int vendedor;
    int veterinario;
    int tosador;
    int gerente;
} Usuario;

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

// Funções para exibir os menus específicos de cada tipo de usuário
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

void menuGerente()
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
            // Lógica para gerenciar funcionários
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

// Função para realizar login
int login(Usuario usuarios[], int totalUsuarios, Usuario *usuarioLogado)
{
    char email[50], senha[20];
    printf("Digite seu email: ");
    scanf("%s", email);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    // Verifica se o usuário está na lista de usuários
    for (int i = 0; i < totalUsuarios; i++)
    {
        if (strcmp(usuarios[i].email, email) == 0 && strcmp(usuarios[i].senha, senha) == 0)
        {
            *usuarioLogado = usuarios[i];
            return 1; // Login bem-sucedido
        }
    }
    return 0; // Falha no login
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    // Lista de usuários cadastrados (para exemplo)
    Usuario usuarios[] = {
        {"vendedor@petshop.com", "Vendedor", "12345", 1, 0, 0, 0},
        {"veterinario@petshop.com", "Veterinario", "12345", 0, 1, 0, 0},
        {"tosador@petshop.com", "Tosador", "12345", 0, 0, 1, 0},
        {"gerente@petshop.com", "Gerente", "12345", 0, 0, 0, 1}};
    int totalUsuarios = 4;
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
                menuGerente();
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
