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
void menuVendedor(Hash *usuarios, Usuario *logado);
void menuVeterinario(Hash *usuarios, Usuario *logado);
void menuTosador(Hash *usuarios, Usuario *logado);
void menuGerente(Hash *usuarios, Usuario *gerenteLogado);
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

// Função apenas para fins de desenvolvimento, remover antes da entrega.
void exibeUsuarios(Hash *ha)
{
    int i;
    for (i = 0; i < ha->TAM_TAB; i++)
    {
        printf("Posição %d:\n", i);
        Usuario *atual = ha->usuarios[i]; // Pega o primeiro usuário na posição i

        if (atual == NULL)
        {
            printf("  [Vazio]\n");
        }
        else
        {
            // Percorre a lista encadeada de usuários na posição i
            while (atual != NULL)
            {
                printf("  Usuário: %s, Email: %s\n", atual->username, atual->email);
                atual = atual->prox; // Avança para o próximo usuário na lista encadeada
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

    int totalUsuarios = 17;

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
        if (login(Usuarios, &usuarioLogado))
        {
            printf("\nLogin bem-sucedido!\n");

            // Verifica o tipo de usuário e exibe o menu correspondente
            if (usuarioLogado.vendedor)
            {
                menuVendedor(Usuarios, &usuarioLogado);
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

void menuVendedor(Hash *usuarios, Usuario *logado)
{
    int opcao;
    do
    {
        printf("=== Menu Vendedor ===\n");
        printf("1. Realizar Venda\n");
        printf("2. Agendar Serviço\n");
        printf("5. Atualizar minhas informações \n");
        printf("6. Sair\n");
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
        case 5:
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
        case 6:
            printf("Saindo do menu vendedor...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 6);
}

void menuVeterinario(Hash *usuarios, Usuario *logado)
{
    int opcao;
    do
    {
        printf("=== Menu Veterinário ===\n");
        printf("1. Realizar Atendimento\n");
        printf("5. Atualizar minhas informações.\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Realizando atendimento...\n");
            // Lógica para realizar atendimento
            break;
        case 5:
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
        case 6:
            printf("Saindo do menu veterinário...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 6);
}

void menuTosador(Hash *usuarios, Usuario *logado)
{
    int opcao;
    do
    {
        printf("=== Menu Tosador ===\n");
        printf("1. Realizar Tosa\n");
        printf("5. Atualizar minhas informações.\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Realizando tosa...\n");
            // Lógica para realizar tosa
            break;
        case 5:
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
        case 6:
            printf("Saindo do menu tosador...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 6);
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

int validarEmail(Hash *ha, char *email)
{

    // Verifica se o email contém o caractere '@'
    char *arroba = strchr(email, '@');
    if (arroba == NULL)
    {
        printf("E email deve ter o caractere @.\n");
        return 1;
    }

    // Verifica se o comprimento do email é menor que 80 caracteres
    if (strlen(email) >= 80)
    {
        printf("O email deve ser menor que 80 caracteres.\n");
        return 1;
    }

    for (int i = 0; i < ha->TAM_TAB; i++)
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

    printf("Senha: ");
    scanf("%s", usuarioAtualizado.senha);
    if (deletarUsuarioSimplificado(ha, *usuarioAntigo))
    {
        if (inserirUsuario(ha, usuarioAtualizado))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

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