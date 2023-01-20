/*=================================================================================
  net.h
  Vladislav Zykov
  18.01.2023
  Eltex School

  This module should be used in implementation of networking logic.
  Any direct use of linux api is strictly prohibited and limited by this module.
  Add lacking functionality here if needed.
=================================================================================*/

#ifndef NET_H
#define NET_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT	7070
#define LOCALHOST		"127.0.0.1"
#define MAX_BACKLOG		128

static void net_error(const char* message, bool stdErr);

//	The following four procedures return socket descriptor values

static int create_socket_tcp(void);
//	Should be used at the server side only
int create_listener_socket(void);
//	Should be used at the client side only
int create_client_socket(void);
int accept_client(int listenerSocketDescriptor);

void listen_for_new_connections(int listenerSocketDescriptor);
void connect_to_server(
	int clientSocketDescriptor,
	const char* serverAddress
);

bool send_data(
	int socketDescriptor,
	void* dataBuffer,
	unsigned int buffSize
);
bool receive_data(
	int socketDescriptor,
	void* dataBuffer,
	unsigned int buffSize,
	int* bytesReceived
);

void close_socket(int socketToCloseDescriptor);
void close_sockets(
	int* socketBuffer,
	unsigned int size
);

#endif //NET_H