Bruna Magrini da Cruz, 11218813  
Marlon José Martins, 10249010  
Wellington Matos Amaral, 11315054  

Projeto para matéria de Redes de Computadores (SSC0142) para o curso de Ciências de Computação da USP São Carlos.

Versão do sistema operacional: Ubuntu 20.04.2 LTS (Linux)
Compilador utilizados: GCC
Instruções adicionais: 
    Principais verificação de falha da conexão do socket:
        ERRO: Erro ao criar o socket! -> Ocorre quando o servidor ou o cliente nao conseguem criar um socket.
        ERRO: Erro na funcao bind() -> Ocorre quando ha falha no vinculo entre socket do servidor e o seu endereco.
        ERRO: Erro na funcao listen() -> Ocorre quando o servidor nao esta ouvindo as mensagens que estao sendo enviadas a ele.
        ERRO: Erro na funcao accept() -> Ocorre quando o servidor nao consegue aceitar o pedido de conexao do socket cliente.
        ERRO: Erro ao se conectar! -> Ocorre quando o cliente nao consegue se conectar com o servidor.

    Transmissão das informações:
        O servidor oferece ao cliente um menu de possiveis operacoes, em seguida o cliente escolhe qual das opcoes o servidor devera executar e assim o servirdor requista informacoes de acordo com a operacao escolhida, apos a resposta do cliente, o servidor fornece o que foi requisitado e o ciclo se repete. 

# Referências
https://www.geeksforgeeks.org/socket-programming-cc/  
https://www.di.ubi.pt/~operativos/praticos/html/9-threads.html  
https://www.ppgia.pucpr.br/~laplima/ensino/sd/lab/lab-02-sock.html  
https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html