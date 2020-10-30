#include <pthread.h>
#include <iostream>
#include "worklist.h"
using namespace std;

static int arg = 20; // number of work items
static const int success = 0;
static const int error = -1;

int doTest()
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

    return 0;
}

void *producer(void *arg)
{
    int *maxwork = (int *)arg;
    cout << "in thread with argument maxwork: " << *maxwork << endl;
    return ((void *)&success);
}

int main(int argc, char *argv[], char *envp[])
{
    const int maxpool = 3; // producer and 2 consumers

    pthread_t pool[maxpool];
    pthread_t producerThread;
    void *statusp;
    int retval;

    retval = pthread_create(&producerThread, NULL, producer, &arg);
    if (0 == retval)
    {
        retval = pthread_join(producerThread, &statusp);
    }

    if (statusp == PTHREAD_CANCELED)
    {
        cout << "Thread was cancelled" << endl;
    }
    else
    {
        cout << "thread compled with exit status: " << *((int *)statusp) << endl;
    }

    return retval;
}