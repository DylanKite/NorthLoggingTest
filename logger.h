#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <fstream>

//using std::string;
//using std::fstream;
/* conditions to test for:
 * - normal write (all three levels)
 * - normal read (all three levels)
 * - normal empty (all three levels)
 * - concurrent write
 * - concurrent read
 * - concurrent empty
 */
using namespace std;
class logger
{
public:
    logger();
    ~logger();
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
    fstream f_log; //log file
    fstream f_lock; //lock file

    int log_message_error(string *client_ID, string* log_message);
    int log_message_warning(string *client_ID, string* log_message);
    int log_message_info(string *client_ID, string* log_message);

    int set_lock();
    void reset_lock();
};

#endif // LOGGER_H

