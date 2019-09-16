#include "logging_server.h"
#include "logging_client.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define MESSAGE_SIZE 1024

using namespace std;

void logging_client::delete_log()
{
    send(sock, "4", 2, 0);
}

void logging_client::request_dump(log_level level)
{
    string to_send;
    char message[MESSAGE_SIZE];
    int n_bytes_read = 0;
    string recieved_message("");
    string string_message("");
    bool is_end_of_file = false;
    switch(level)
    {
    case INFO:
    {
        to_send = "3, 0";
    }
    case WARNING:
    {
        to_send = "3, 1";
    }
    case ERROR:
    {
        to_send = "3, 2";
    }
    }

    send(sock, to_send.c_str(), (strlen(to_send.c_str()) + 1), 0);
    //TODO: add breakout incase EOF gets missed to prevent getting stuck in infinite loop
    while(is_end_of_file == false)
    {
        n_bytes_read = read(sock, message, MESSAGE_SIZE);
        string_message = message;
        if (n_bytes_read != 0)
        {
            if(string_message.find("EOF") != string::npos)
            {
                is_end_of_file = true;
            }
            else
            {
                recieved_message += message;
                recieved_message += "\n";
            }
        }
    }
    cout << recieved_message;
}

void logging_client::send_message(string message, string client_id, log_level selected_level)
{
    string to_send;
    to_send = to_string((int)selected_level) + ", " + client_id + ", " + message;
    cout << to_send;
    // +1 adds length for null terminator
    send(sock, to_send.c_str(), (to_send.size() + 1), 0);
}

void logging_client::connect_to_socket()
{
    int error;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    cout << "connecting to socket\n";

    error = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (error != 0)
    {
        cout << error << "STDERR: " << stderr << "\n";
        exit(-1);
    }
    cout << "connected\n";
}

logging_client::logging_client()
{
    connect_to_socket();
}

logging_client::~logging_client()
{
    shutdown(sock, 2);
}

void logging_client::ui()
{
    static log_level level_to_send = INFO;
    static string client_id_to_send = "North";
    string selected_option;
    char user_message[MESSAGE_SIZE];
    for(;;)
    {
        cout << "logging program\n";
        cout << "options:\n";
        cout << "1 send message\n";
        cout << "2 read logs\n";
        cout << "3 clear logs\n";
        cout << "4 change CLIENT ID [default: North]" << " [current: " << client_id_to_send << "]\n";
        cout << "5 change log level [default: INFO]";
        switch(level_to_send)
        {
        case INFO:
        {
            cout << " [current: INFO]\n";
            break;
        }
        case WARNING:
        {
            cout << " [current: WARNING]\n";
            break;
        }
        case ERROR:
        {
            cout << " [current: ERROR]\n";
            break;
        }
        }
        cout << "6 exit\n";
        cin >> selected_option;
        if (selected_option.compare("1") == 0)
        {
            cout << "what message would you like to send\n";
            cin.ignore();
            cin.getline(user_message, MESSAGE_SIZE);
            selected_option = user_message;
            send_message(selected_option, client_id_to_send, level_to_send);
        }
        else if(selected_option.compare("2") == 0)
        {
            request_dump(level_to_send);
        }
        else if(selected_option.compare("3") == 0)
        {
            cout << "clearing logs\n";
            delete_log();
        }
        else if(selected_option.compare("4") == 0)
        {
            cout << "New CLIENT ID:\n";
            cin >> client_id_to_send;
        }
        else if(selected_option.compare("5") == 0)
        {
            cout << "New Log Level: \n";
            cout << "1 INFO\n";
            cout << "2 WARNING\n";
            cout << "3 ERROR\n";
            cin >> selected_option;
            if (selected_option.compare("1") == 0)
            {
                cout << "INFO selected\n";
                level_to_send = INFO;
            }
            else if (selected_option.compare("2") == 0)
            {
                cout << "WARNING selected\n";
                level_to_send = WARNING;
            }
            else if (selected_option.compare("3") == 0)
            {
                cout << "ERROR selected\n";
                level_to_send = ERROR;
            }
            else
            {
                cout << "invalid choice selected \n";
            }
        }
        else if(selected_option.compare("6") == 0)
        {
            exit(0);
        }
        else
        {
            cout << "invalid option";
        }
    }
}
int main(int argc, char* argv[])
{
    logging_client client;
    client.ui();
    return 1;
}
