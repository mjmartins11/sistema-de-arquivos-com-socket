#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "lista.h"

#define SAIR 4

typedef struct {
  int socket_cliente;
  int continua_execucao;
} THREAD_CONTROLE;

void *enviar_mensagem(void * argumento);
void *receber_mensagem(void * argumento);

int main() { 
  int socket_servidor;
  struct sockaddr_in endereco;

  socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_servidor == -1) {
    printf("\nErro ao criar o socket!\n");
    return 1;
  }

  endereco.sin_family = AF_INET;
  endereco.sin_port = htons(1235);
  endereco.sin_addr.s_addr = INADDR_ANY;
  memset(&endereco.sin_zero, 0, sizeof(endereco.sin_zero));

  //https://www.geeksforgeeks.org/socket-programming-cc/

  //Binds the socket to the address and port number specified in addr (custom data structure)
  if (bind(socket_servidor, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
    printf("\nErro na funcao bind()\n");
    return 1;
  }

  //It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection
  if (listen(socket_servidor, 1) == -1) {
    printf("\nErro na funcao listen()\n");
    return 1;
  }

  //https://www.di.ubi.pt/~operativos/praticos/html/9-threads.html
  //https://www.ppgia.pucpr.br/~laplima/ensino/sd/lab/lab-02-sock.html
  while(1) {
    printf("\nAguardando cliente...\n");

    int socket_cliente = accept(socket_servidor, 0, 0);
    if (socket_cliente == -1) {
      printf("\nErro na funcao accept()\n");
      return 1;
    }
    printf("\nCliente conectado!\n");
  
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int 

    pthread_t thread_enviar_mensagem;
    pthread_t thread_receber_mensagem;
    pthread_create(&thread_enviar_mensagem, &attr, enviar_mensagem, (void *) &socket_cliente);
    pthread_create(&thread_receber_mensagem, &attr, receber_mensagem, (void *) &socket_cliente);
  }
  
  return 0;
}

void *enviar_mensagem(void * argumento) {
  int socket_cliente =  * (int *) argumento;

  int enviados;
  char mensagem[256];

  //Enviando o menu para o cliente
  char menu[228] = "\n************* MENU *************\nEscolha a opcao digitando o numero correspondente a ela\nOpcao 0 - inserir documento\nOpcao 1 - imprimir todos os documentos\nOpcao 2 - remover documento\nOpcao 3 - buscar documento\nOpcao 4 - sair\n";
  enviados = send(socket_cliente, menu, strlen(menu), 0);

  do {  
    printf("Server: ");
    fgets(mensagem,256,stdin);
    mensagem[strlen(mensagem)-1] = '\0';
    printf("aqui1\n");
    //TODO: Ao enviar send para um cliente que já morreu, isso aqui dá problema e faz o código parar -> TODO (4)
    enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
    printf("aqui2\n");
  } while(strcmp(mensagem,"exit") != 0);

  printf("\nExit do enviar_mensagem\n");

  pthread_exit(NULL);
}

void *receber_mensagem(void * argumento) {
  int socket_cliente =  * (int *) argumento;

  int recebidos;
  char resposta[256];

  do {
    recebidos = recv(socket_cliente,resposta,256,0);
    if(recebidos == 0) //Conexão finalizada pois o servidor recebeu um FIN (https://stackoverflow.com/a/3203663/13274909)
      break;

    resposta[recebidos] = '\0';
    printf("\n Cliente: %s\n",resposta);
    //TODO: Aqui o servidor recebe a mensagem do cliente e ela deve ser processada 
    //Nota: terão várias threads executando essa função, ou seja, provavelmente será necessário utilizar um mutex no acesso a lista
    //Assim, como o mutex, evita da lista ser acessada errada
  
  } while(strcmp(resposta, "exit")!=0); 

  printf("\nExit receber_mensagem\n");
  
  //TODO (4): Quando essa thread morrer, a do enviar_mensagem também deve morrer

  printf("Cancelou a thread de enviar: %d\n", socket_cliente);
  pthread_exit(NULL);
}