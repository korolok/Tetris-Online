#include "util.h"

char path_to_root_dir[FILENAME_MAX] = {0};

void set_random_seed(void)
{
    srand((int)time(NULL));
}

int generate_random_number(int minval, int maxval)
{
    return (rand() % (maxval - minval + 1)) + minval;
}

void make_log(const char* log_message)
{
    time_t time_info = time(NULL);
    struct tm curr_time = *localtime(&time_info);
    
    char path_to_log[FILENAME_MAX] = {0};
    concat_to_root_path(LOG_FILE_NAME, path_to_log);

    FILE *file_pointer = fopen(path_to_log, "a");
    if(file_pointer != NULL)
    {
        fprintf(
            file_pointer, 
            "[%02d:%02d:%02d] %s\n", 
            curr_time.tm_hour, curr_time.tm_min, curr_time.tm_sec, log_message
        );
    }
    fclose(file_pointer);
}

void concat_to_root_path(char *path_to_concat , char * full_path_destination_buff)
{
    strcat(full_path_destination_buff, path_to_root_dir);
    strcat(full_path_destination_buff, path_to_concat);
}

void set_path_to_root(char *path)
{
    if (path[0] == '.')
    {
        for (size_t i = 2; i < strlen(path) - 6; ++i)
        {
            path_to_root_dir[i - 2] = path[i];
        }
    }
    else
    {
        for (size_t i = 0; i < strlen(path) - 6; ++i)
        {
            path_to_root_dir[i] = path[i];
        }
    }
}

long long get_timestamp_in_milliseconds(void)
{
    struct timeval time_value;

    gettimeofday(&time_value, NULL);
    long long milliseconds = time_value.tv_sec * 1000LL + time_value.tv_usec / 1000;
    
    return milliseconds;
}

bool get_port_from_file(int* port_buffer) 
{
    char path_to_config[FILENAME_MAX] = {0};
    concat_to_root_path("config.cfg", path_to_config);

    FILE *file_pointer = fopen(path_to_config, "r");

    if (file_pointer == NULL)
    {   
        make_log("Net: Failed to open config file");
        return false;  
    } 

    fscanf(file_pointer, "%d", port_buffer);

    return true;
}