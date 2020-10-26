#include "tcpclient.h"
#include <string.h>
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

    try
    {
        retval = bssocket.connectsock(hostname, port); // connect and creat socket
    }
    catch (BSException ex)
    {
        cerr << "Exception occured " << ex.what() << endl;
    }
    return retval;
}

int TCPClient::write(const string buffer, int flags)
{
    void *bufferptr = (void *)buffer.data();
    int len = buffer.length() + 1;
    int retval = write(bufferptr, len, flags);

    return retval;
}

int TCPClient::write(const void *buffer, int len, int flags)
{
    int retval = 0;

    try
    {
        retval = bssocket.write(buffer, len, flags);
    }
    catch (BSException ex)
    {
        cerr << "Exception occured " << ex.what() << endl;
    }
    catch (exception ex)
    {
        cerr << "Exception occured " << ex.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception" << endl;
    }
    return retval;
}