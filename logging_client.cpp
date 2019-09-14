#include "logger.h"
#include "logging_server.h"
#include "logging_client.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>


using namespace std;

void logging_client::send_message(string message, string client_id, int selected_level)
{
    string to_send;
    to_send = to_string(selected_level) + ", " + client_id + ", " + message;
    cout << to_send;
    send(sock , to_send.c_str() , strlen(to_send.c_str()) , 0 );
}

void logging_client::connect_to_socket()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    cout << "connecting to socket\n";

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    cout << "connected\n";
}
logging_client::logging_client()
{
    connect_to_socket();
}
logging_client::~logging_client()
{

}

int main(int argc, char* argv[])
{
    logging_client client;
    client.send_message("this is a test", "Dylan", ERROR);
    return 1;
}
