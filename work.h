#ifndef STRING_INCLUDED
#include <string>
#define STRING_INCLUDED 1
#endif

#ifndef PTHEAD_INCLUDED
#include <pthread.h>
#define PTHEAD_INCLUDED 1
#endif

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
    pthread_t thread;

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

    pthread_t *getThreadAdres();

    friend ostream &operator<<(ostream &_stream, Work const &w);
};
