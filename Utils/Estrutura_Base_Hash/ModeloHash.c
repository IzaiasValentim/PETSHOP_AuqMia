#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    ** Info:
            A Tabela Hash foi implementada seguindo os códigos
            disponibilizados no capítulo 10(dez) do livro: 
            
            BACKES, André R. Algoritmos e Estruturas de Dados em Linguagem C.
            Rio de Janeiro: LTC, 2023. E-book. ISBN 9788521638315.
            Disponível em: https://app.minhabiblioteca.com.br/#/books/9788521638315/.
            Acesso em: 22 set. 2024.

            Foram realizadas pequenas alterações para que a estrutura de dados lidasse com o nosso tipo de dado.
*/

typedef struct {
    char email[50];
    char username[50];
    char senha[20];
    int vendedor;
    int veterinario;
    int tosador;
    int gerente;
} Usuario;

typedef struct hash
{
    int quantidade, TAM_TAB;
    Usuario **usuarios;
} Hash;

int chaveDivisao (int chave, int TABLE_SIZE) {
return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

Hash *criaHash(int TABLE_SIZE)
{
    Hash *ha = (Hash *)malloc(sizeof(Hash));
    if (ha != NULL)
    {
        int i;
        ha->TAM_TAB = TABLE_SIZE;
        ha->usuarios = (Usuario **)malloc(
            TABLE_SIZE * sizeof(Usuario *));
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

int chaveTabelaPeloNome(char *str)
{
    int i, valor = 7;
    int tam = strlen(str);
    for (i = 0; i < tam; i++)
        valor = 31 * valor + (int)str[i];
    
    return valor;
}

int insereHash_SemColisao(Hash *ha, Usuario al)
{
    if (ha == NULL || ha->quantidade == ha->TAM_TAB)
        return 0;

    int chave = chaveTabelaPeloNome(al.username);
    int pos = chaveDivisao(chave, ha->TAM_TAB);
    Usuario * novo;
    novo = (Usuario *)malloc(sizeof(Usuario));
    if (novo == NULL)
        return 0;
    *novo = al;
    ha->usuarios[pos] = novo;
    ha->quantidade++;
    return 1;
}



int buscaHash(Hash* ha, char * nome, Usuario *al){
    if(ha == NULL){
        return 0;
    }
    int chave = chaveTabelaPeloNome(nome);
    int pos = chaveDivisao (chave,ha->TAM_TAB);
    if(ha->usuarios[pos] == NULL){
        return 0;
    }
    *al = *(ha->usuarios[pos]);
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
                free(ha->usuarios[i]);
            }
        }
        free(ha->usuarios);
        free(ha);
    }
}

int main()
{
     Usuario usuarios[] = {
        {"vendedor@petshop.com","Izaias","12345", 1, 0, 0, 0},
        {"veterinario@petshop.com","Pablo","12345", 0, 1, 0, 0},
        {"tosador@petshop.com","Stph","12345", 0, 0, 1, 0},
        {"gerente@petshop.com","Tulio","12345", 0, 0, 0, 1}
    };

    Hash * Usuarios = criaHash(4);
    insereHash_SemColisao(Usuarios, usuarios[0]);
    insereHash_SemColisao(Usuarios, usuarios[1]);
    insereHash_SemColisao(Usuarios, usuarios[2]);
    // Usuario busca;
    // buscaHash(Usuarios, "Izaias", &busca);
    // printf("%s", busca.email);
   
    printf("%d", Usuarios->quantidade);
    
    return 0;
}