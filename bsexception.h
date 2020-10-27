#ifndef BS_STRING_INCLUDED
#define BS_STRING_INCLUDED 1
#include <string>
#endif

#ifndef BS_EXCEPTION_INCLUDED
#include <exception>
#define BS_EXCEPTION_INCLUDED 1
#endif

using namespace std;

class BSException : public exception
{
private:
    int errcode;
    string source;
    int lineno;
    string errmsg;

    void fillErrorCode(const int errcode);

public:
    BSException();
    BSException(const int errcode);
    BSException(const int errcode, const string source, const int lineno);
    BSException(const string message, const string source, const int lineno);

    virtual const char *what() const throw();

    int getErrorCode();
    string getErrorMessage();
    string getSourceFile();
    int getLineNo();
};