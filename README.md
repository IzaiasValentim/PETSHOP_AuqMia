## <div align="center">🐾 Sistema de PetShop AuqMia 🐾</div>

### *AuqMia foi desenvolvido em linguagem C, como projeto avaliativo para a disciplina de Laboratório de Algoritmos e Estrutura de Dados II, sob orientação do docente Kennedy Reurison Lopes.*
1. Resumo:
   - *Considerando o escopo da segunda entrega.*
   - *O projeto consiste no desenvolvimento de um sistema de gerenciamento para
um Pet Shop chamado AuqMia, implementado em linguagem C. O objetivo é permitir a atividade do Pet Shop com um sistema comercial que permitirá o login e gerência dos usuários, disponibilização de consultas para banho e tosa ou atendimento veterinário em geral. Atualmente o sistema está no escopo da segunda entrega, onde foi aplicada a estrutura de dados Tabela Hash para a organização dos usuários e otimização das consulta pelo seu 'username'. Também encontram-se implementadas as funções de criação, atualização e remoção de usuário. Abaixo, segue alguns protótipos das funções de CRUD implementadas que são acessadas diretamente ou indiretamente pelos menus dos usuários.*

      Tabela Hash, funções de CRUD:
      -  (create) int cadastrarUsuario(Hash *ha);
      -  (read)   void visualizarTodosUsuarios(Hash *usuarios);
      -  (read)   int buscaUsuarioPorUsername(Hash *ha, char *nome, Usuario *usuario);
      -  (delet)  int deletarUsuario(Hash *ha, Usuario *gerenteLogado);
      -  (update) int atualizarUsuario(Hash *ha, Usuario *usuarioAntigo);
      -  (update) int atualizarCargoDeFuncionario(Hash *ha, char *username);
2. Execução, na pasta raiz do projeto:
   - > Compilador utilizado: MinGW.org GCC-6.3.0-1
   - > Compilação do arquivo .c: gcc main.c -o main.exe
   - > Execução: .\\'main.exe'
4. Considerações:
   - *Foi adotada a funcionalidade "Issues" do GitHub para cadastro e acompanhamento das demandas. Logo, na aba "Issues" é possível acompanhar detalhadamente a descrição dos problemas que surgiram, sua resolução e atribuição aos integrantes. Em resumo, até o momento surgiram tarefas de atualização de documentação, correção de compatibilidade de código aos diferentes compiladores dos integrantes, atualização de código e implementação de novas funcionalidades.*
   - *No momento apenas trabalhamos com a compatibilidade de execução em ambiente Windows, visto algumas funções como Sleep e uso de caracteres especiais.*
6. Equipe e Papéis:
    - LUIS IZAÍAS VALENTIM DE AQUINO (Integrador Git/GitHub)
    - PABLO GERALDO LINHARES DO NASCIMENTO LEITE (Testador)
    - STHEFFANY DA CÂMARA GUILHERMINO (Redatora)
    - TÚLIO GOMES DE ARAÚJO FEITOSA (Codificador)
   
