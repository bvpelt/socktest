#include <string>
using namespace std;

#define WORK_STARTED 0
#define WORK_ACTIVE 1
#define WORK_ENDED 2

class Work
{
private:
    int connection;
    int status; // 0 started, 1 active, 2 ended
    int retval;

public:
    Work();
    Work(const int connection, const int status, const int retval);
    Work(const Work &w);
    ~Work();

    int getConnection();
    void setConnection(const int connection);
    int getStatus();
    void setStatus(const int status);
    int getRetval();
    void setRetval(const int retval);
    string toString();

    friend ostream &operator<<(ostream &_stream, Work const &w);
};
