#ifndef BS_STRING_INCLUDED
#include <string.h>
#define BS_STRING_INCLUDED 1
#endif

#ifndef BS_SSTREAM_INCLUDED
#include <sstream>
#define BS_SSTREAM_INCLUDED 1
#endif

using namespace std;

#include "bsexception.h"

BSException::BSException()
{
    errcode = 0;
    errmsg = "SUCCESS";
    source = "unknown";
    lineno = 0;
}

BSException::BSException(const int errcode)
{
    source = "unknown";
    lineno = 0;

    fillErrorCode(errcode);
}

BSException::BSException(const int errcode, const string source, const int lineno)
{

    fillErrorCode(errcode);
    this->source = source;
    this->lineno = lineno;
}

BSException::BSException(const string message, const string source, const int lineno)
{
    errcode = -1;
    errmsg = message;
    this->source = source;
    this->lineno = lineno;
}

int BSException::getErrorCode()
{
    return errcode;
}

string BSException::getErrorMessage()
{
    return errmsg;
}

string BSException::getSourceFile()
{
    return source;
}

int BSException::getLineNo()
{
    return lineno;
}

const char *BSException::what() const throw()
{
    ostringstream str;
    str << "Error (" << errcode << "): " << errmsg << " - " << source << "(" << lineno << ")" << source;
    return str.str().c_str();
}

//
// private
//
void BSException::fillErrorCode(const int errcode)
{
    this->errcode = errcode;

    char buffer[256];
    char *retval = strerror_r(errcode, buffer, 255);
    if (retval != NULL)
    {
        errmsg = buffer;
    }
    else
    {
        errmsg = "UNDETERMINED";
    }
}