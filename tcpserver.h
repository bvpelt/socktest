#ifndef BSSOCKET_INCLUDED
#include "bssocket.h"
#define BSSOCKET_INCLUDED 1
#endif

class TCPServer
{
private:
    BSSocket bssocket;
    char port[6] = "1223";

public:
    TCPServer();
    TCPServer(const char *portNumber);
    ~TCPServer();

    int startUp();
};