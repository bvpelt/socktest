#include "work.h"

Work::Work()
{
}

Work::Work(const int connection, const int status, const int retval)
{
    this->connection = connection;
    this->status = status;
    this->retval = retval;
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