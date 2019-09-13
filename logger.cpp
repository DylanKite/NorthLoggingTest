#include <fstream>
#include <string>
#include <iostream>

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "logger.h"

logger::logger()
{
// initialize files? faster write times but at what cost?
    f_log.open("./log", fstream::in | fstream::out | ios::app);
   // f_warning.open("./warming", ios::app);
  //  f_info.open("./info", ios::app);
}

logger::~logger()
{
    //close files?
    f_log.close();
}

/* lock file logic
 * check if the file size != 0
 * if it is empty add 1 char to the file to "lock"
 * exit 1
 * else
 * return 0
 */
int logger::set_lock()
{
    struct stat file_size;

    stat("./lock", &file_size);

    f_lock.open("./lock", fstream::in | fstream::out | ios::app | fstream::binary);

    if (file_size.st_size == 0)
    {
        cout <<"file is empty \n";
        f_lock << 1;
        f_lock.close();
        return 1;
    }
    cout << "file is not empty";
    f_lock.close();
    return 0;
}

/*unlock file logic
 * clear the contents of the file
 */
void logger::reset_lock()
{
    f_lock.open("./lock", fstream::out | fstream::trunc);
    f_lock << "";
    f_lock.close();
}

int logger::log_message_error(string *client_ID, string* log_message)
{
    if (f_log.is_open())
    {
        set_lock();
        f_log << "Timestamp: " << time(NULL) << " Level: Error " << "Client ID: " << *client_ID << " Message: " << *log_message << "\n";
        cout << "Timestamp: " << time(NULL) << " Client ID: " << *client_ID << " Message: " << *log_message << "\n";
        reset_lock();
    }
    else
    {
        cout << "unable to open file\n";
        return 0;
    }
    return 1;
}

int logger::log_message_warning(string *client_ID, string* log_message)
{
    return 1;
}

int logger::log_message_info(string *client_ID, string* log_message)
{
    return 1;
}

int logger::log_message(string client_ID, log_level selected_log_level, string log_message)
{
    switch(selected_log_level) {
    case ERROR:
    {
        log_message_error(&client_ID, &log_message);
        break;
    }
    case WARNING:
    {
        log_message_warning(&client_ID, &log_message);
        break;
    }
    case INFO:
    {
        log_message_info(&client_ID, &log_message);
        break;
    }
    }
    return 1;
}
