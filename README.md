# <div align="center">🐾 Sistema de PetShop AuqMia 🐾</div>
                                                へ   AuqMia System  ╱|       
                                            ૮ -  ՛)        ♡      (` - 7    
                                            /  ⁻ ៸|                |、⁻〵     
                                         乀(_,ل ل   --^_--^-       じし_,)ノ  
                                        --^___--^--          _^--^--_--__^^--
## *AuqMia foi desenvolvido em linguagem C, como projeto avaliativo para a disciplina de Laboratório de Algoritmos e Estrutura de Dados II, sob orientação do docente Kennedy Reurison Lopes.*
### 1. Introdução:
   - *Considerando o escopo da terceira entrega.*
   <p>&emsp;O projeto consiste no desenvolvimento de um sistema de gerenciamento para o Pet Shop AuqMia, implementado em linguagem C. O objetivo é criar um sistema comercial que permita o login e a gestão de usuários, além de disponibilizar agendamentos para serviços de banho e tosa ou atendimento veterinário. Atualmente, o sistema está na fase da terceira entrega, onde foram aplicadas três estruturas de dados para otimizar funcionalidades específicas: a Tabela Hash, a Árvore Binária de Busca e a Heap Binária. A seguir serão descritas algumas das motivações para a escolha de tais estruturas de dados:</p>
   <p>&emsp;A Tabela Hash foi escolhida para gerenciar os usuários, proporcionando uma busca eficiente pelo nome de usuário ('username'), o que agiliza operações como o login. A Árvore Binária de Busca foi implementada para organizar os plantões de atendimento, permitindo uma busca rápida e eficiente por atendimentos com base na data. A inserção de novos atendimentos nessa estrutura mantém a ordem cronológica, oferecendo flexibilidade para futuras expansões, como a estruturação das consultas. Por fim, a Heap Binária é utilizada para garantir que as consultas sejam realizadas na ordem correta de prioridade, com os horários mais próximos (valores menores) recebendo maior prioridade.</p>

### 2. O quê o sistema AuqMia oferece:
   - #### Manejo de usuários e login:
     - No sistema o controle dos usuários é implementado por meio da Tabela Hash. A seguir é possível conferir o protótipo das funções desta estrutura:
       ```
       - Hash *criaHash(int TABLE_SIZE);
       - int chaveDivisao(int chave, int TABLE_SIZE);
       - int chaveTabelaPorUsername(char *str);
       - int inserirUsuario(Hash *ha, Usuario usuario);
       - int buscaUsuarioPorUsername(Hash *ha, char *nome, Usuario *usuario);
       - void liberarHash(Hash *ha);
       ```
     - Um usuário cadastrado poderá realizar o login com suas credênciais: 'username' e senha. O seu 'username' é um identificador único que gera uma posição na tabela Hash.
     - O cadastro do usuário é realizada pelo gerente, em seu menu específico que logo será exposto. 
> [!IMPORTANT]
> Em `main.c`, na definição de cada função, há comentários informativos a respeito do seu funcionamento, parâmetros e retorno.

  - #### Menu do Gerente e suas atribuições:
      - Após o login, se o usuário possuir o identificador de gerente, ele terá acesso ao menu da gerência que tem como principal função a gestão dos funcionários.
      - Funções de gestão: *Cadastrar Usuário, Deletar Usuário, Visualizar Usuários, Atualizar minhas informações, Atualizar cargo de um funcionário*.
      - Todas as funções anteriores utilizam diretamente ou indiretamente a tabela Hash para o seu funcionamento e possuem os protótipos em ordem:
        ```
        - int cadastrarUsuario(Hash *ha);
        - int validarEmail(Hash *ha, char *email);
        - int verificaSeUsuarioExiste(Hash *ha, int posicao, char *username);
        - int deletarUsuario(Hash *ha, Usuario *gerenteLogado);
        - int deletarUsuarioSimplificado(Hash *ha, Usuario usuario);
        - void visualizarTodosUsuarios(Hash *usuarios);
        - int atualizarUsuario(Hash *ha, Usuario *usuarioAntigo);
        - int atualizarCargoDeFuncionario(Hash *ha, char *username); 
        ```
- #### Menu do Veterinário, Banho/Tosa e suas atribuições:
  - Essas duas sessões serão apresentadas juntas pois possuem as mesmas funções e diferem apenas pelo cargo no negócio.
  - Nesses tipos de sessões, o usuário terá acesso ao Menu Veterinário ou Tosador a depender do tipo de usuário.
  - Eles poderão: Cadastrar dia de atendimento, Realizar checkin(Da próxima consulta do dia presente, caso haja), Visualizar atendimentos, Visualizar consultas do dia, Consultar consultas por dia, Atualizar minhas informações e Sair.
  - Os plantões de atendimentos são implementados por meio da estrutura de dados Árvore Binária de Busca e esses são os seus protótipos de funções:
    ```
    - Atendimentos *cria_ArvBin();
    - int comparaDatas(Atendimento a1, Atendimento a2);
    - void setarDataNoAtendimento(Atendimento *atendimento);
    - int insere_ArvBin(Atendimentos *raiz, Atendimento atendimento);
    - int cadastrarAtendimento(Atendimentos *raiz, Usuario *usuario);
    - void preordem_ArvBin(Atendimentos *raiz);
    - int totalNO_ArvBin(Atendimentos *raiz);
    - void libera_ArvBin(Atendimentos *raiz);
    - int estaVazia_ArvBin(Atendimentos *raiz);
    - int consulta_ArvBin(Atendimentos *raiz, Atendimento *atendimento);
    - void solicitarDiaDeAtendimento(Atendimento *atendimento);
    - int remove_ArvBin(Atendimentos *raiz, int valor);
    ```
  - Apesar de não possuir funções claras de atualização(update), cabe salientar que um nó da árvore de atendimentos conta com as informações das consultas, logo a atualização de um atendimento ocorre em conjunto com a próxima estrutura apresentada. 
- #### Menu do Atendente e suas atribuições:
  - Se a sessão do usuário for do tipo Atendente, o usuário terá acesso ao menu de atendimento. Este menu possui as opções: Atendimento veterinário, Banho/Tosa, Atualizar minhas informações e Sair que encerra a sessão.
  - Se a escolha for Atendimento veterinário, o atendente poderá: Horarios disponíveis(De acordo com o profissional e dia informado), Marcar atendimento, Desmarcar atendimento, Alterar atendimento.
  - Se a escolha for Banho/Tosa, o atendente poderá: Consultar os Horarios disponíveis(De acordo com o profissional e dia informado), Marcar atendimento, Desmarcar atendimento, Alterar atendimento.
  - "Por trás dos panos" as funcionalidades acerca do CRUD de um atendimento( Realizar uma consulta veterinária ou atendimento de banho ou tosa) são realizadas em conjunto com a estrutura de dados anterior, uma vez que a fila de consultas é contida em um nó da árvore de atendimentos.
  - A fila de consultas é uma fila de prioridade implementada em Heap Binária e possui tais protótipos:
     ```
    - Consultas *cria_FilaPrio(); 
    - void libera_FilaPrio(Consultas *fila_Consultas);
    - int insere_FilaPrio(Consultas *fila_Consultas, int horario);      
    - void validarFila(Consultas *fila_Consultas, int ultimo_Elemento); 
    - int marcarConsulta(Consultas *fila_Consultas);
    - void consulta_FilaPrio(Consultas *fila_Consultas);  
    - void visualizar_FilaPrio(Consultas *fila_Consultas); 
    - int atualizarConsulta(Consultas *fila_Consultas);
    - int remove_FilaPrio(Consultas *fila_Consultas);            
    - void rebaixarElemento(Consultas *fila_Consultas, int pai); 
    - int realizarCheckin(Consultas *fila_Consultas);            
    - int desmarcarConsulta(Consultas *fila_Consultas);
    - int estaCheia_FilaPrio(Consultas *fila_Consultas); 
    - int estaVazia_FilaPrio(Consultas *fila_Consultas); 
    ```
  - Como dito anteriormente, há uma relação de dependência entre a estrutura dos Atendimentos e Consultas. Por exemplo, para cadastrar ou alterar uma consulta é necessário primeiro possuir um dia de antendimento cadastrado. Por meio da consulta a árvore, o nó(Dia de atendimento) poderá ser atualizada com a fila de consulta com um cadastro ou atualização, utilizando alguma das funções logo acima.
### 3. Execução:
   - [x] Primeiro clone o repositório em seu computador, no terminal ou Git Bash, execute o comando:
   - > git clone https://github.com/IzaiasValentim/PETSHOP_AuqMia.git
  - [x] Execução, na pasta raiz do projeto: 
   - > Compilador utilizado: MinGW.org GCC-6.3.0-1
   - > Compilação do arquivo .c: gcc main.c -o main.exe
   - > Execução: ./main.exe
### 4.  Considerações:
   - *Na pasta "Utils", ./Utils, foram adicionadas os modelos base de estruturas de dados as quais estão sendo ou foram implementadas. No arquivo há um comentário com a fonte dos códigos.*
     - [Tabela Hash](https://github.com/IzaiasValentim/PETSHOP_AuqMia/blob/dev/Utils/Estrutura_Base_Hash/ModeloHash.c)  [Árvore Binária de Busca](https://github.com/IzaiasValentim/PETSHOP_AuqMia/blob/dev/Utils/Estrutura_Base_ArvoreB/Estrutura_Base_ArvoreB.c)  [Heap Binária](https://github.com/IzaiasValentim/PETSHOP_AuqMia/blob/dev/Utils/Estrutura_Base_Heap/Estrutura_Base_Heap.c)
   - *Ainda na pasta citada anteriormente, foi adicionado um diagrama com a modelagem do nosso sistema como meio auxiliar de desenvolvimento e para documentação.*
        - [Diagrama AuqMia](https://github.com/IzaiasValentim/PETSHOP_AuqMia/blob/dev/Utils/Diagrama_Do_Sistema/AUQMIA-DIAGRAMA-BASE.png)
   - *Foi adotada a funcionalidade "Issues" do GitHub para cadastro e acompanhamento das demandas. Logo, na aba "Issues" é possível acompanhar detalhadamente a descrição dos problemas que surgiram, sua resolução e atribuição aos integrantes. Em resumo, até o momento surgiram tarefas de atualização de documentação, correção de compatibilidade de código aos diferentes compiladores dos integrantes, atualização de código e implementação de novas funcionalidades.*
      - [Issues](https://github.com/IzaiasValentim/PETSHOP_AuqMia/issues)
   - *No momento apenas trabalhamos com a compatibilidade de execução em ambiente Windows, visto algumas funções como Sleep e uso de caracteres especiais.*
### 5. Equipe e Papéis:
    - LUIS IZAÍAS VALENTIM DE AQUINO (Integrador Git/GitHub)
    - PABLO GERALDO LINHARES DO NASCIMENTO LEITE (Testador)
    - STHEFFANY DA CÂMARA GUILHERMINO (Redatora)
    - TÚLIO GOMES DE ARAÚJO FEITOSA (Codificador)
   
