/*=====================================================
  net.c
  Vladislav Zykov
  18.01.2023
  Eltex School
=====================================================*/

#include "net.h"

void net_error(const char* message, bool stdErr)
{
    if (!stdErr)
    {
         perror(message);
    }
    else 
    {
        fprintf(stderr, message, "%s");
    }
        
    exit(EXIT_FAILURE);
}

int create_socket_tcp(void)
{
    int socktcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socktcp == -1)
    {
        net_error("Failed to create tcp socket\n", false);
    }
    
    return socktcp;
}

int create_listener_socket(void)
{
    int sockListener = 0;
    struct sockaddr_in listenerInfo = {0};

    sockListener = create_socket_tcp();

    listenerInfo.sin_family = AF_INET;
	listenerInfo.sin_addr.s_addr = inet_addr(LOCALHOST);
	listenerInfo.sin_port = htons(DEFAULT_PORT);
	
	if (bind(sockListener, (struct sockaddr*)&listenerInfo, sizeof(listenerInfo)) == -1)
    {
        net_error("Failed to bind socket\n", false);
    }
        
    return sockListener;
}

int create_client_socket(void)
{
    return create_socket_tcp();
}

void close_socket(int socketToCloseDescriptor)
{
    if (close(socketToCloseDescriptor) == -1)
    {
        net_error("Failed to close socket\n", false);
    }
}

void close_sockets(int* socketBuffer, unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        close_socket(socketBuffer[i]);
    }
}