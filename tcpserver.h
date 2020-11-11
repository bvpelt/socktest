#ifndef BSSOCKET_INCLUDED
#include "bssocket.h"
#define BSSOCKET_INCLUDED 1
#endif

#ifndef BS_STRING_INCLUDED
#include <string>
#define BS_STRING_INCLUDED 1
#endif

#ifndef LINKEDLIST_INCLUDED
#include "linkedlist.h"
#define LINKEDLIST_INCLUDED 1
#endif

using namespace std;

// typedef int SocketHandlerType(BSSocket *);
typedef void *SocketHandlerType(void *arg);

class TCPServer
{
private:
    BSSocket bssocket;
    bool *goOn;
    string port = "1223";
    int ipver = 4; // possible values 4 of 6
    bool debug = false;
    SocketHandlerType *socketHandler = NULL;
    LinkedList<Work> worklist;
    int threadid;

public:
    TCPServer();
    TCPServer(bool *goOn);
    TCPServer(const string portNumber, bool *goOn);
    ~TCPServer();

    int startUp();

    void setDebug(const bool debug);
    bool getDebug();

    void setPort(const string port);
    string getPort();

    void setIPVersion(const int version);
    int getIPVersion();

    void setGoOn(bool *goOn);
    bool *getGoOn();

    void setSocketHandler(const SocketHandlerType handler);
    SocketHandlerType *getSocketHandler();
};