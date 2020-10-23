#include "bssocket.h"

class TCPClient
{
private:
    BSSocket bssocket;
    char port[6] = "1223";
    char hostname[254] = "localhost";

public:
    TCPClient();
    TCPClient(const char *hostname, const char *portNumber);
    ~TCPClient();

    int startUp();

    int write(const void *buffer, int len, int flags);
};