#include "logging_server.h"

#include <fstream>
#include <string>
#include <iostream>
#include <mutex>

#include <time.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

// may need to install bsd library this is used for strlcpy
#include <bsd/string.h>

#define MESSAGE_SIZE 1024
using namespace std;

int logging_server::parse_socket_message(char *incoming_message, log_level *current_level, string *client_id, string *message)
{
    char selected_log_level;
    char *working_message_client_id;
    char *working_message_message;
    char *client_id_ptr = 0;

    int message_len_client_id = 0;
    int message_len_message = 0;
    int client_id_len = 0;
    int level_or_command = 0;

    // since the log level is always the first digit in the string it can just be taken as a fixed value
    selected_log_level = incoming_message[0];
    level_or_command = atoi(&selected_log_level);

    switch (level_or_command)
    {
    case 0:
    {
        *current_level = INFO;
        break;
    }
    case 1:
    {
        *current_level = WARNING;
        break;
    }
    case 2:
    {
        *current_level = ERROR;
        break;
    }
    case 3: // dump log
    {
        return level_or_command;
    }
    case 4: // clear log
    {
        return level_or_command;
    }
    default:
    {
        cout << "log level error";
        return -1;
    }
    }

    client_id_ptr = (char*)calloc(MESSAGE_SIZE, sizeof(char));

    //moves pointer to the start of the client id
    working_message_client_id = strstr(incoming_message, ",");
    working_message_client_id += 2;

    //moves pointer to the start of the message
    working_message_message = strstr(working_message_client_id, ",");
    working_message_message += 2;

    message_len_client_id = strlen(working_message_client_id);
    message_len_message = strlen(working_message_message);

    client_id_len = message_len_client_id - message_len_message;

    // 1 is subtracted from the client_id_len to remove the comma
    strlcpy(client_id_ptr, working_message_client_id, (client_id_len - 1));

    *client_id = client_id_ptr;
    *message = working_message_message;

    free(client_id_ptr);
    return level_or_command;
}

void logging_server::dump_logs()
{
    fstream f_log;
    f_log.open("./log", fstream::in);
    f_log.close();
}

void logging_server::clear_logs()
{
    fstream f_log;
    f_log.open("./log", fstream::out | ios::trunc);
    f_log << "";
    f_log.close();
}

/*
 * parse the incoming message, to determine its log level.
 * seperate out the client ID
 * seperate out the Message
 * log message in the following format:
 * timestamp: POSIX TIME
 * Log Level: Info, Warning, Error
 * Message
 *
 * lock mutex
 * write log to log file
 * unlock mutex
 *
 * return
 */
int logging_server::log_message(char *to_log)
{
    string client_id;
    string message;

    fstream f_log;
    log_level message_level;
    int command_or_log;

    command_or_log = parse_socket_message(to_log, &message_level, &client_id, &message);

    switch(command_or_log)
    {
    case 3: // dump log
    {
        dump_logs();
        return 3;
        break;
    }
    case 4: // clear log
    {
        clear_logs();
        return 4;
        break;
    }
    }
    f_log.open("./log", fstream::out | ios::app);

    write_lock.lock();
    switch(message_level)
    {
    case(INFO):
    {
        f_log << "Timestamp: " << time(NULL) << " Level: Info " << "Client ID: " << client_id << " Message: " << message << "\n";
        break;
    }
    case(WARNING):
    {
        f_log << "Timestamp: " << time(NULL) << " Level: Warning " << "Client ID: " << client_id << " Message: " << message << "\n";
        break;
    }
    case(ERROR):
    {
        f_log << "Timestamp: " << time(NULL) << " Level: Error " << "Client ID: " << client_id << " Message: " << message << "\n";
        break;
    }
    }
    write_lock.unlock();

    f_log.close();
    return 0;
}

/*
 * the socket message will be in the following format
 * single digit (ENUM) to reperesetn the log level
 * client ID
 * Message
 *
 * all incoming information will be comma seperated
 * ex.
 * 1, Dylan, this is an example of the message
 */
int logging_server::read_socket()
{
    int n_bytes_read;
    char message[MESSAGE_SIZE];

    n_bytes_read = read( new_socket , message, MESSAGE_SIZE);
    if (!n_bytes_read)
    {
        log_message(message);
        return 1;
    }
    return 0;
}

/*
 * current implementation only allows for one client
 * TODO: allow for multiple clients
 */
int logging_server::start_server()
{
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    return 1;
}

logging_server::logging_server()
{
    cout << "starting server\n";
    start_server();
    cout << "client connected\n";

    //infinate loop to read the socket forever
    for(;;)
    {
        read_socket();
    }
}
logging_server::~logging_server()
{

}

int main()
{
    logging_server start_log;
    //start_log();
    return 1;
}
