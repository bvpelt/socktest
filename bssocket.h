#ifndef BS_ERRNO_INCLUDED
#include <errno.h>
#define BS_ERRNO_INCLUDED 1
#endif

#ifndef BS_SYS_TYPES_INCLUDED
#include <sys/types.h>
#define BS_SYS_TYPES_INCLUDED 1
#endif

#ifndef BS_SYS_SOCKET_INCLUDED
#include <sys/socket.h>
#define BS_SYS_SOCKET_INCLUDED 1
#endif

#ifndef BS_NETDB_INCLUDED
#include <netdb.h>
#define BS_NETDB_INCLUDED 1
#endif

#ifndef BS_ARPA_INET_INCLUDED
#include <arpa/inet.h>
#define BS_ARPA_INET_INCLUDED 1
#endif

#ifndef BS_STRING_H_INCLUDED
#include <string.h>
#define BS_STRING_H_INCLUDED 1
#endif

#ifndef BS_STDIO_INCLUDED
#include <stdio.h>
#define BS_STDIO_INCLUDED 1
#endif

#ifndef BS_UNISTD_INCLUDED
#include <unistd.h>
#define BS_UNISTD_INCLUDED 1
#endif

#ifndef BS_STRING_INCLUDED
#include <string>
#define BS_STRING_INCLUDED 1
#endif

#ifndef BS_BSEXCEPTION_INCLUDED
#include "bsexception.h"
#define BS_BSEXCEPTION_INCLUDED 1
#endif

using namespace std;

#define BS_MAX_ERROR_SIZE 256
#define BS_ERROR -1
#define BS_SUCCESS 0

class BSSocket
{
private:
    int sockfd;
    bool showDebug = false;
    int backlog = 0; // number of connections on incoming queue
    char errmsg[BS_MAX_ERROR_SIZE] = "";
    char buffer[1024] = {0};
    addrinfo adresinfo, *res;

    bool validateDomain(const int domain);
    bool validateType(const int type);
    bool validateProtocol(const int protocol);
    int closeExistingSocket();
    void dbgMsg(const string msg);
    string getInet4Adres();
    string getInet6Adres();

public:
    BSSocket();
    BSSocket(const int clientConnection);
    ~BSSocket();
    int createsock();
    int bindsock();
    int listensock();
    int acceptsock();
    int readit();
    int readsock(const int clientConnection);
    string getBuffer();
    int writesock(const int connection, const void *buffer, const int len, const int flags);
    int write(const void *buffer, const int len, const int flags);
    int create(const int domain, const int type, const int protocol);
    int setOptions(int level, int optname, const void *optval, int optlen);

    int connectsock(const char *host, const char *port, const int version);

    int getAddrInfo(const char *portNumber, const int family, const int flags);
    void getAddrInfo();

    bool getDebug();
    void setDebug(const bool debug);

    int getBacklog();
    void setBacklog(const int backlog);

    int getSockfd();
};