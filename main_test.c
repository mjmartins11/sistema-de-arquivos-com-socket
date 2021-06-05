#include "lista.h"

int main(void){
    boolean in_code = true;
    int option = -1;
    Lista *l = lista_criar();

    while (in_code){
        char nome_do_cliente[TAMANHO_TEXTO];
        char titulo[TAMANHO_TEXTO];
        char conteudo[TAMANHO_CONTEUDO];
        char titulo_aux[TAMANHO_TEXTO];
        
        printf("************* MENU *************\n");
        printf("Escolha a opcao digitando o numero correspondente a ela\n");
        printf("Opcao 0 - inserir documento\n");
        printf("Opcao 1 - imprimir todos os documentos\n");
        printf("Opcao 2 - remover documento\n");
        printf("Opcao 3 - buscar documento\n");
        printf("Opcao 4 - sair\n");
        scanf("%d", &option);

        switch(option){
            case 0:        
                printf("Qual o seu nome?\n");
                scanf(" %[^\n]s", nome_do_cliente);
                nome_do_cliente[strlen(nome_do_cliente)] = '\0';
                //printf("nome: %s", nome_do_cliente);
                printf("Insira o titulo do documento\n");
                scanf(" %[^\n]s", titulo);
                titulo[strlen(titulo)] = '\0';
                //printf("titulo: %s", titulo);
                printf("Insira o conteudo do documento\n");
                scanf(" %[^\n]s", conteudo);
                conteudo[strlen(conteudo)] = '\0';
                //printf("conteudo: %s", conteudo);
                lista_inserir(l, nome_do_cliente, titulo, conteudo);
                break;

            case 1:
                lista_imprimir(l);
                break;

            case 2:
                printf("Qual o titulo a ser removido?\n");
                scanf(" %[^\n]s", titulo_aux);
                titulo_aux[strlen(titulo_aux)] = '\0';
                lista_remover_documento(l, titulo_aux);
                break;

            case 3: 
                printf("Qual o titulo desejado?\n");
                scanf(" %[^\n]s", titulo_aux);
                titulo_aux[strlen(titulo_aux)] = '\0';
                lista_buscar_e_imprimir(l, titulo_aux);
                break;

            case 4:
                lista_sair(l);
                in_code = false;
                break;
        }
    }
    
    return 0;
}