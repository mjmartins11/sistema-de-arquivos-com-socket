#include "lista.h"

typedef struct arquivo_ {
    char nome_do_cliente[TAMANHO_TEXTO];
    char titulo[TAMANHO_TEXTO];
    char conteudo[TAMANHO_CONTEUDO];
} Arquivo;

typedef struct no_ {
    Arquivo *a;
    struct no_ *proximo_no;
}No;

struct lista_ {
    No *inicio;
    No *fim;
    int tamanho;
};

Lista *lista_criar() {
    Lista *l = (Lista *) malloc(sizeof(Lista));
    if (l != NULL) {
        l->inicio = NULL;
        l->fim = l->inicio;
        l->tamanho = 0;
    }
    return l;
}

No* lista_buscar(Lista *l, char titulo[TAMANHO_TEXTO]) {
    if (!lista_vazia(l)) {
        No *aux = l->inicio;
        while (aux != NULL && strcmp(aux->a->titulo, titulo)) {
            printf("titulo atual:%s\n", aux->a->titulo);
            aux = aux->proximo_no;
        }

        if (aux != NULL)
            return aux;
    }
    return NULL;
}

void lista_inserir(Lista *l, char nome_do_cliente[TAMANHO_TEXTO], char titulo[TAMANHO_TEXTO], char conteudo[TAMANHO_CONTEUDO], int socket_cliente) {
    char mensagem[TAMANHO_CONTEUDO];
    printf("\n");
    if (l != NULL) {

        if (lista_buscar(l, titulo) != NULL) {
            printf("O documento ja existe.\n");
            strcpy(mensagem, "O documento ja existe.");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
            return;
        }

        No *a_novo = (No*) malloc(sizeof(No));
        if (l->inicio == NULL)
            l->inicio = a_novo;
        else
            l->fim->proximo_no = a_novo;

        a_novo->a = (Arquivo*) malloc(sizeof(Arquivo));

        strcpy(a_novo->a->nome_do_cliente, nome_do_cliente);
        strcpy(a_novo->a->titulo, titulo);
        strcpy(a_novo->a->conteudo, conteudo);

        if (a_novo->a != NULL) {
            a_novo->proximo_no = NULL;
            l->fim = a_novo;
            l->tamanho++;
            printf("Documento inserido com sucesso.");
            strcpy(mensagem, "Documento inserido com sucesso.");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
        } else {
            printf("Falha ao inserir documento.");
            strcpy(mensagem, "Falha ao inserir documento.");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
        }
    }
    printf("\n");
}

void lista_imprimir(Lista *l, int socket_cliente) {
    char mensagem[TAMANHO_CONTEUDO + (2*TAMANHO_TEXTO)];
    if (!lista_vazia(l)) {
        No *no_aux = l->inicio;
        // strcpy(mensagem, ".:: Lista de documentos ::.");
        // send(socket_cliente, mensagem, strlen(mensagem), 0);

        while (no_aux != NULL) {
            //printf("Titulo: %s\n", no_aux->a->titulo);
            strcpy(mensagem, "\nTitulo: ");
            strcat(mensagem, no_aux->a->titulo);
            strcat(mensagem, "\0");

            strcat(mensagem, "\nAutor: ");
            strcat(mensagem, no_aux->a->nome_do_cliente);
            strcat(mensagem, "\0");

            strcat(mensagem, "\nConteudo: ");
            strcat(mensagem, no_aux->a->conteudo);
            strcat(mensagem, "\n");
            send(socket_cliente, mensagem, strlen(mensagem), 0);

            printf("aqui\n");

            no_aux = no_aux->proximo_no;
        }
    } else {
        //printf("A lista esta vazia.\n");
        strcpy(mensagem, "A lista esta vazia.");
        send(socket_cliente, mensagem, strlen(mensagem), 0);
    }
}

void lista_buscar_e_imprimir(Lista *l, char titulo[TAMANHO_TEXTO], int socket_cliente) {
    char mensagem[TAMANHO_CONTEUDO + (2*TAMANHO_TEXTO)];

    if (!lista_vazia(l)) {
        No *no_aux = lista_buscar(l, titulo);
        if (no_aux != NULL) {
            strcpy(mensagem, "Titulo: ");
            strcat(mensagem, no_aux->a->titulo);
            strcat(mensagem, "\0");

            strcat(mensagem, "\nAutor: ");
            strcat(mensagem, no_aux->a->nome_do_cliente);
            strcat(mensagem, "\0");

            strcat(mensagem, "\nConteudo: ");
            strcat(mensagem, no_aux->a->conteudo);
            strcat(mensagem, "\n\n\0");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
        } else {
            strcpy(mensagem, "Documento inexistente.");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
        }
    } else {
        strcpy(mensagem, "A lista esta vazia.");
        send(socket_cliente, mensagem, strlen(mensagem), 0);
    }
}

void lista_remover_documento(Lista *l, char titulo[TAMANHO_TEXTO], int socket_cliente) {
    char mensagem[TAMANHO_CONTEUDO];
    if (!lista_vazia(l)) {
        No *atual = l->inicio;
        No *no_aux = NULL;
        while (atual != NULL && strcmp(atual->a->titulo, titulo)) {
            no_aux = atual;
            atual = atual->proximo_no;
        }

        if (atual != NULL) {
            if (atual == l->inicio)
                l->inicio = atual->proximo_no;
            else
                no_aux->proximo_no = atual->proximo_no;
        
            atual->proximo_no = NULL;
            if (atual == l->fim)
                l->fim = no_aux;
        
            l->tamanho--;
            free(atual->a);
            free(atual);
            printf("Documento removido com sucesso.\n");
            strcpy(mensagem, "Documento removido com sucesso.\n\0");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
        } else {
            printf("Documento inexistente.\n");
            strcpy(mensagem, "Documento inexistente.\n\0");
            send(socket_cliente, mensagem, strlen(mensagem), 0);
        }
    } else {
        printf("A lista esta vazia.\n");
        strcpy(mensagem, "A lista esta vazia.\n\0");
        send(socket_cliente, mensagem, strlen(mensagem), 0);
    }
}

int lista_tamanho(Lista *l){
    if(!lista_vazia(l)){
        return l->tamanho;
    }
    return 0;
}

void lista_sair(Lista *l) {
    if (!lista_vazia(l)) {
        No *atual = l->inicio;
        No *no_aux = NULL;
        while (atual != NULL) {
            no_aux = atual;
            atual = atual->proximo_no;
            free(no_aux->a);
            no_aux->proximo_no = NULL;
            free(no_aux);
        }
        free(l);
    }
}

boolean lista_vazia(Lista *l) {
    if (l != NULL && l->inicio == NULL)
        return true;
    return false;
}