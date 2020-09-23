#ifdef __cplusplus
	extern "C" {
#endif
// ConexaoRawSocket.c
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int ConexaoRawSocket(char *device);

#ifdef __cplusplus
	}

#include <bits/stdc++.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <dirent.h>

//common.cpp
#include <chrono>
#include <thread>

#define T_CD 0
#define T_LS 1
#define T_VER 2
#define T_LINHA 3
#define T_LINHAS 4
#define T_EDIT 5
#define T_ACK 8
#define T_NACK 9
#define T_LINHASIF 10 //linhas inicial e final
#define T_LSC 11 //conteudo ls
#define T_ARQC 12 //conteudo arquivo
#define T_FIM 13 //fim da transmissao
#define T_ERR 15 //erro

extern int my_seq, their_seq, c;

void send_(std::string s);

std::string receive_();

void send_status(std::string s);

std::string receive_status();

std::string format(std::string s, int type);

int parity(std::string s);

void read_formatted(std::string s);

std::string get_data(std::string s, int end_of_data);

int get_parity(std::string m, int end_of_data);

int get_type(std::string m);

int get_sequence(std::string m);

int get_size(std::string m);

std::string _itos(int val, int sz);

std::vector<std::string> divide(std::string s, int sz);
#endif
