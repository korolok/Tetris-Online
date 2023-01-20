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

int accept_client(int listenerSocketDescriptor)
{
    int newClientSock = 0;
    struct sockaddr_in newClientInfo = {0};
    int sockSize = sizeof(struct sockaddr_in);

    newClientSock = accept(
                        listenerSocketDescriptor,
                        (struct sockaddr*)&newClientInfo,
                        (socklen_t*)&sockSize
                    );

    char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(newClientInfo.sin_addr.s_addr), str, INET_ADDRSTRLEN);
	printf("Connection established [%s]\n", str);

    return newClientSock;
}

void listen_for_new_connections(int listenerSocketDescriptor)
{
    if (listen(listenerSocketDescriptor, MAX_BACKLOG) == -1)
    {
        net_error("Failed to start listening\n", false);
    }
    
    printf("Listening for incoming connections...\n");
}

void connect_to_server(int clientSocketDescriptor, const char* serverAddress)
{
    struct sockaddr_in serverInfo;

    serverInfo.sin_addr.s_addr = inet_addr(serverAddress);
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(DEFAULT_PORT);

	if (connect(clientSocketDescriptor , (struct sockaddr*)&serverInfo , sizeof(serverInfo)) < 0)
    {
        net_error("Failed to connect to the server\n", false);
    }
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