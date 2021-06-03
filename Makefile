all: cliente servidor

cliente: socket_cliente.c
	gcc socket_cliente.c -lpthread -o cliente

servidor: socket_servidor.c
	gcc socket_servidor.c -lpthread -o servidor