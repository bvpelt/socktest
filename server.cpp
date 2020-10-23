#include <iostream>
#include "tcpserver.h"
using namespace std;

int main(int argc, char *argv[], char *envp[])
{

    TCPServer server;

    int retval = BS_SUCCESS;
    retval = server.startUp();

    return retval;
}