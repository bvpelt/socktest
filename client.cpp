#include "tcpclient.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
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
// ./client -h <host> -p <port> -?
//
int main(int argc, char *argv[], char *envp[])
{

    TCPClient client;
    bool goOn = true;
    int retval = BS_SUCCESS;
    string inputString = "";

    string host = "localhost";
    string port = "1223";
    bool help = false;

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
            cerr << endl;
        }
    }

    cout << "Host: " << host << " port: " << port << endl;

    client.setHost(host);
    client.setPort(port);

    try
    {
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