#ifndef LOGGING_CLIENT_H
#define LOGGING_CLIENT_H

#endif // LOGGING_CLIENT_H
#include "logging_server.h"

class logging_client
{
public:
    logging_client();
    ~logging_client();

    /*
     * send message is whats used to log an activity.
     * input variables:
     * -string message
     *      this is the log message that will be recoreded in the log
     * -string client_id
     *      this is the client ID that will be recorded. there can be no spaces
     *      or commas in the clientID
     * - log_level selected_level
     *      this is the selected log level the three levels are INFO, WARNING, ERROR
     */
    void send_message(string message, string client_id, log_level selected_level);

    /*
     * delete log sends a request to the server to delete the contents of the log file
     */
    void delete_log();

    /*
     *  request_dump requests the contents of the log file at the given log level and the above levels
     *  ex. if log level warning is selected it will collect the logs with the log level warning and error.
     */
    void request_dump(log_level level);

    /*
     * ui is used for testing the above functions
     */
    void ui();
private:
    void connect_to_socket();

    struct sockaddr_in serv_addr;
    int sock;
};
