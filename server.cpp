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
    cout << " -4" << endl;
    cout << " -6" << endl;
    cout << " -u         this message" << endl;
    return BS_SUCCESS;
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

    int c;
    while ((c = getopt(argc, argv, "p:u46")) != -1)
    {
        switch (c)
        {
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

    cout << " port: " << port << endl;

    try
    {
        server.setPort(port);
        server.setIPVersion(version);
        retval = server.startUp();
    }
    catch (BSException ex)
    {
        cerr << "Error (" << ex.getErrorCode() << "): " << ex.getErrorMessage() << " - " << ex.getSourceFile() << "(" << ex.getLineNo() << ")" << endl;
    }

    return retval;
}