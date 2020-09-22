all:
	g++ client.cpp common.cpp ConexaoRawSocket.c -o client
	g++ server.cpp common.cpp ConexaoRawSocket.c -o server

client: all
	@sudo ./client

server: all
	@sudo ./server

test:
	g++ teste.cpp && ./a.out
