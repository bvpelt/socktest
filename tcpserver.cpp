#include "tcpserver.h"
#include <iostream>
using namespace std;

TCPServer::TCPServer()
{
}

TCPServer::TCPServer(const char *portNumber)
{
    int len = strlen(portNumber);
    len = strnlen(portNumber, sizeof(port));
    strncpy(port, portNumber, len);
    port[len + 1] = '\0';
}

TCPServer::~TCPServer()
{
}

int TCPServer::startUp()
{
    int retval = BS_SUCCESS;

    retval = bssocket.getAddrInfo(port, AF_INET, AI_PASSIVE); // AI_PASSIVE gives structure for server

    if (BS_SUCCESS == retval)
    {
        bssocket.getAddrInfo();
    }

    if (BS_SUCCESS == retval)
    {
        retval = bssocket.createsock();

        if (retval == BS_ERROR)
        {
            cout << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
        }
    }

    if (retval > BS_SUCCESS)
    {
        retval = bssocket.bindsock();
        if (retval == BS_ERROR)
        {
            cout << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
        }
    }

    if (retval == BS_SUCCESS)
    {
        retval = bssocket.listensock();
        if (retval == BS_ERROR)
        {
            cout << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
        }
    }

    if (retval == BS_SUCCESS)
    {
        int clientConnection = bssocket.acceptsock();
        if (clientConnection == BS_ERROR)
        {
            cout << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
        }
        else
        {
            int bytesRead = bssocket.readsock(clientConnection);
            if (retval == BS_ERROR)
            {
                cout << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
            }
        }
    }

    return retval;
}