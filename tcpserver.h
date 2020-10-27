#ifndef BSSOCKET_INCLUDED
#include "bssocket.h"
#define BSSOCKET_INCLUDED 1
#endif

#ifndef BS_STRING_INCLUDED
#include <string>
#define BS_STRING_INCLUDED 1
#endif

using namespace std;

class TCPServer
{
private:
    BSSocket bssocket;
    string port = "1223";
    int ipver = 4; // possible values 4 of 6

public:
    TCPServer();
    TCPServer(const string portNumber);
    ~TCPServer();

    int startUp();

    void setPort(const string port);
    string getPort();

    void setIPVersion(const int version);
    int getIPVersion();
};