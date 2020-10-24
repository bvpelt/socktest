#include <string>
#include <exception>
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

    virtual const char *what() const throw();

    int getErrorCode();
    string getErrorMessage();
    string getSourceFile();
    int getLineNo();
};