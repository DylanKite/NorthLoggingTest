#ifndef LOGGER_H
#define LOGGER_H
#include <string.h>
#include <fstream>
class logger
{
public:
    logger();
    enum log_level
    {
        ERROR,
        WARNING,
        INFO
    };
    int log_message(string client_ID, log_level selected_log_level, string log_message);
    void dump_logs(log_level selected_log_level);
    void clear_logs();
private:
    fstream f_error;
    fstream f_warning;
    fstream f_info;
    int log_message_error(string *client_ID, string* log_message);
    int log_message_warning(string *client_ID, string* log_message);
    int log_message_info(string *client_ID, string* log_message);
};

#endif // LOGGER_H

