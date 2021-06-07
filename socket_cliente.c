#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "lista.h"

#define TAMANHO_TEXTO 32

char nome[TAMANHO_TEXTO];

int socket_cliente;
struct sockaddr_in endereco;

void *conexao();

int main() {
  socket_cliente = socket(AF_INET, SOCK_STREAM, 0);

  if(socket_cliente == -1) {
    printf("ERRO: Erro ao criar o socket!\n");
    return 1;
  }

  endereco.sin_family = AF_INET; 
  endereco.sin_port = htons(1236);
  endereco.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(&endereco.sin_zero, 0, sizeof(endereco.sin_zero));

  printf("INFO: Tentando se conectar ao servidor...\n");

  if(connect(socket_cliente, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
    printf("ERRO: Erro ao se conectar!\n");
    return 1;
  }

  printf("INFO: Conectado!\n");

  pthread_t threads[2];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  pthread_t thread;
  pthread_create(&thread, &attr, conexao, (void *) &socket_cliente);

  pthread_join(thread, NULL);

  printf("INFO: Desconectado.\n\n");

  return 0;
}

void limpar_buffer() {
  char c;
  while ((c = getchar()) != '\n' && c != EOF) {}
}

void receber_mensagem(int socket_cliente) {
  int retorno;
  char mensagem[256];

  retorno = recv(socket_cliente, mensagem, 256, 0);
  if(retorno == 0) 
    return;
  mensagem[retorno] = '\0';

  printf("\n%s\n\n", mensagem);
}

void enviar_mensagem(int socket_cliente) {
  int retorno;
  char mensagem[256];

  printf("Digite a resposta: ");
  fgets(mensagem, 256, stdin); 
  mensagem[strlen(mensagem)-1] = '\0';

  retorno = send(socket_cliente, mensagem, sizeof(char)*strlen(mensagem), 0);
}

void *conexao() {
  int retorno;
  char operacao;
  char mensagem[256];
  
  //Recebendo o menu do servidor
  retorno = recv(socket_cliente, mensagem, 256, 0);
  if(retorno == 0) 
    pthread_exit(NULL);
  mensagem[retorno] = '\0';
  printf("\n%s\n", mensagem);

  printf(">> Digite a operação: ");
  scanf("%c", &operacao);
  limpar_buffer();

  while(operacao != '4') {
    //Enviando operação para o servidor
    retorno = send(socket_cliente, &operacao, sizeof(char), 0);

    switch(operacao) {
      case '0':
        //Recebendo "Qual o seu nome?"
        receber_mensagem(socket_cliente);
        //Respondendo o nome
        enviar_mensagem(socket_cliente);

        //Recebendo "Insira o titulo do documento?"
        receber_mensagem(socket_cliente);
        //Respondendo o titulo
        enviar_mensagem(socket_cliente);

        //Recebendo "Insira o conteudo do documento"
        receber_mensagem(socket_cliente);
        //Respondendo o conteudo
        enviar_mensagem(socket_cliente);

        printf("\n");
        break;

      case '1':
        printf("\n");
        break;

      case '2':
        //Recebendo "Qual o titulo a ser removido?"
        receber_mensagem(socket_cliente);
        //Respondendo o titulo
        enviar_mensagem(socket_cliente);

        //Recebendo resultado da operação
        receber_mensagem(socket_cliente);

        printf("\n");
        break;

      case '3':
        //Recebendo "Qual o titulo desejado?"
        receber_mensagem(socket_cliente);
        //Respondendo o titulo
        enviar_mensagem(socket_cliente);

        //Recebendo resultado da operação
        receber_mensagem(socket_cliente);

        printf("\n");
        break;

      default:
        receber_mensagem(socket_cliente);
    }

    printf(">> Digite a operação: ");
    scanf("%c", &operacao);
    limpar_buffer();
  }

  pthread_exit(NULL);
}