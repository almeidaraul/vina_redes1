client_exec:
	g++ client.cpp common.cpp ConexaoRawSocket.c -o _client.out

server_exec:
	g++ server.cpp common.cpp ConexaoRawSocket.c -o _server.out

client: client_exec
	@sudo ./_client.out

server: server_exec
	@sudo ./_server.out
