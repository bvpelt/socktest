#ifndef BS_SSTREAM_INCLUDED
#include <sstream>
#define BS_SSTREAM_INCLUDED 1
#endif
using namespace std;

#ifndef WORK_INCLUDED
#include "work.h"
#define WORK_INCLUDED 1
#endif

Work::Work()
{
}

Work::Work(const int connection, const int status, const int retval)
{
    this->connection = connection;
    this->status = status;
    this->retval = retval;
}

Work::Work(const Work &w)
{
    this->connection = w.connection;
    this->status = w.status;
    this->retval = w.retval;
}

Work::~Work()
{
}

int Work::getConnection()
{
    return connection;
}

void Work::setConnection(const int connection)
{
    this->connection = connection;
}

int Work::getStatus()
{
    return status;
}

void Work::setStatus(const int status)
{
    this->status = status;
}

int Work::getRetval()
{
    return retval;
}

void Work::setRetval(const int retval)
{
    this->retval = retval;
}

ostream &operator<<(ostream &str, Work const &w)
{
    str << "connection: " << w.connection << " status: " << w.status << " retval: " << w.retval;
    return str;
}