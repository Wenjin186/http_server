server: create_socket.o server.o
	gcc -o server server.o create_socket.o
server.o:
	gcc -c server.c -o server.o
create_socket.o:
	gcc -c create_socket.c -o create_socket.o
clean:
	rm server server.o create_socket.o

