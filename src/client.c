#include "net.h"
#include "client.h"
#include "keycodes.h"

int main()
{
    int client_input = 0;

    initialize();

    system("clear");
    printf("Enter you name: ");
    scanf("%s", client_name);

    start_game_menu();

    system("clear");
    receive_data(sock, client_shape_info, CLIENT_SHAPE_INFO_SIZE, &bytes_received);

    nc_init();

    return 0;
}

void initialize(void)
{
    atexit(exit_handler);
    memset(client_name, 0, CLIENT_NAME_SIZE);
    signal(SIGUSR1, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    sock = create_client_socket();
}

void exit_handler(void)
{
    clear();
    nc_cleanup();
    close_socket(sock);
    system("clear");
}

void start_game_menu(void)
{
    char server_address[20];

    system("clear");

    if (print_game_menu() == 1)
    {
        printf("Enter server adress: ");
        scanf("%s", server_address);
        connect_to_server(sock, server_address);
        reply_with_name();
    }
    else
    {
        setup_as_a_server = true;

        if (pthread_create(&server_thread, NULL, &start_server, NULL) < 0)
        {
            printf("Could not create server thread\n");
            exit(EXIT_FAILURE);
        }
        pthread_detach(server_thread);

        sleep(1);

        connect_to_server(sock, LOCALHOST);

        reply_with_name();

        getchar();
        kill(0, SIGUSR1);
    }
}

int print_game_menu(void)
{
    const char menu[2][20] = {
        "Setup as a server",
        "Proceed as a client"
        };

    unsigned menu_option = 0;
    setup_terminal();

    while (1)
    {
        system("clear");
        for (unsigned i = 0; i < 2; i++)
        {
            if (i == menu_option)
            {
                printf(">");
            }
            else
            {
                printf(" ");
            }

            printf("%s\n", menu[i]);
        }

        if (getchar() == '\033')
        {
            getchar();
            switch (getchar())
            {
            case 'A':
                if (menu_option)
                {
                    menu_option--;
                }
                break;

            case 'B':
                if (menu_option != 1)
                {
                    menu_option++;
                }
                break;

            case 'C':
                system("clear");
                tcsetattr(0, TCSANOW, &save);
                return menu_option;
                break;
            }
        }
    }
    return 0;
}

void setup_terminal(void)
{
    struct termios custom;
    int fd = fileno(stdin);
    tcgetattr(fd, &save);
    custom = save;
    custom.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fd, TCSANOW, &custom);
}

void *start_server()
{
    execution_result = system("./bin/server");
    return (void *)&execution_result;
}

void reply_with_name(void)
{
    int code = 0;

    receive_data(sock, &code, sizeof(unsigned int), &bytes_received);

    if (code == 133)
    {
        send_data(sock, client_name, 16);
    }
}

void nc_cleanup(void)
{
    endwin();
}

void nc_init(void)
{
    initscr();
    noecho();
    curs_set(0);
    halfdelay(1);
    start_color();
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    color_set(8, 0);
    keypad(stdscr, 1);
}

void send_data_to_server(int input_code)
{
    if (input_code != 0)
    {
        send_data(sock, &input_code, sizeof(input_code));
    }
}