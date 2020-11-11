#ifndef BS_BSSOCKET_INCLUDED
#include "bssocket.h"
#define BS_BSSOCKET_INCLUDED 1
#endif

#ifndef BS_STRING_INCLUDED
#include <string>
#define BS_STRING_INCLUDED 1
#endif

#ifndef BS_IOSTREAM_INCLUDED
#include <iostream>
#define BS_IOSTREAM_INCLUDED 1
#endif

#ifndef BS_SSTREAM_INCLUDED
#include <sstream>
#define BS_SSTREAM_INCLUDED 1
#endif

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

BSSocket::BSSocket()
{
    sockfd = 0;
    memset(&adresinfo, 0, sizeof(adresinfo));
}

BSSocket::BSSocket(const int clientConnection)
{
    this->sockfd = clientConnection;
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

string BSSocket::getInet4Adres()
{
    char ipstr[INET6_ADDRSTRLEN]; // space to hold the IPv4 string
    ostringstream str;
    struct in_addr *sa;
    sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(adresinfo.ai_addr);
    sa = &(ipv4->sin_addr);

    int port = ntohs(ipv4->sin_port);
    // convert IPv4 and IPv6 addresses from binary to text form

    inet_ntop(AF_INET, sa, ipstr, sizeof(ipstr));

    str << ipstr << ":" << port << endl;

    return str.str().c_str();
}

string BSSocket::getInet6Adres()
{
    char ipstr[INET6_ADDRSTRLEN]; // space to hold the IPv4 string
    ostringstream str;
    struct in6_addr *sa;

    struct sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(adresinfo.ai_addr);
    sa = &(ipv6->sin6_addr);

    int port = ntohs(ipv6->sin6_port);
    // convert IPv4 and IPv6 addresses from binary to text form

    inet_ntop(AF_INET6, sa, ipstr, sizeof(ipstr));

    str << ipstr << ":" << port;

    return str.str().c_str();
}

void BSSocket::dbgMsg(const string msg)
{
    if (showDebug)
    {
        if (adresinfo.ai_family == AF_INET)
        {
            cout << msg << " " << getInet4Adres() << endl;
        }
        else if (adresinfo.ai_family == AF_INET6)
        {
            cout << msg << " " << getInet6Adres() << endl;
        }
        else
        {
            throw BSException("Unknown family", __FILE__, __LINE__);
        }
    }
}

bool BSSocket::getDebug()
{
    return showDebug;
}
void BSSocket::setDebug(const bool debug)
{
    this->showDebug = debug;
}

int BSSocket::getBacklog()
{
    return backlog;
}
void BSSocket::setBacklog(const int backlog)
{
    this->backlog = backlog;
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

    dbgMsg("bind");
    retval = bind(sockfd, adresinfo.ai_addr, adresinfo.ai_addrlen);
    if (BS_ERROR == retval)
    {
        throw new BSException(errno, __FILE__, __LINE__);
    }

    return retval;
}

int BSSocket::listensock()
{
    int retval = 0;

    dbgMsg("listen");
    retval = listen(sockfd, backlog);
    if (BS_ERROR == retval)
    {
        throw new BSException(errno, __FILE__, __LINE__);
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

    dbgMsg("accept");

    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);

    /* Wait up to one seconds. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    retval = select(sockfd + 1, &rfds, NULL, NULL, &tv);

    if (retval == -1)
    {
        perror("select()");
        if (errno == EINTR)
        {
            retval = -5;
        }
    }
    else if (retval) // data available
    {
        retval = accept(sockfd, (sockaddr *)&client_addr, &client_addr_size);
        if (BS_ERROR == retval)
        {
            throw new BSException(errno, __FILE__, __LINE__);
        }
    }
    else
    {
        retval = -2; // no data available
    }

    return retval;
}

//
// client side
//
int BSSocket::connectsock(const char *host, const char *port, const int version)
{
    int retval = BS_SUCCESS;
    retval = closeExistingSocket();

    if (BS_SUCCESS == retval)
    {
        memset(&adresinfo, 0, sizeof(adresinfo));
        if (version == 4)
        {
            adresinfo.ai_family = AF_INET;
        }
        if (version == 6)
        {
            adresinfo.ai_family = AF_INET6;
        }
        adresinfo.ai_socktype = SOCK_STREAM;
        adresinfo.ai_flags = AI_PASSIVE;

        retval = getaddrinfo(host, port, &adresinfo, &res);
        if (retval != BS_SUCCESS)
        {
            throw new BSException(errno, __FILE__, __LINE__);
        }
        else
        {
            retval = create(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (retval != BS_ERROR)
            {
                memcpy(&adresinfo, res, sizeof(adresinfo));
                sockfd = retval;
                dbgMsg("socket");

                retval = connect(sockfd, adresinfo.ai_addr, adresinfo.ai_addrlen);
                if (retval != BS_SUCCESS)
                {
                    throw new BSException(errno, __FILE__, __LINE__);
                }
                else
                {
                    dbgMsg("connect");
                }
            }
            else
            {
                throw new BSException(errno, __FILE__, __LINE__);
            }
        }
        if (res != NULL)
        {
            free(res);
        }
    }
    return retval;
}

//
// server and client side
//

int BSSocket::readit()
{
    int valread = 0;

    valread = readsock(sockfd);

    return valread;
}

int BSSocket::readsock(const int clientConnection)
{
    int valread = 0;

    dbgMsg("read");
    valread = read(clientConnection, buffer, sizeof(buffer));
    if (BS_ERROR == valread)
    {
        throw new BSException(errno, __FILE__, __LINE__);
    }
    else
    {
        buffer[valread + 1] = '\0';
    }
    return valread;
}

string BSSocket::getBuffer()
{
    return buffer;
}

int BSSocket::write(const void *buffer, const int len, const int flags)
{
    int valwritten = BS_SUCCESS;

    valwritten = writesock(sockfd, buffer, len, flags);

    return valwritten;
}

int BSSocket::writesock(const int connection, const void *buffer, const int len, const int flags)
{
    int valwritten = BS_SUCCESS;

    dbgMsg("send");
    valwritten = send(connection, buffer, len, flags);
    if (BS_ERROR == valwritten)
    {
        throw new BSException(errno, __FILE__, __LINE__);
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
        dbgMsg("close");
        retval = close(sockfd);
        if (retval == 0)
        {
            sockfd = 0;
        }
        else
        {
            throw new BSException(errno, __FILE__, __LINE__);
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
            throw new BSException(errno, __FILE__, __LINE__);
        }
    }
    return retval;
}

int BSSocket::getAddrInfo(const char *portNumber, const int family, const int flags)
{
    int retval = 0;

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    // for more explanation, man socket
    hints.ai_family = family;        // don't specify which IP version to use yet
    hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM refers to TCP, SOCK_DGRAM will be UDP
    hints.ai_flags = flags;

    retval = getaddrinfo(NULL, portNumber, &hints, &res);

    if (retval != 0)
    {
        throw new BSException(errno, __FILE__, __LINE__);
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
        cout << ipVer << " : " << ipStr << endl;
    }

    if (adresinfo.ai_family == AF_INET6)
    {
        ipVer = "IPv6";
        sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(adresinfo.ai_addr);
        addr = &(ipv6->sin6_addr);

        // convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(adresinfo.ai_family, addr, ipStr, sizeof(ipStr));
        cout << ipVer << " : " << ipStr << endl;
    }
}

int BSSocket::setOptions(int level, int optname, const void *optval, int optlen)
{
    int retval = 0;

    retval = setsockopt(sockfd, level, optname, optval, optlen);
    if (-1 == retval)
    {
        throw new BSException(errno, __FILE__, __LINE__);
    }
    return retval;
}

int BSSocket::getSockfd()
{
    return sockfd;
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