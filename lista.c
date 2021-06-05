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

void lista_inserir(Lista *l, char nome_do_cliente[TAMANHO_TEXTO], char titulo[TAMANHO_TEXTO], char conteudo[TAMANHO_CONTEUDO]) {
    printf("\n");
    if (l != NULL) {

        if (lista_buscar(l, titulo) != NULL) {
            printf("O documento ja existe.\n");
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
            printf("Documento inserido com sucesso.\n");
        } else {
            printf("Falha ao inserir documento.\n");
        }
    }
    printf("\n");
}

void lista_imprimir(Lista *l) {
    printf("\n");
    if (!lista_vazia(l)) {
        No *no_aux = l->inicio;
        printf(".:: Lista de documentos ::.\n");
        while (no_aux != NULL) {
            printf("Titulo: %s\n", no_aux->a->titulo);
            printf("Autor: %s\n", no_aux->a->nome_do_cliente);
            printf("Conteudo: %s\n\n", no_aux->a->conteudo);
            no_aux = no_aux->proximo_no;
        }
    } else {
        printf("A lista esta vazia.\n");
    }
    printf("\n");
}

void lista_buscar_e_imprimir(Lista *l, char titulo[TAMANHO_TEXTO]) {
    if (!lista_vazia(l)) {
        No *no_aux = lista_buscar(l, titulo);
        if (no_aux != NULL) {
            printf("Titulo: %s\n", no_aux->a->titulo);
            printf("Autor: %s\n", no_aux->a->nome_do_cliente);
            printf("Conteudo: %s\n\n", no_aux->a->conteudo);
        } else {
            printf("Documento inexistente\n");
        }
    } else {
        printf("A lista esta vazia.\n");
    }
}

void lista_remover_documento(Lista *l, char titulo[TAMANHO_TEXTO]) {
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
        } else {
            printf("Documento inexistente.\n");
        }
    } else {
        printf("A lista esta vazia.\n");
    }
}

/*void lista_remover_registro2(Lista *l, int id, int qtd_aluno) {
    if (!lista_vazia(l)) {
        while (qtd_aluno > 0) {
            No *atual = l->inicio;
            No *no_aux = NULL;
            while (atual != NULL && aluno_id(atual->a) != id) {
                no_aux = atual;
                atual = atual->proximo_no;
            }
            if (atual != NULL) {
                if (atual == l->inicio) {
                    l->inicio = atual->proximo_no;
                } else {
                    no_aux->proximo_no = atual->proximo_no;
                }
                atual->proximo_no = NULL;
                if (atual == l->fim) {
                    l->fim = no_aux;
                }
                l->tamanho--;
                aluno_remover(atual->a);
                free(atual);
            }
            qtd_aluno--;
        }
        printf("Registros removidos com sucesso.\n");
    } else {
        printf("A lista esta vazia.\n");
    }
}*/

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

/*int lista_aluno_qtd_id(Lista *l, int id) {
    int qtd = 0;
    if (!lista_vazia(l)) {
        No *no_aux = (No*) malloc(sizeof(No));
        no_aux = l->inicio;
        while (no_aux != NULL) {
            if (aluno_id(no_aux->a) == id)
                qtd++;
            no_aux = no_aux->proximo_no;
        }
    }
    return qtd;
}*/

boolean lista_vazia(Lista *l) {
    if (l != NULL && l->inicio == NULL)
        return true;
    return false;
}