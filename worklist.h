
#ifndef LIST_INCLUDED
#include <list>
#define LIST_INCLUDED 1
#endif

/*
#ifndef VECTOR_INCLUDED
#include <vector>
#define VECTOR_INCLUDED 1
#endif
*/

#ifndef WORK_INCLUDED
#include "work.h"
#define WORK_INCLUDED 1
#endif

using namespace std;

class Worklist
{
private:
    list<Work> worklist;

public:
    Worklist();
    ~Worklist();

    int addWork(const Work work);
    int cleanup();
    list<Work>::iterator getBegin();
    list<Work>::iterator getEnd();
};