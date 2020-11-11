#ifndef BS_IOSTREAM_INCLUDED
#include <iostream>
#define BS_IOSTREAM_INCLUDED 1
#endif

#ifndef BS_TCPSERVER_INCLUDED
#include "tcpserver.h"
#define BS_TCPSERVER_INCLUDED 1
#endif

#include <signal.h>
#include <sys/time.h>

using namespace std;

static const int success = 0;
static const int error = -1;
bool goOn = true;

void my_function(int sig)
{                 // can be called asynchronously
    goOn = false; // set flag
}

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

struct parameterType
{
    bool *goOn;
    LinkedList<Work> *linkedList;
    int threadid;
    pthread_mutex_t *output_mutex;
};

//int socketHandler(BSSocket *socket)
void *socketHandler(void *arg)
{
    parameterType *parameter = (parameterType *)arg;
    bool *goOn = parameter->goOn;
    LinkedList<Work> *linkedlist = parameter->linkedList;
    int threadid = parameter->threadid;

    Work *work = linkedlist->getData();
    int clientConnection = work->getConnection();

    BSSocket *socket = new BSSocket(clientConnection);
    socket->setBacklog(8);

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

    return ((void *)&success);
}
//
// usage
// ./server -p <port> -d -4 -6 -u
//
int main(int argc, char *argv[], char *envp[])
{
    // Register signals
    signal(SIGINT, my_function);

    TCPServer server = TCPServer(&goOn);

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