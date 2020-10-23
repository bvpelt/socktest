#include "tcpclient.h"

#include <iostream>
using namespace std;

TCPClient::TCPClient()
{
}

TCPClient::TCPClient(const char *hostname, const char *portNumber)
{
    int len = strnlen(portNumber, sizeof(port));
    strncpy(port, portNumber, len);
    port[len + 1] = '\0';

    len = strnlen(hostname, sizeof(this->hostname));
    strncpy(this->hostname, hostname, len);
    this->hostname[len + 1] = '\0';
}

TCPClient::~TCPClient()
{
}

int TCPClient::startUp()
{
    int retval = BS_SUCCESS;

    retval = bssocket.connect(hostname, port);
    if (retval == BS_ERROR)
    {
        cerr << "Error during connect: " << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
    }
    return retval;
}

int TCPClient::write(const void *buffer, int len, int flags)
{
    int retval = 0;

    retval = bssocket.writesock(buffer, len, flags);
    if (retval == BS_ERROR)
    {
        cerr << "Error during connect: " << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
    }
    return retval;
}