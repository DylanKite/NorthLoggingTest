#ifndef LOGGING_CLIENT_H
#define LOGGING_CLIENT_H

#endif // LOGGING_CLIENT_H
#include "logging_server.h"

class logging_client
{
public:
    logging_client();
    ~logging_client();

    void send_message(string message, string client_id, int selected_level);
private:
    void connect_to_socket();

    struct sockaddr_in serv_addr;
    int sock;
};
