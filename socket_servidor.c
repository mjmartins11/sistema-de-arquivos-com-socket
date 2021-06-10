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

Lista *l;
sem_t mutex;

typedef struct {
  int socket_cliente;
  int continua_execucao;
} THREAD_CONTROLE;

void escolher_opcao(char resposta, int socket_cliente);
void *conexao(void * argumento);

Lista *l; // lista que armazena todos os arquivos criados pelos clientes

int main() { 
  int socket_servidor;
  struct sockaddr_in endereco;
  l = lista_criar();

  sem_init(&mutex, 0, 1);

  socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_servidor == -1) {
    printf("ERRO: Erro ao criar o socket!\n");
    return 1;
  }

  endereco.sin_family = AF_INET;
  endereco.sin_port = htons(1236);
  endereco.sin_addr.s_addr = INADDR_ANY;
  memset(&endereco.sin_zero, 0, sizeof(endereco.sin_zero));

  if (bind(socket_servidor, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
    printf("ERRO: Erro na funcao bind()\n");
    return 1;
  }
  if (listen(socket_servidor, 1) == -1) {
    printf("ERRO: Erro na funcao listen()\n");
    return 1;
  }

  while(1) {
    printf("INFO: Aguardando cliente...\n");

    int socket_cliente = accept(socket_servidor, 0, 0);
    if (socket_cliente == -1) {
      printf("ERRO: Erro na funcao accept()\n");
      return 1;
    }
    printf("INFO: Cliente %d conectado!\n\n", socket_cliente);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_t thread;
    pthread_create(&thread, &attr, conexao, (void *) &socket_cliente);
  }

  sem_destroy(&mutex);
  lista_sair(l);

  printf("INFO: O servidor foi encerrado.\n");
  return 0;
}

void *conexao(void * argumento) {
  int socket_cliente =  * (int *) argumento;
  int retorno;                                               
  char operacao;
  
  //Enviando o menu para o cliente
  char menu[TAMANHO_CONTEUDO] = "\n************* MENU *************\nEscolha a opcao digitando o numero correspondente a ela\nOpcao 0 - Inserir documento\nOpcao 1 - Imprimir todos os documentos\nOpcao 2 - Remover documento\nOpcao 3 - Buscar documento\nOpcao 4 - Sair\n";
  send(socket_cliente, menu, strlen(menu), 0);

  //Aguardando resposta do cliente
  retorno = recv(socket_cliente, &operacao, 256, 0);
  if(retorno == 0) //Conexão finalizada pois o servidor recebeu um FIN (https://stackoverflow.com/a/3203663/13274909)
    pthread_exit(NULL);

  while(operacao != '4') {
    //Realizando operação solicitada pelo cliente
    printf("INFO: Realizando operação %c para o cliente %d.\n", operacao, socket_cliente);
    escolher_opcao(operacao, socket_cliente);

    //Aguardando resposta de nova operação do cliente
    retorno = recv(socket_cliente, &operacao, 256, 0);
    if(retorno == 0) 
      break;
  }

  printf("INFO: Cliente %d foi desconectado.\n", socket_cliente);

  pthread_exit(NULL);
}

void escolher_opcao(char resposta, int socket_cliente) {
  char nome_do_cliente[TAMANHO_TEXTO];
  char titulo[TAMANHO_TEXTO];
  char conteudo[TAMANHO_CONTEUDO];
  char mensagem[TAMANHO_TEXTO];
  int recebidos;
  char lista_tam;

  switch(resposta){
    case '0':   // Incluir novo documento
      strcpy(mensagem, "Qual o seu nome?");
      send(socket_cliente, mensagem, TAMANHO_TEXTO, 0);
      recebidos = recv(socket_cliente, nome_do_cliente, TAMANHO_TEXTO, 0);
      if(recebidos == 0) return;
      nome_do_cliente[recebidos] = '\0';

      strcpy(mensagem, "Insira o titulo do documento");
      send(socket_cliente, mensagem, TAMANHO_TEXTO, 0);
      recebidos = recv(socket_cliente, titulo, TAMANHO_TEXTO, 0);
      if(recebidos == 0) return;
      titulo[recebidos] = '\0';

      strcpy(mensagem, "Insira o conteudo do documento");
      send(socket_cliente, mensagem, TAMANHO_CONTEUDO, 0);
      recebidos = recv(socket_cliente, conteudo, TAMANHO_CONTEUDO, 0);
      if(recebidos == 0) return;
      conteudo[recebidos] = '\0';

      sem_wait(&mutex);
      lista_inserir(l, nome_do_cliente, titulo, conteudo, socket_cliente);
      sem_post(&mutex);
      break;

    case '1':   // Imprimir lista completa de documentos
      lista_imprimir(l, socket_cliente);
      break;

    case '2':   // Remover documento
      strcpy(mensagem, "Qual o titulo a ser removido?\n");
      send(socket_cliente, mensagem, TAMANHO_CONTEUDO, 0);
      recebidos = recv(socket_cliente, titulo, TAMANHO_CONTEUDO, 0);
      if(recebidos == 0) return;
      titulo[recebidos] = '\0';

      sem_wait(&mutex);
      lista_remover_documento(l, titulo, socket_cliente);
      sem_post(&mutex);
      break;

    case '3':   // Imprimir documento especifico
      printf("Qual o titulo desejado?\n");
      strcpy(mensagem, "Qual o titulo desejado?\n");
      send(socket_cliente, mensagem, TAMANHO_CONTEUDO, 0);
      recebidos = recv(socket_cliente, titulo, TAMANHO_CONTEUDO, 0);
      if(recebidos == 0) return;
      titulo[recebidos] = '\0';
      lista_buscar_e_imprimir(l, titulo, socket_cliente);
      break;
      
    default:
      strcpy(mensagem, "Escolha de opcao invalida\n\0");
      send(socket_cliente, mensagem, strlen(mensagem), 0);
  }

  return;
}