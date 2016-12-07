all: client.o server.o
	gcc -o run server.o client.o 

client: client.c
	gcc -o client.o client.c

server: server.c
	gcc -o server.o server.c

clean:
	rm run client.o server.o
