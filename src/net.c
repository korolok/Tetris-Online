/*=====================================================
  net.c
  Vladislav Zykov
  18.01.2023
  Eltex School
=====================================================*/

#include "net.h"

void net_error(const char* message, bool std_err)
{
    if (!std_err)
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
    int sock_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock_tcp == -1)
    {
        net_error("Failed to create tcp socket\n", false);
    }
   
    return sock_tcp;
}

int create_listener_socket(void)
{
    int sock_listener = 0;
    struct sockaddr_in listener_info = {0};

    sock_listener = create_socket_tcp();

    listener_info.sin_family = AF_INET;
    listener_info.sin_addr.s_addr = inet_addr(LOCALHOST);
    listener_info.sin_port = htons(DEFAULT_PORT);

    if (bind(sock_listener, (struct sockaddr*)&listener_info, sizeof(listener_info)) == -1)
    {
        net_error("Failed to bind socket\n", false);
    }
    
    return sock_listener;
}

int create_client_socket(void)
{
    return create_socket_tcp();
}

int accept_client(int listener_socket_descriptor)
{
    int new_client_sock = 0;
    struct sockaddr_in new_client_info = {0};
    int sock_size = sizeof(struct sockaddr_in);

    new_client_sock = accept(
        listener_socket_descriptor,
        (struct sockaddr*)&new_client_info,
        (socklen_t*)&sock_size
    );  

    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(new_client_info.sin_addr.s_addr), str, INET_ADDRSTRLEN);
    printf("Connection established [%s]\n", str);

    return new_client_sock;
}

void listen_for_new_connections(int listener_socket_descriptor)
{
    if (listen(listener_socket_descriptor, MAX_BACKLOG) == -1)
    {
        net_error("Failed to start listening\n", false);
    }

    printf("Listening for incoming connections...\n");
}

void connect_to_server(int client_socket_sescriptor, const char* server_address)
{
    struct sockaddr_in server_info;

    server_info.sin_addr.s_addr = inet_addr(server_address);
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(DEFAULT_PORT);

    if (connect(client_socket_sescriptor , (struct sockaddr*)&server_info , sizeof(server_info)) < 0)
    {
        net_error("Failed to connect to the server\n", false);
    }
}

bool send_data(int socket_descriptor, void* data_buffer, unsigned int buff_size) 
{
    if(send(socket_descriptor, data_buffer, buff_size, 0) < 0) 
    {
        net_error("Failed to send data\n", false);
        return false;
    }

    return true;
}

bool receive_data(int socket_descriptor, void* data_buffer, unsigned int buff_size, int* bytes_received)
{
    if (*bytes_received = recv(socket_descriptor, data_buffer, buff_size, 0) > 0)
    {
        return true;
    }
    
    return false;
}

void close_socket(int socket_to_close_descriptor)
{
    if (close(socket_to_close_descriptor) == -1)
    {
        net_error("Failed to close socket\n", false);
    }
}

void close_sockets(int* socket_buffer, unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        close_socket(socket_buffer[i]);
    }
}