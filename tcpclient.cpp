#ifndef BS_TCPCLIENT_INCLUDED
#include "tcpclient.h"
#define BS_TCPCLIENT_INCLUDED 1
#endif

#ifndef BS_STRING_H_INCLUDED
#include <string.h>
#define BS_STRING_H_INCLUDED 1
#endif

#ifndef BS_IOSTREAM_INCLUDED
#include <iostream>
#define BS_IOSTREAM_INCLUDED 1
#endif

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
        bssocket.setDebug(debug);
        retval = bssocket.connectsock(hostname.c_str(), port.c_str(), version); // connect and creat socket
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

void TCPClient::setDebug(const bool debug)
{
    this->debug = debug;
}
bool TCPClient::getDebug()
{
    return debug;
}

void TCPClient::setPort(const string port)
{
    int len = port.length();
    bool valid = true;
    int i = 0;

    while (valid && (i < len))
    {
        valid = isdigit(port[i]);
        i++;
    }

    if (!valid)
    {
        throw BSException("Invalid number: " + port, __FILE__, __LINE__);
    }

    this->port = port;
}

const string TCPClient::getPort()
{
    return port;
}

void TCPClient::setVersion(const int version)
{

    if ((version == 4) || (version == 6))
    {
        this->version = version;
    }
    else
    {
        throw BSException("Invalid inet version only 4 or 6 allowed", __FILE__, __LINE__);
    }
}

const int TCPClient::getVersion()
{
    return version;
}