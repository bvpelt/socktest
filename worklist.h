
#ifndef LIST_INCLUDED
#include <list>
#define LIST_INCLUDED 1
#endif

#ifndef WORK_INCLUDED
#include "work.h"
#define WORK_INCLUDED 1
#endif

#ifndef PTHEAD_INCLUDED
#include <pthread.h>
#define PTHEAD_INCLUDED 1
#endif

using namespace std;

class Worklist
{
private:
    list<Work *> worklist;

public:
    Worklist();
    ~Worklist();

    int addWork(Work *work);
    Work *getWork();
    int cleanup();
    int size();
    list<Work *>::iterator getBegin();
    list<Work *>::iterator getEnd();
};