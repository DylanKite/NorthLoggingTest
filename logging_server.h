#ifndef LOGGING_SERVER_H
#define LOGGING_SERVER_H

#include <string>
#include <mutex>

#include <netinet/in.h>

#define PORT 8080
enum log_level
{
    INFO,
    WARNING,
    ERROR
};
using namespace std;

class logging_server
{
public:
    logging_server();
    ~logging_server();

private:
    void send_logs(log_level determined_level);
    int log_message(char *to_log);
    void clear_logs();
    int start_server();
    int read_socket();
    int parse_socket_message(char *incoming_message, log_level *current_level, string *client_id, string *message);
    int parse_log_level(char *level);
    void dump_logs(char *level);

    int server_fd;
    int new_socket;
    mutex write_lock;

    sockaddr_in address;

};

#endif // LOGGING_SERVER_H
