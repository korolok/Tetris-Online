#include "net.h"
#include "client.h"
#include "keycodes.h"

int main(int argc, char *argv[])
{   
    int client_input = 0;

    system("clear");
    initialize(&argc, argv[0]);
    start_game_menu();

    if (client_start_game)
    {
        printf("Waiting for other clients...\n");
    }

    receive_data(sock, client_shape_info, CLIENT_SHAPE_INFO_SIZE, &bytes_received);
    signal(SIGINT, game_over_handler);

    nc_init();

    while (client_input != KEY_ESCAPE && !game_over)
    {
        client_input = getch();
        flushinp();
        int input_code = procces_input(client_input);

        send_data_to_server(input_code);

        if (!receive_data_from_server())
        {
            break;
        }

        draw_cup();
    }

    if (setup_as_a_server)
    {
        kill(0, SIGINT);
    }

    system("clear");
    printf("\n\n\n\n\n\t\t Game over! Total score: %d\n", score);
    sleep(3);
    (void)argc;

    return 0;
}

void initialize(int *argc, char *path_pointer)
{
    atexit(exit_handler);
    set_path_to_root(path_pointer);
    memset(client_name, 0, CLIENT_NAME_SIZE);
    signal(SIGUSR1, SIG_IGN);
    signal(SIGWINCH, resize_term_handler);
    sock = create_client_socket();
    get_terminal_size(&initial_terminal_size_x, &initial_terminal_size_y);
    (void)argc;

    make_log("Client: initialized");
}

void start_game_menu(void)
{
    char server_address[20];
    
    print_welcome_message();

    if (print_game_menu() == 1)
    {
        client_start_game = true;
        printf("Press Enter to continue\n\n");
        printf("Enter server adress: ");
        scanf("%s", server_address);
        system("clear");
        connect_to_server(sock, server_address);
        reply_with_name();
    }
    else
    {
        setup_as_a_server = true;

        printf("Press Enter to stop accepting clients\n");

        if (pthread_create(&server_thread, NULL, (void *)&start_server, NULL) < 0)
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
    nc_init();

    const char menu[2][20] = {
        "Setup as a server",
        "Proceed as a client"
    };

    unsigned menu_option = 0;
    setup_terminal();

    while (true)
    {
        erase();
        printw("Press Enter to continue\n\n");

        for (unsigned i = 0; i < 2; i++)
        {
            if (i == menu_option)
            {
                addch('>');
            }
            else
            {
                addch(' ');
            }

            printw("%s\n", menu[i]);
        }

        switch (getch())
        {
            case KEY_UP:
                if (menu_option)
                {
                    menu_option--;
                }
                break;

            case KEY_DOWN:
                if (menu_option != 1)
                {
                    menu_option++;
                }
                break;

            // 10 = key enter code
            case 10:
                clear();
                tcsetattr(0, TCSANOW, &save);
                nc_cleanup();
                return menu_option;
                break;
        }
    }
    return 0;
}

void *start_server(void)
{
    char app_path_buffer[FILENAME_MAX] = {0};
    
    app_path_buffer[0] = '.';
    app_path_buffer[1] = '/';
    concat_to_root_path("tetris_server", app_path_buffer);

    make_log("Client: starting up server");
    
    execution_result = system(app_path_buffer);

    if (execution_result < 0)
    {
        perror("Server not found");
        exit(EXIT_FAILURE);
    }

    return (void *)&execution_result;
}

void print_welcome_message(void)
{
    int row = 0;
    int col = 0;

    nc_init();
    
    init_pair(9, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(9));
    
    getmaxyx(stdscr, row, col);
    mvwprintw(stdscr, (row / 2) - 1, (col - 25) / 2, "Welcome To Tetris Online!");
    mvwprintw(stdscr, (row / 2) + 1, (col - 16) / 2, "Type your name: ");
    echo();

    attroff(COLOR_PAIR(9));

    scanw("%s", client_name);
    nc_cleanup();
}

void exit_handler(void)
{
    clear();
    nc_cleanup();
    close_socket(sock);
    system("clear");

    make_log("Client: exiting");
}

void resize_term_handler()
{
    unsigned int curr_term_size_x = 0;
    unsigned int curr_term_size_y = 0;

    get_terminal_size(&curr_term_size_x, &curr_term_size_y);

    if (
        curr_term_size_x < initial_terminal_size_x ||
        curr_term_size_y < initial_terminal_size_y
    ) {
        nc_cleanup();
        refresh();
        clear();
        nc_init();
    }
    else 
    {
        getmaxyx(stdscr, curr_term_size_y, curr_term_size_x);
        wresize(stdscr, curr_term_size_y, curr_term_size_x);
    }
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

void send_data_to_server(int input_code)
{
    if (input_code != 0)
    {
        send_data(sock, &input_code, sizeof(input_code));
    }
}

bool receive_data_from_server()
{
    if (!receive_data(sock, cup, CUP_SIZE, &bytes_received))
    {
        return false;
    }
    if (!receive_data(sock, &score, sizeof(score), &bytes_received))
    {
        return false;
    }
    return true;
}

int procces_input(int key_code)
{
    if (key_code == KEY_UP)
    {
        return KEY_ARROW_UP;
    }
    else if (key_code == KEY_DOWN)
    {
        return KEY_ARROW_DOWN;
    }
    else if (key_code == KEY_LEFT)
    {
        return KEY_ARROW_LEFT;
    }
    else if (key_code == KEY_RIGHT)
    {
        return KEY_ARROW_RIGHT;
    }
    return 0;
}

void draw_cup(void)
{
    nc_setup_colors();
    erase();

    for (int i = 0; cup[i] != '\0'; ++i)
    {
        if (cup[i] == '|')
        {
            addch('|' | COLOR_PAIR(1));
        }
        else if (cup[i] == '-')
        {
            addch('-' | COLOR_PAIR(1));
            addch('-' | COLOR_PAIR(1));
        }
        else if (cup[i] == 'O')
        {
            addch('O' | COLOR_PAIR(2));
            addch('O' | COLOR_PAIR(2));
        }
        else if (cup[i] == 'T')
        {
            addch('T' | COLOR_PAIR(3));
            addch('T' | COLOR_PAIR(3));
        }
        else if (cup[i] == 'I')
        {
            addch('I' | COLOR_PAIR(4));
            addch('I' | COLOR_PAIR(4));
        }
        else if (cup[i] == 'J')
        {
            addch('J' | COLOR_PAIR(5));
            addch('J' | COLOR_PAIR(5));
        }
        else if (cup[i] == 'L')
        {
            addch('L' | COLOR_PAIR(6));
            addch('L' | COLOR_PAIR(6));
        }
        else if (cup[i] == 'Z')
        {
            addch('Z' | COLOR_PAIR(1));
            addch('Z' | COLOR_PAIR(1));
        }
        else if (cup[i] == 'S')
        {
            addch('S' | COLOR_PAIR(7));
            addch('S' | COLOR_PAIR(7));
        }
        else
        {
            printw(" %c", cup[i]);
        }
    }

    mvwprintw(stdscr, 5, 25, "SCORE: %d", score);

    int row = 6;
    int col = 25;

    for (size_t i = 0; i <= strlen(client_shape_info); ++i)
    {
        if (client_shape_info[i] != '\n')
        {
            mvwprintw(stdscr, row, col, "%c", client_shape_info[i]);

            if (col < 28)
            {
                if (client_shape_info[i] == 'O')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(2));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(2));
                }
                else if (client_shape_info[i] == 'T')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(3));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(3));
                }
                else if (client_shape_info[i] == 'I')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(4));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(4));
                }
                else if (client_shape_info[i] == 'J')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(5));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(5));
                }
                else if (client_shape_info[i] == 'L')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(6));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(6));
                }
                else if (client_shape_info[i] == 'Z')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(1));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(1));
                }
                else if (client_shape_info[i] == 'S')
                {
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(7));
                    col++;
                    mvwaddch(stdscr, row, col, ' ' | COLOR_PAIR(7));
                }
            }
            ++col;
        }
        else if (client_shape_info[i] == '\n')
        {
            ++row;
            col = 25;
        }
    }

    refresh();
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

    make_log("Client: ncurses initialized");
}

void nc_setup_colors(void)
{
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_pair(6, COLOR_RED, COLOR_RED);
    init_pair(7, COLOR_GREEN, COLOR_GREEN);
}

void nc_cleanup(void)
{
    endwin();
    make_log("Client: cleaning up ncurses");
}

void game_over_handler()
{
    game_over = true;
}

void get_terminal_size(unsigned int* buff_x, unsigned int* buff_y)
{
    struct winsize terminal_size;

    ioctl(0, TIOCGWINSZ, &terminal_size);
    *buff_x = terminal_size.ws_col;
    *buff_y = terminal_size.ws_row;
}