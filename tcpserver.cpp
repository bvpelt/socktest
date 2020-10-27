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

        retval = bssocket.getAddrInfo(port.c_str(), AF_INET, AI_PASSIVE); // AI_PASSIVE gives structure for server

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

            int bytesRead = 1;

            while (bytesRead > 0)
            {
                bytesRead = bssocket.readsock(clientConnection);

                string clientmsg = bssocket.getBuffer();
                cout << "Received: " << clientmsg << endl;
                if (clientmsg == "STOP")
                {
                    bytesRead = -2;
                }
            }
        }
    }
    catch (BSException ex)
    {
        cerr << "Exception occured " << ex.what() << endl;
    }
    return retval;
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