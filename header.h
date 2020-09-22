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
extern int server_sequence, client_sequence;

void send_(std::string s);

std::string receive_();

std::string format(std::string s);

void read_formatted(std::string s);

#endif
