#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

int sock_cliente;
struct sockaddr_in endereco;

void *estabelecer_conexao();
void *enviar_mensagem();
void *receber_mensagem();

int main() { 
  int socket_servidor;
  socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_servidor == -1) {
    printf("\nErro ao criar o socket!\n");
    return 1;
  }

  endereco.sin_family = AF_INET;
  endereco.sin_port = htons(1236);
  endereco.sin_addr.s_addr = INADDR_ANY;
  memset(&endereco.sin_zero, 0, sizeof(endereco.sin_zero));

  //https://www.geeksforgeeks.org/socket-programming-cc/

  //Binds the socket to the address and port number specified in addr (custom data structure)
  if (bind(socket_servidor, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
    printf("\nErro na funcao bind()\n");
    return 1;
  }

  //It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection
  if (listen(socket_servidor, 3) == -1) {
    printf("\nErro na funcao listen()\n");
    return 1;
  }

  printf("\nAguardando cliente...\n");
  
  sock_cliente = accept(socket_servidor,0,0);

  if (sock_cliente == -1) {
    printf("\nErro na funcao accept()\n");
    return 1;
  }

  printf("\nCliente conectado!\n");

  pthread_t threads[2];  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  pthread_create(&threads[0], &attr, enviar_mensagem, NULL);
  pthread_create(&threads[1], &attr, receber_mensagem, NULL);

  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);

  return 0;
}

void *estabelecer_conexao() {
  // sock_cliente = accept(socket_servidor,0,0);

  // if (sock_cliente == -1) {
  //   printf("\nErro na funcao accept()\n");
  //   return 1;
  // }

  // printf("\nCliente conectado!\n");

  // pthread_t threads[2];  
  // pthread_attr_t attr;
  // pthread_attr_init(&attr);
  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // pthread_create(&threads[0], &attr, enviar_mensagem, NULL);
  // pthread_create(&threads[1], &attr, receber_mensagem, NULL);

  // pthread_join(threads[0],NULL);
  // pthread_join(threads[1],NULL);
}

void *enviar_mensagem() {
  int  enviados;
  char mensagem[256];

  do {  
    printf("Server: ");
    fgets(mensagem,256,stdin);
    mensagem[strlen(mensagem)-1] = '\0';
    enviados = send(sock_cliente, mensagem, strlen(mensagem), 0);
  } while(strcmp(mensagem,"exit") != 0);

  pthread_exit(NULL);
}

void *receber_mensagem() {
  int recebidos;
  char resposta[256];

  do {
    recebidos = recv(sock_cliente,resposta,256,0);              /* Recebe mensagem do cliente */
    resposta[recebidos] = '\0';
    printf("\n Cliente: %s\n",resposta);
  } while(strcmp(resposta, "exit")!=0); 

  pthread_exit(NULL);
}