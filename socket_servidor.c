#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "lista.h"

#define SAIR 4

void *enviar_mensagem(void * socket_cliente);
void *receber_mensagem(void * socket_cliente);

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

    pthread_t threads[2];
    pthread_create(&threads[0], &attr, enviar_mensagem, (void *) &socket_cliente);
    pthread_create(&threads[1], &attr, receber_mensagem, (void *) &socket_cliente);
  }
  
  return 0;
}



void *enviar_mensagem(void * argumento) {
  int socket_cliente =  * (int *) argumento;

  int enviados;
  char mensagem[256];

  //Enviando o menu para o cliente
  char menu[228] = "\n************* MENU *************\nEscolha a opcao digitando o numero correspondente a ela\nOpcao 0 - Inserir documento\nOpcao 1 - Imprimir todos os documentos\nOpcao 2 - Remover documento\nOpcao 3 - Buscar documento\nOpcao 4 - Sair\n";
  enviados = send(socket_cliente, menu, strlen(menu), 0);

  do {  
    printf("Server: ");
    fgets(mensagem, 256, stdin); // lendo a mensagem do servidor para enviar ao cliente
    mensagem[strlen(mensagem)-1] = '\0';
    //TODO: Ao enviar send para um cliente que já morreu, isso aqui dá problema e faz o código parar -> TODO (4)
    enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
  } while (strcmp(mensagem, "exit") != 0);

  printf("\nexit enviar_mensagem\n");

  pthread_exit(NULL);
}

void escolher_opcao(char resposta, int socket_cliente) {
  char mensagem[32];
  switch(resposta){
    case '0':   
      printf("opa, entrou no 0\n");
      /*printf("Qual o seu nome?\n");
      scanf(" %[^\n]s", nome_do_cliente);
      nome_do_cliente[strlen(nome_do_cliente)] = '\0';
      printf("Insira o titulo do documento\n");
      scanf(" %[^\n]s", titulo);
      titulo[strlen(titulo)] = '\0';
      printf("Insira o conteudo do documento\n");
      scanf(" %[^\n]s", conteudo);
      conteudo[strlen(conteudo)] = '\0';
      lista_inserir(l, nome_do_cliente, titulo, conteudo);*/
      break;

    case '1':
    printf("opa, entrou no 1\n");
      /*lista_imprimir(l);*/
      break;

    case '2':
    printf("opa, entrou no 2\n");
      /*printf("Qual o titulo a ser removido?\n");
      scanf(" %[^\n]s", titulo_aux);
      titulo_aux[strlen(titulo_aux)] = '\0';
      lista_remover_documento(l, titulo_aux);*/
      break;

    case '3': 
      printf("opa, entrou no 3\n");
      /*printf("Qual o titulo desejado?\n");
      scanf(" %[^\n]s", titulo_aux);
      titulo_aux[strlen(titulo_aux)] = '\0';
      lista_buscar_e_imprimir(l, titulo_aux);*/
      break;

    case '4':
      //lista_sair(l);
      //in_code = false;
      break;
      
    default:
      strcpy(mensagem, "Escolha de opcao invalida\0");
      send(socket_cliente, mensagem, strlen(mensagem), 0);
  }
}

void *receber_mensagem(void *argumento) {
  int socket_cliente =  *(int *) argumento;

  int recebidos;
  char resposta;

  do {
    recebidos = recv(socket_cliente, &resposta, sizeof(char), 0);
    if(recebidos == 0) //Conexão finalizada, pois o servidor recebeu um FIN (https://stackoverflow.com/a/3203663/13274909)
      break;

    //printf("resposta: %d\n", resposta);

    //resposta[recebidos] = '\0';

    escolher_opcao(resposta, socket_cliente);

    printf("\n Cliente: %d\n", resposta);
    //TODO: Aqui o servidor recebe a mensagem do cliente e ela deve ser processada 
    //Nota: terão várias threads executando essa função, ou seja, provavelmente será necessário utilizar um mutex no acesso a lista
    //Assim, como o mutex, evita da lista ser acessada errada
  
  } while(resposta != '4'); 

  printf("\nexit receber_mensagem\n");
  
  //TODO (4): Quando essa thread morrer, a do enviar_mensagem também deve morrer
  pthread_exit(NULL);
}