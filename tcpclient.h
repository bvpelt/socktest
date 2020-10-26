#ifndef BS_STRING_INCLUDED
#include <string>
#define BS_STRING_INCLUDED 1
#endif
using namespace std;

#include "bssocket.h"

class TCPClient
{
private:
    BSSocket bssocket;
    char port[6] = "1223";
    char hostname[254] = "127.0.0.1";

public:
    TCPClient();
    TCPClient(const char *hostname, const char *portNumber);
    ~TCPClient();

    int startUp();

    int write(const string buffer, int flags);
    int write(const void *buffer, int len, int flags);
};