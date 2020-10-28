#include "worklist.h"

Worklist::Worklist()
{
}

Worklist::~Worklist()
{
}

int Worklist::addWork(const int parameter)
{
    int retval = 0;

    Work *work = new Work(parameter, 0, 0);
    worklist.push_back(*work);

    return retval;
}