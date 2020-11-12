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
    cout << "Received ";
    switch (sig)
    {
    case        // Register signals
        SIGINT: // ctrl c interrupt
        cout << "SIGINT";
        break;
    case SIGHUP:
        cout << "SIGINT";
        break;
    case SIGQUIT: // ctrl \ quit
        cout << "SIGINT";
        break;
    case SIGABRT:
        cout << "SIGINT";
        break;
    case SIGKILL:
        cout << "SIGINT";
        break;
    case SIGPIPE:
        cout << "SIGINT";
        break;
    case SIGTERM:
        cout << "SIGINT";
        break;
    case SIGSTOP:
        cout << "SIGINT";
        break;
    case SIGTSTP: // ctrl z suspend
        cout << "SIGINT";
        break;
    default:
        cout << sig;
        break;
    }

    cout << " recieved - shutting down, wait 5 seconds" << endl;
    sleep(5); // give threads change to stop

    exit(sig);
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
        int bytesRead = 0;

        while (*goOn)
        {
            while (*goOn && ((bytesRead == -2) || (bytesRead == 0)))
            {
                bytesRead = socket->readit();
            }

            if (bytesRead > 0)
            {
                string clientmsg = socket->getBuffer();
                cout << "Received: " << clientmsg << endl;
                if (clientmsg == "STOP")
                {
                    bytesRead = -2;
                }
                else
                {
                    bytesRead = 0; // handled message
                }
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
    signal(SIGINT, my_function); // ctrl c interrupt
    signal(SIGHUP, my_function);
    signal(SIGQUIT, my_function); // ctrl \ quit
    signal(SIGABRT, my_function);
    signal(SIGKILL, my_function);
    signal(SIGPIPE, my_function);
    signal(SIGTERM, my_function);
    signal(SIGSTOP, my_function);
    // signal(SIGINFO, my_function); // ctrl t info
    signal(SIGTSTP, my_function); // ctrl z suspend

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