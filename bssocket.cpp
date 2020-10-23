#ifndef BS_BSSOCKET_INCLUDED
#include "bssocket.h"
#define BS_BSSOCKET_INCLUDED 1
#endif

#include <string>
#include <iostream>
using namespace std;

BSSocket::BSSocket()
{
    sockfd = 0;
    errcode = 0;
    memset(&adresinfo, 0, sizeof(adresinfo));
}

//
// Remove claimed resources
//
BSSocket::~BSSocket()
{
    int retval = 0;
    if (sockfd > 0)
    {
        retval = close(sockfd);
        if (retval == 0)
        {
            sockfd = 0;
        }
    }
}

//
// server side routines (https://www.geeksforgeeks.org/socket-programming-cc/)
//
// - socket          createsock
// - setsockopt
// - bind            bindsock
// - listen          listensock
// - accept          acceptsock
//
// client side routines
// - connect         connect(host, port)
//
// server and client
// - send/recv       writesock/readsock
//
int BSSocket::createsock()
{
    return create(adresinfo.ai_family, adresinfo.ai_socktype, adresinfo.ai_protocol);
}

int BSSocket::bindsock()
{
    int retval = 0;

    retval = bind(sockfd, adresinfo.ai_addr, adresinfo.ai_addrlen);
    if (BS_ERROR == retval)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
    }

    return retval;
}

int BSSocket::listensock()
{
    int retval = 0;

    retval = listen(sockfd, backlog);
    if (BS_ERROR == retval)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
    }

    return retval;
}

//
// accept incoming connections
//
int BSSocket::acceptsock()
{
    int retval = 0;

    // structure large enough to hold client's address
    sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    retval = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
    if (BS_ERROR == retval)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
    }

    return retval;
}

//
// client side
//
int BSSocket::connect(const char *host, const char *port)
{
    int retval = BS_SUCCESS;
    retval = closeExistingSocket();

    if (BS_SUCCESS == retval)
    {
        struct addrinfo *res;
        memset(&adresinfo, 0, sizeof(adresinfo));

        retval = getaddrinfo(host, port, &adresinfo, &res);
        if (retval != BS_SUCCESS)
        {
            errcode = errno;
            char buffer[256];
            strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
        }
        else
        {
            retval = create(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (retval != BS_ERROR)
            {
                sockfd = retval;
            }
        }
    }
    return retval;
}

//
// server and client side
//
int BSSocket::readsock(const int clientConnection)
{
    int valread = 0;
    char buffer[1024] = {0};

    valread = read(clientConnection, buffer, sizeof(buffer));
    if (BS_ERROR == valread)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
    }
    return valread;
}

int BSSocket::writesock(const void *buffer, const int len, const int flags)
{
    int valwritten = BS_SUCCESS;

    valwritten = send(sockfd, buffer, len, flags);
    if (BS_ERROR == valwritten)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
    }
    return valwritten;
}

//
//
//
int BSSocket::closeExistingSocket()
{
    int retval = BS_SUCCESS;

    if (sockfd > 0)
    { // socket already in use, might not occur so first close existing socket
        retval = close(sockfd);
        if (retval == 0)
        {
            sockfd = 0;
        }
        else
        {
            errcode = errno;
            char buffer[256];
            strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
        }
    }
    return retval;
}

int BSSocket::create(const int domain, const int type, const int protocol)
{
    int retval = BS_SUCCESS;

    retval = closeExistingSocket();

    if (retval == BS_SUCCESS)
    {
        sockfd = socket(domain, type, protocol);
        retval = sockfd;
        if (BS_ERROR == retval)
        {
            errcode = errno;
            char buffer[256];
            strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
        }
    }
    return retval;
}

int BSSocket::getAddrInfo(const char *portNumber, const int family, const int flags)
{
    int retval = 0;

    addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));

    // for more explanation, man socket
    hints.ai_family = family;        // don't specify which IP version to use yet
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM refers to TCP, SOCK_DGRAM will be UDP
    hints.ai_flags = flags;

    retval = getaddrinfo(NULL, portNumber, &hints, &res);

    if (retval != 0)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
        return -2;
    }
    else
    {
        memcpy(&adresinfo, res, sizeof(*res));
    }
    return retval;
}

void BSSocket::getAddrInfo()
{
    std::string ipVer = "";
    char ipStr[INET6_ADDRSTRLEN]; // ipv6 length makes sure both ipv4/6 addresses can be stored in this variable
    void *addr;
    if (adresinfo.ai_family == AF_INET)
    {
        ipVer = "IPv4";
        sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(adresinfo.ai_addr);
        addr = &(ipv4->sin_addr);

        // convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(adresinfo.ai_family, addr, ipStr, sizeof(ipStr));
        std::cout << ipVer << " : " << ipStr
                  << std::endl;
    }

    if (adresinfo.ai_family == AF_INET6)
    {
        ipVer = "IPv6";
        sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(adresinfo.ai_addr);
        addr = &(ipv6->sin6_addr);

        // convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(adresinfo.ai_family, addr, ipStr, sizeof(ipStr));
        std::cout << ipVer << " : " << ipStr
                  << std::endl;
    }
}

int BSSocket::setOptions(int level, int optname, const void *optval, int optlen)
{
    int retval = 0;

    retval = setsockopt(sockfd, level, optname, optval, optlen);
    if (-1 == retval)
    {
        errcode = errno;
        char buffer[256];
        strncpy(errmsg, strerror_r(errcode, buffer, 255), 256);
    }
    return retval;
}

char *BSSocket::getErrorMessage()
{
    return errmsg;
}

int BSSocket::getErrorCode()
{
    return errcode;
}

// private parts

bool BSSocket::validateDomain(const int domain)
{

    bool goon = true;

    goon = ((domain == AF_UNIX) ||
            (domain == AF_LOCAL) ||
            (domain == AF_INET) ||
            (domain == AF_AX25) ||
            (domain == AF_IPX) ||
            (domain == AF_APPLETALK) ||
            (domain == AF_X25) ||
            (domain == AF_INET6) ||
            (domain == AF_DECnet) ||
            (domain == AF_KEY) ||
            (domain == AF_NETLINK) ||
            (domain == AF_PACKET) ||
            (domain == AF_RDS) ||
            (domain == AF_PPPOX) ||
            (domain == AF_LLC) ||
            (domain == AF_IB) ||
            (domain == AF_MPLS) ||
            (domain == AF_CAN) ||
            (domain == AF_TIPC) ||
            (domain == AF_BLUETOOTH) ||
            (domain == AF_ALG) ||
            (domain == AF_VSOCK) ||
            (domain == AF_KCM) ||
            (domain == AF_XDP));

    return goon;
}

bool BSSocket::validateType(const int type)
{

    bool goon = true;

    goon = ((type == SOCK_STREAM) ||
            (type == SOCK_DGRAM) ||
            (type == SOCK_SEQPACKET) ||
            (type == SOCK_RAW) ||
            (type == SOCK_RDM) ||
            (type == SOCK_PACKET));

    return goon;
}

bool BSSocket::validateProtocol(const int protocol)
{

    bool goon = true;

    goon = ((protocol == 0) ||
            (protocol == 1) ||
            (protocol == 2) ||
            (protocol == 3) ||
            (protocol == 4) ||
            (protocol == 5) ||
            (protocol == 6) ||
            (protocol == 8) ||
            (protocol == 9) ||
            (protocol == 12) ||
            (protocol == 17) ||
            (protocol == 20) ||
            (protocol == 22) ||
            (protocol == 27) ||
            (protocol == 29) ||
            (protocol == 33) ||
            (protocol == 36) ||
            (protocol == 37) ||
            (protocol == 38) ||
            (protocol == 41) ||
            (protocol == 43) ||
            (protocol == 44) ||
            (protocol == 45) ||
            (protocol == 46) ||
            (protocol == 47) ||
            (protocol == 50) ||
            (protocol == 51) ||
            (protocol == 57) ||
            (protocol == 58) ||
            (protocol == 59) ||
            (protocol == 60) ||
            (protocol == 73) ||
            (protocol == 81) ||
            (protocol == 88) ||
            (protocol == 89) ||
            (protocol == 93) ||
            (protocol == 94) ||
            (protocol == 97) ||
            (protocol == 98) ||
            (protocol == 99) ||
            (protocol == 103) ||
            (protocol == 108) ||
            (protocol == 112) ||
            (protocol == 115) ||
            (protocol == 124) ||
            (protocol == 132) ||
            (protocol == 133) ||
            (protocol == 135) ||
            (protocol == 136) ||
            (protocol == 137) ||
            (protocol == 138) ||
            (protocol == 139) ||
            (protocol == 140) ||
            (protocol == 141) ||
            (protocol == 142));

    return goon;
}