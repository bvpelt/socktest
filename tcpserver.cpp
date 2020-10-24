#include "tcpserver.h"
#include <iostream>
#include <string>
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

    try
    {

        retval = bssocket.getAddrInfo(port, AF_INET, AI_PASSIVE); // AI_PASSIVE gives structure for server

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