//#include <stdio.h>
//#include <stdlib.h>

#include <iostream>

#include "logger.h"

using namespace std;

int main(int argc, char* argv[]) {
    logger log_test;
    log_test.log_message("testing", log_test.ERROR, "this is a test");
    cout << "log message: afjaskl d\n";
    //~log_test();
    return 1;
}
