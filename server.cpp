#include <iostream>
#include "tcpserver.h"
using namespace std;

int main(int argc, char *argv[], char *envp[])
{

    TCPServer server;

    int retval = BS_SUCCESS;

    try
    {
        retval = server.startUp();
    }
    catch (BSException ex)
    {
        cerr << "Error (" << ex.getErrorCode() << "): " << ex.getErrorMessage() << " - " << ex.getSourceFile() << "(" << ex.getLineNo() << ")" << endl;
    }

    return retval;
}