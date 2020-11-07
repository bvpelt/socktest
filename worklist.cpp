#ifndef ITERATOR_INCLUDED
#include <iterator>
#define ITERATOR_INCLUDED 1
#endif

#ifndef WORKLIST_INCLUDED
#include "worklist.h"
#define WORKLIST_INCLUDED 1
#endif

using namespace std;

Worklist::Worklist()
{
}

Worklist::~Worklist()
{
}

int Worklist::addWork(Work *work)
{
    int retval = 0;

    worklist.push_back(work);

    return retval;
}

Work *Worklist::getWork()
{
    Work *result = NULL;
    list<Work *>::iterator it = worklist.begin();
    list<Work *>::iterator itend = worklist.end();
    list<Work *>::iterator itwork;

    if (it != itend)
    {
        itwork = it;
        it++;
        result = *itwork;
        worklist.erase(itwork);
    }
    return result;
}

int Worklist::cleanup()
{
    int retval = 0;

    list<Work *>::iterator it = worklist.begin();
    list<Work *>::iterator itend = worklist.end();
    list<Work *>::iterator iterase;
    while (it != itend)
    {
        Work *w = *it;
        if (w->getStatus() == WORK_ENDED)
        { // get location to delete
            // increment to next postion
            // delete element at saved location
            iterase = it;
            it++;
            worklist.erase(iterase);
        }
        else
        {
            it++;
        }
    }

    return retval;
}

int Worklist::size()
{
    return worklist.size();
}

list<Work *>::iterator Worklist::getBegin()
{
    return worklist.begin();
}

list<Work *>::iterator Worklist::getEnd()
{
    return worklist.end();
}