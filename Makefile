all: cliente servidor 

cliente: socket_cliente.c
	gcc socket_cliente.c -lpthread -o cliente

servidor: lista.o socket_servidor.o
	gcc lista.o socket_servidor.o -lpthread -o servidor

servidor.o:
	gcc servidor.c -lpthread -c

lista.o: lista.c
	gcc lista.c -c