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
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.h"

#define DEFAULT_PORT    7070
#define LOCALHOST       "127.0.0.1"
#define MAX_BACKLOG     128
#define SOCKET_NULL     -100

//  The following four procedures return socket descriptor values

int create_socket_tcp(void);
//  Should be used at the server side only
int create_listener_socket(void);
//  Should be used at the client side only
int create_client_socket(void);
int accept_client(int listener_socket_descriptor, bool is_silent);

void listen_for_new_connections(int listener_socket_descriptor);
void connect_to_server(int client_socket_descriptor, const char* server_address);

bool send_data(int socket_descriptor, void* data_buffer, unsigned int buff_size);
bool receive_data(int socket_descriptor, void* data_duffer, unsigned int buff_size, int* bytes_received);

void set_socket_non_blocking(int socket_descriptor);

void close_socket(int socket_to_close_descriptor);
void close_sockets(int* socket_buffer, unsigned int size);

#endif // NET_H
