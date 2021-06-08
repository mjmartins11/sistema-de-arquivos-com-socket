#ifndef LISTA_H
    #define LISTA_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>

    #define boolean int
    #define true 1
    #define false 0
    
    #define TAMANHO_TEXTO 32
    #define TAMANHO_CONTEUDO 256

    typedef struct lista_ Lista;

    Lista *lista_criar();
    void lista_inserir(Lista *l, char nome_do_cliente[TAMANHO_TEXTO], char titulo[TAMANHO_TEXTO], char conteudo[TAMANHO_CONTEUDO], int socket_cliente);
    void lista_imprimir(Lista *l, int socket_cliente);
    void lista_buscar_e_imprimir(Lista *l, char titulo[TAMANHO_TEXTO], int socket_cliente);
    void lista_remover_documento(Lista *l, char titulo[TAMANHO_TEXTO], int socket_cliente);
    int lista_tamanho(Lista *l);
    void lista_sair(Lista *l);
    boolean lista_vazia(Lista *l);

#endif