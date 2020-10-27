#ifndef BS_STDIO_INCLUDED
#include <stdio.h>
#define BS_STDIO_INCLUDED 1
#endif

#ifndef BS_UNISTD_INCLUDED
#include <unistd.h>
#define BS_UNISTD_INCLUDED 1
#endif

#ifndef BS_IOSTREAM_INCLUDED
#include <iostream>
#define BS_IOSTREAM_INCLUDED 1
#endif

#ifndef BS_TCPCLIENT_INCLUDED
#define BS_TCPCLIENT_INCLUDED 1
#include "tcpclient.h"
#endif

using namespace std;

int usage(const char *name)
{
    cout << "Usage " << name << " -h <host> -p <port> -u" << endl;
    cout << " -h <host> host to connect to" << endl;
    cout << " -p <port> port to connect to" << endl;
    cout << " -u         this message" << endl;
    return BS_SUCCESS;
}

//
// usage
// ./client -h <host> -p <port> -u
//
int main(int argc, char *argv[], char *envp[])
{

    TCPClient client;
    bool goOn = true;
    int retval = BS_SUCCESS;
    string inputString = "";

    string host = "localhost";
    string port = "1223";

    int c;
    while ((c = getopt(argc, argv, "h:p:u")) != -1)
    {
        switch (c)
        {
        case 'h':
            host = optarg;
            break;

        case 'p':
            port = optarg;
            break;

        case 'u':
            usage(argv[0]);
            return BS_SUCCESS;
            break;

        default:
            cerr << "Unknown argument: " << (char)c << ", ignored!" << endl;
        }
    }

    cout << "Host: " << host << " port: " << port << endl;

    try
    {
        client.setHost(host);
        client.setPort(port);
        retval = client.startUp();

        while (goOn)
        {
            cout << "Input: ";
            getline(cin, inputString);

            cout << "Received: " << inputString << endl;

            client.write(inputString, 0);

            if (inputString.length() >= 4)
            {
                goOn = (inputString.substr(0, 4) != "STOP");
            }
        }
    }
    catch (BSException ex)
    {
        cerr << "Error (" << ex.getErrorCode() << "): " << ex.getErrorMessage() << " - " << ex.getSourceFile() << "(" << ex.getLineNo() << ")" << endl;
    }

    return retval;
}