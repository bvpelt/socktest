#ifndef BS_IOSTREAM_INCLUDED
#include <iostream>
#define BS_IOSTREAM_INCLUDED 1
#endif

#ifndef BS_TCPSERVER_INCLUDED
#include "tcpserver.h"
#define BS_TCPSERVER_INCLUDED 1
#endif

using namespace std;

int usage(const char *name)
{
    cout << "Usage " << name << " -p <port> -u" << endl;
    cout << " -p <port> port to connect to" << endl;
    cout << " -d        show debug messages" << endl;
    cout << " -4" << endl;
    cout << " -6" << endl;
    cout << " -u         this message" << endl;
    return BS_SUCCESS;
}

int socketHandler(BSSocket *socket)
{
    int retval = BS_SUCCESS;
    try
    {
        int bytesRead = 1;

        while (bytesRead > 0)
        {
            bytesRead = socket->readit();

            string clientmsg = socket->getBuffer();
            cout << "Received: " << clientmsg << endl;
            if (clientmsg == "STOP")
            {
                bytesRead = -2;
            }
        }
    }
    catch (...)
    {
        delete socket;
        retval = BS_ERROR;
    }

    return retval;
}
//
// usage
// ./server -p <port> -u
//
int main(int argc, char *argv[], char *envp[])
{
    TCPServer server;

    int retval = BS_SUCCESS;
    int version = 4;
    string port = "1223";
    bool debug = false;

    int c;
    while ((c = getopt(argc, argv, "dp:u46")) != -1)
    {
        switch (c)
        {
        case 'd':
            debug = true;
            break;

        case 'p':
            port = optarg;
            break;

        case '4':
            version = 4;
            break;

        case '6':
            version = 6;
            break;

        case 'u':
            usage(argv[0]);
            return BS_SUCCESS;
            break;

        default:
            cerr << "Unknown argument: " << (char)c << ", ignored!" << endl;
        }
    }

    cout << "port: " << port << endl;

    try
    {
        server.setPort(port);
        server.setIPVersion(version);
        server.setDebug(debug);
        server.setSocketHandler(socketHandler);

        retval = server.startUp();
    }
    catch (BSException ex)
    {
        cerr << ex.what() << endl;
    }

    return retval;
}