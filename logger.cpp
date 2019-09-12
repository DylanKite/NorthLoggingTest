#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"

logger::logger()
{
// initialize files? faster write times but at what cost?
    f_error.open("./error");
    f_warning.open("./warming");
    f_info.open("./info");
}
logger::~logger()
{
    //close files?
    f_error.close();
    f_warning.close();
    f_info.close();
}
int logger::log_message_error(string *client_ID, string* log_message)
{

}
int logger::log_message_warning(string *client_ID, string* log_message)
{

}
int logger::log_message_info(string *client_ID, string* log_message)
{

}
int logger::log_message(string client_ID, log_level selected_log_level, string log_message)
{
    switch(selected_log_level) {
    case ERROR:
    {

        break;
    }
    case WARNING:
    {
        break;
    }
    case INFO:
    {
        break;
    }
    }

}
