#include <iostream>
#include "worklist.h"

int main(int argc, char *argv[], char *envp[])
{
    const int maxwork = 10;
    Work works[maxwork];
    Worklist wl;
    for (int i = 0; i < maxwork; i++)
    {
        Work w(1, WORK_STARTED, i);
        works[i] = w;
    }

    works[5].setStatus(WORK_ENDED);
    works[9].setStatus(WORK_ENDED);
    works[0].setStatus(WORK_ENDED);

    for (int i = 0; i < maxwork; i++)
    {
        wl.addWork(works[i]);
    }

    cout << "Before" << endl;
    for (list<Work>::iterator it = wl.getBegin(); it != wl.getEnd(); ++it)
    {
        cout << *it << endl;
    }

    cout << "Cleanup" << endl;
    wl.cleanup();

    cout << "After" << endl;
    for (list<Work>::iterator it = wl.getBegin(); it != wl.getEnd(); ++it)
    {
        cout << *it << endl;
    }
}