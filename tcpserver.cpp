#ifndef BS_IOSTREAM_INCLUDED
#include <iostream>
#define BS_IOSTREAM_INCLUDED 1
#endif

#ifndef BS_STRING_INCLUDED
#include <string>
#define BS_STRING_INCLUDED 1
#endif

#ifndef BS_TCPSERVER_INCLUDED
#include "tcpserver.h"
#define BS_TCPSERVER_INCLUDED 1
#endif

using namespace std;

TCPServer::TCPServer()
{
}

TCPServer::TCPServer(const string portNumber)
{
    this->port = portNumber;
}

TCPServer::~TCPServer()
{
}

int TCPServer::startUp()
{
    int retval = BS_SUCCESS;

    try
    {
        bssocket.setDebug(debug);

        if (ipver == 4)
        {
            retval = bssocket.getAddrInfo(port.c_str(), AF_INET, AI_PASSIVE); // AI_PASSIVE gives structure for server
        }
        if (ipver == 6)
        {
            retval = bssocket.getAddrInfo(port.c_str(), AF_INET6, AI_PASSIVE); // AI_PASSIVE gives structure for server
        }
        if (BS_SUCCESS == retval)
        {
            bssocket.getAddrInfo();
        }

        if (BS_SUCCESS == retval)
        {
            retval = bssocket.createsock();
        }

        if (retval > BS_SUCCESS)
        {
            retval = bssocket.bindsock();
        }

        if (retval == BS_SUCCESS)
        {
            retval = bssocket.listensock();
        }

        if (retval == BS_SUCCESS)
        {
            int clientConnection = bssocket.acceptsock();

            if (socketHandler != NULL)
            {
                BSSocket *socket = new BSSocket(clientConnection);
                socket->setBacklog(bssocket.getBacklog());

                retval = socketHandler(socket);
            }
        }
    }
    catch (BSException ex)
    {
        cerr << "Exception occured " << ex.what() << endl;
    }
    return retval;
}

void TCPServer::setDebug(const bool debug)
{
    this->debug = debug;
}

bool TCPServer::getDebug()
{
    return debug;
}

void TCPServer::setPort(const string port)
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

string TCPServer::getPort()
{
    return port;
}

void TCPServer::setIPVersion(const int version)
{
    if ((version == 4) || (version == 6))
    {
        this->ipver = version;
    }
    else
    {
        throw BSException("Unsupported version only 4 or 6 allowed", __FILE__, __LINE__);
    }
}

int TCPServer::getIPVersion()
{
    return this->ipver;
}

void TCPServer::setSocketHandler(const SocketHandlerType handler)
{
    this->socketHandler = handler;
}
SocketHandlerType *TCPServer::getSocketHandler()
{
    return this->socketHandler;
}