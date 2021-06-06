#include <stdio.h>
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
struct sockaddr_in endereco; //Estrutura usada com enderecos IPv4 (https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html)

void *enviar_mensagem();
void *receber_mensagem();

int main() {
                 //Protocolo IPv4       TCP     IP
  socket_cliente = socket(AF_INET, SOCK_STREAM, 0);

  if(socket_cliente == -1) {
    printf("Erro ao criar o socket!\n");
    return 1;
  }

  printf("Informe seu nome com até 31 caracteres.\n");
  scanf("%s", nome);

  //Limpando o ENTER do buffer
  char c;
  while ((c = getchar()) != '\n' && c != EOF) {}

  endereco.sin_family = AF_INET; 
  endereco.sin_port = htons(1235);
  endereco.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(&endereco.sin_zero, 0, sizeof(endereco.sin_zero));

  printf("Tentando se conectar ao servidor...\n");

  if(connect(socket_cliente, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
    printf("Erro ao se conectar!\n");
    return 1;
  }

  //TODO: Talvez venha aqui a mensagem que envia o nome de usuario para o servidor

  printf("Conectado!\n\n");

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

int conexao_finalizada_pelo_cliente = 0;

void escolher_opcao(char input[1]){
    char nome_do_cliente[TAMANHO_TEXTO];
    char titulo[TAMANHO_TEXTO];
    char conteudo[TAMANHO_CONTEUDO];
    char titulo_aux[TAMANHO_TEXTO]; 
    int enviados;
    char mensagem[TAMANHO_TEXTO];
    int opc = atoi(input);

    //Limpando o ENTER do buffer
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    switch (opc){
      case 0:     
        printf("Qual o seu nome?\n");
        fgets(mensagem, TAMANHO_TEXTO, stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
        printf("Insira o titulo do documento\n");
        fgets(mensagem, TAMANHO_TEXTO, stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
        printf("Insira o conteudo do documento\n");
        fgets(mensagem, TAMANHO_CONTEUDO, stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
        break;

      case 1:
        /* tratar no servidor
        lista_imprimir(l);*/
        break;

      case 2:
        printf("Qual o titulo a ser removido?\n");
        fgets(mensagem, 256, stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
        /*tratar no servidor
        lista_remover_documento(l, titulo_aux);*/
        break;

      case 3: 
        printf("Qual o titulo escolhido?\n");
        fgets(mensagem, 256, stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);
        /* tratar no servidor
        lista_buscar_e_imprimir(l, titulo_aux);
        */
        break;
    }
}

void *enviar_mensagem(){
  int enviados;
  char mensagem[1];

  do{
    printf("Escolha uma opcao do menu! ");
    scanf("%c", mensagem);
    enviados = send(socket_cliente, mensagem, 1, 0);

    if(strcmp(mensagem, "4") != 0) 
      escolher_opcao(mensagem);
  } while(strcmp(mensagem, "4") != 0);

  conexao_finalizada_pelo_cliente = 1;
  close(socket_cliente);       

  pthread_exit(NULL);
}

void *receber_mensagem() {
  int recebidos;
  char resposta[256];

  //TODO: Quando o servidor é desligado, impedir que aqui dê bug

  do {
    recebidos = recv(socket_cliente, resposta, 256, 0);
    resposta[recebidos] = '\0';
    printf("\nResposta do servidor: %s\n", resposta);
  } while(recebidos != -1 && !conexao_finalizada_pelo_cliente); 

  pthread_exit(NULL);
}