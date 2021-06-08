Bruna Magrini da Cruz, 11218813 <br>
Marlon José Martins, 10249010 <br>
Wellington Matos Amaral, 11315054

Projeto para a matéria de Redes de Computadores (SSC0142) para o curso de Ciências de Computação da USP São Carlos.

# Especificações do projeto
Versão do sistema operacional: Ubuntu 20.04.2 LTS (Linux) <br>
Compilador utilizado: GCC <br>

Principais verificações de falha  da conexão:
 - ERRO: Erro ao criar o socket! -> Ocorre quando o servidor ou o cliente não conseguem criar um socket.
 - ERRO: Erro na funcao bind() -> Ocorre quando há falha no vinculo entre socket do servidor e o seu endereco.
 - ERRO: Erro na funcao listen() -> Ocorre quando o servidor não esta ouvindo as mensagens que estão sendo enviadas a ele.
 - ERRO: Erro na funcao accept() -> Ocorre quando o servidor não consegue aceitar o pedido de conexão do socket cliente.
 - ERRO: Erro ao se conectar! -> Ocorre quando o cliente não consegue se conectar com o servidor.

Transmissão das informações:
 - O servidor oferece ao cliente um menu de possíveis operações, em seguida o cliente escolhe qual das opções o servidor deverá executar e assim o servirdor requista informações de acordo com a operação escolhida, após a resposta do cliente, o servidor fornece o que foi requisitado e o ciclo se repete. 

# Execução do sistema
Para compilar o programa basta executar ```make``` no terminal. <br>
Para iniciá-lo execute ```./servidor``` no terminal para colocar o servidor no ar, e em outros terminais execute ```./cliente``` para os clientes terem acesso a suas funcionalidades no sistema.

# Referências
https://www.geeksforgeeks.org/socket-programming-cc/  
https://www.di.ubi.pt/~operativos/praticos/html/9-threads.html  
https://www.ppgia.pucpr.br/~laplima/ensino/sd/lab/lab-02-sock.html  
https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
