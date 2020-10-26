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
    string port = "1223";
    string hostname = "localhost";

public:
    TCPClient();
    TCPClient(const string hostname, const string portNumber);
    ~TCPClient();

    int startUp();

    int write(const string buffer, int flags);
    int write(const void *buffer, int len, int flags);

    void setHost(const string host);
    const string getHost();

    void setPort(const string port);
    const string getPort();
};