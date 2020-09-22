all:
	g++ client.cpp common.cpp ConexaoRawSocket.c -o _client.out
	g++ server.cpp common.cpp ConexaoRawSocket.c -o _server.out

client: all
	@sudo ./_client.out

server: all
	@sudo ./_server.out
