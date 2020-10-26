#include "tcpclient.h"
#include <string.h>
#include <iostream>
using namespace std;

TCPClient::TCPClient()
{
}

TCPClient::TCPClient(const string hostname, const string portNumber)
{
    this->hostname = hostname;
    this->port = portNumber;
}

TCPClient::~TCPClient()
{
}

int TCPClient::startUp()
{
    int retval = BS_SUCCESS;

    try
    {
        retval = bssocket.connectsock(hostname.c_str(), port.c_str()); // connect and creat socket
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

void TCPClient::setHost(const string host)
{
    hostname = host;
}
const string TCPClient::getHost()
{
    return hostname;
}

void TCPClient::setPort(const string port)
{
    this->port = port;
}
const string TCPClient::getPort()
{
    return port;
}