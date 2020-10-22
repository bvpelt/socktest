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

#ifndef BS_STRING_INCLUDED
#include <string.h>
#define BS_STRING_INCLUDED 1
#endif

#ifndef BS_STDIO_INCLUDED
#include <stdio.h>
#define BS_STDIO_INCLUDED 1
#endif

#ifndef BS_UNISTD_INCLUDED
#include <unistd.h>
#define BS_UNISTD_INCLUDED 1
#endif

#define BS_MAX_ERROR_SIZE 256

class BSSocket
{
private:
    int sockfd;
    int errcode;
    int backlog; // number of connections on incoming queue
    char errmsg[BS_MAX_ERROR_SIZE] = "";
    addrinfo adresinfo;

    bool validateDomain(const int domain);
    bool validateType(const int type);
    bool validateProtocol(const int protocol);

public:
    BSSocket();
    ~BSSocket();
    int create();
    int create(const int domain, const int type, const int protocol);
    int setOptions(int level, int optname, const void *optval, int optlen);

    int getAddrInfo(const char *portNumber, const int family, const int flags);

    char *getErrorMessage();
    int getErrorCode();
};