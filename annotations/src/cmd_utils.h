#ifndef CMD_UTILS_H
#define CMD_UTILS_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

string execute_command(const char* cmd)
{
    // prepare structures to host data
    char buffer[128];
    string result = "";

    // open a pipe
    FILE* pipe = popen(cmd, "r");

    // if pipe couldn't be opened, throw an error
    if (!pipe)
        throw runtime_error("popen() failed!");

    // try to read from pipe
    try
    {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }

    }

    // throw an exeption if pipe couldn't be read
    catch (exception& e)
    {
        cout << "An error occurred while reading from pipe. " << endl << e.what() << endl;
        pclose(pipe);
        throw;
    }

    // close pipe
    pclose(pipe);

    // return executed command
    return result;
}

#endif // CMD_UTILS_H

