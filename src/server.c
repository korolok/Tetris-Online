#include "server.h"
#include "net.h"
#include "game.h"
#include "util.h"

void interrupt_handler(int sigint)
{
#ifdef DEBUG
    printf("Signal accepted SIGINT: %d\n", sigint);
#endif
    should_stop_accepting_clients = true;
    make_log("Server: SIGINT");
    cleanup();
}

void stop_accepting_clients_handler(int sigusr)
{
#ifdef DEBUG
    printf("Signal accepted SIGUSR1: %d stopped accepting clients\n", sigusr);
#endif
    should_stop_accepting_clients = true;
    make_log("Server: SIGUSR1");
}

void initialize_server(int *argc, char *path_pointer)
{
    set_path_to_root(path_pointer);
    //  Setup server
    listener_socket = create_listener_socket();
    listen_for_new_connections(listener_socket);

    //  Setup signals
    signal(SIGUSR1, stop_accepting_clients_handler);
    signal(SIGINT, interrupt_handler);
    (void)argc;
    make_log("Server: initialized");
}

void accept_clients(bool* stop_accepting)
{
    const unsigned int  CODE = CODE_TO_REPLY_WITH_NAME;
    char                name_buffer[NAME_BUFFER_SIZE] = {0};
    int                 bytes_received = 0;

    while (players_connected != MAX_PLAYER_COUNT && *stop_accepting == false)
    {   
        players[players_connected].socket = SOCKET_NULL;

        // Accept client
        while (*stop_accepting == false)
        {
            players[players_connected].socket = accept_client(listener_socket, true);

            if (players[players_connected].socket != SOCKET_NULL && players[players_connected].socket != -1)
            {
                break;
            }
        }

        if (*stop_accepting)
        {
            break;
        }
		
        // Ask client for their name
        memset(&name_buffer, 0, NAME_BUFFER_SIZE);
        send_data(players[players_connected].socket, (void*)&CODE, sizeof(CODE));
        receive_data(players[players_connected].socket, &name_buffer, NAME_BUFFER_SIZE, &bytes_received);

        if (bytes_received == 0)
        {
            printf("Failed to receive player's name\n");
            exit(EXIT_FAILURE);
        }
        
        strcpy(players[players_connected].name, name_buffer);
        printf("Player %s connected\n", players[players_connected].name);
        make_log("Server: client connected");

        ++players_connected;
	}

#ifdef DEBUG
    printf("Connected clients - %d\n", players_connected);
    for (unsigned int i = 0; i < players_connected; ++i)
    {
        printf("Player #%d name: %s | Socket: %d\n", i, players[i].name, players[i].socket);
    }
#endif
    
    if (players_connected == 0) 
    {
        printf("Failed to accept clients\n");
        exit(EXIT_FAILURE);
    }
}

void set_clients_non_blocking(void)
{
    for (unsigned int i = 0; i < players_connected; ++i)
    {
        set_socket_non_blocking(players[i].socket);
    }
}

unsigned int receive_data_from_clients(void)
{   
    unsigned int    keycode         = 0;
    int             bytes_received  = 0;

    receive_data(players[current_active_player].socket, &keycode, sizeof(keycode), &bytes_received);

#ifdef DEBUG
    if (bytes_received == 0)
    {
        printf("Failed to receive data from client\n");
    }
#endif

    return keycode;
}

bool send_data_to_clients(void *buffer, unsigned int buff_size)
{
    for (unsigned int i = 0; i < players_connected; ++i)
    {
        if (!send_data(players[i].socket, buffer, buff_size))
        {
            return false;
        }
    }
    return true; 
}

void calculate_drop_down_time(void)
{
    long long delta_time = 0;

    current_game_time = get_timestamp_in_milliseconds();
    delta_time = current_game_time - previous_game_time;
    previous_game_time = current_game_time;
    shape_drop_down_time += delta_time;
}

void process_data(void)
{
    calculate_drop_down_time();

    if (current_key != 0)
    {
        shape_control(current_key);
        if(current_key == 2)
        {
            //если клавиша будет стрелкой вниз, 
            //приравниваем время падения фигуры к константе,
            //меньшей чем время падения для плавности игры
            shape_drop_down_time = 150;
        }
        current_key = 0;
    }

    if (shape_drop_down_time >= (800 - 50 * get_game_level()))
    {
        shape_drop_down_time = 0;
        shape_drop();
        form_cup();
    }

    for (int i = 0; i < (int)players_connected; ++i)
    {
        if (players[i].shape_type == get_id_current_shape())
        {
            //При смене текущего игрока очищаем буфер ввода
            int current_player = current_active_player;

            current_active_player = i;

            if(current_player != i)
            {
                while(receive_data_from_clients() != 0);
            }
            break;
        }
    }
}

void update_server_time(long long* send_data_time)
{
    static long long    current_server_time     = 0;
    static long long    previous_server_time    = 0;
    long long           delta_time              = 0;

    current_server_time = get_timestamp_in_milliseconds();
    delta_time = current_server_time - previous_server_time;
    previous_server_time = current_server_time;
    *send_data_time += delta_time;
}

void start_game(void)
{   
    char buffer_to_send[1024] = {0};

    set_random_seed();
    set_user_count_pointer(&players_connected);

    for (int i = 0; i < (int)players_connected; ++i)
    {
        players[i].shape_type = get_shapes_type(i);
    }

    spawn_new_shape();

    for (int i = 0; i < (int)players_connected; ++i)
    {
        if (players[i].shape_type == get_id_current_shape())
        {
            current_active_player = i;
            break;
        }
    }

    for (int i = 0; i < (int)players_connected; ++i)
    {
        char buffer_shape_type_message[37] = {0};
        switch (players[i].shape_type)
        {

        case 0:
            buffer_shape_type_message[0] = 'T';
            break;

        case 1:
            buffer_shape_type_message[0] = 'O';
            break;

        case 2:
            buffer_shape_type_message[0] = 'I';
            break;

        case 3:
            buffer_shape_type_message[0] = 'J';
            break;

        case 4:
            buffer_shape_type_message[0] = 'L';
            break;

        case 5:
            buffer_shape_type_message[0] = 'Z';
            break;

        case 6:
            buffer_shape_type_message[0] = 'S';
            break;
        }

        buffer_shape_type_message[1] = ' ';
        buffer_shape_type_message[2] = '-';
        buffer_shape_type_message[3] = ' ';

        strcat(buffer_shape_type_message, players[i].name);

        buffer_shape_type_message[strlen(buffer_shape_type_message)] = '\n';
        strcat(buffer_to_send, buffer_shape_type_message);
    }

    send_data_to_clients(buffer_to_send, strlen(buffer_to_send));
}

void cleanup(void)
{
    close_socket(listener_socket);

    if (players_connected != 0)
    {
        for (unsigned int i = 0; i < players_connected; ++i)
        {
            close_socket(players[i].socket);
        }
    }

}

int main(int argc, char* argv[])
{
    long long send_data_time = 0;
	
    initialize_server(&argc, argv[0]);
    accept_clients(&should_stop_accepting_clients);
    start_game();
    set_clients_non_blocking();
	
    while (!get_game_over())
    {
        current_key = receive_data_from_clients();
        process_data();
		update_server_time(&send_data_time);

        int total_score = get_total_score();

        if (send_data_time >= 100)
        {
            if (!send_data_to_clients(get_cup_to_send(), get_cup_to_send_size()))
            {
                break;
            }
            if (!send_data_to_clients(&total_score, sizeof(total_score)))
            {
                break;
            }
			
            send_data_time = 0;
        }
		
    }

    cleanup();

    exit(EXIT_SUCCESS);
    return 0;
}