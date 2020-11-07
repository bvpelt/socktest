#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include "worklist.h"
using namespace std;

static int arg = 20; // number of work items
static const int success = 0;
static const int error = -1;

pthread_mutex_t global_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_avail = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

struct parameterType
{
    Worklist *wl;
    long threadid;
    int poolsize;
    pthread_mutex_t *mutex;
    pthread_cond_t *avail;
    pthread_cond_t *empty;
};

int doTest()
{
    const int maxwork = 10;
    Work *works[maxwork];
    Worklist wl;
    for (int i = 0; i < maxwork; i++)
    {
        Work *w = new Work(1, WORK_STARTED, i);
        works[i] = w;
    }

    works[5]->setStatus(WORK_ENDED);
    works[9]->setStatus(WORK_ENDED);
    works[0]->setStatus(WORK_ENDED);

    for (int i = 0; i < maxwork; i++)
    {
        wl.addWork(works[i]);
    }

    cout << "Before" << endl;
    for (list<Work *>::iterator it = wl.getBegin(); it != wl.getEnd(); ++it)
    {
        cout << *(*it) << endl;
    }

    cout << "Cleanup" << endl;
    wl.cleanup();

    cout << "After" << endl;
    for (list<Work *>::iterator it = wl.getBegin(); it != wl.getEnd(); ++it)
    {
        cout << *(*it) << endl;
    }

    return 0;
}

void *producer(void *arg)
{
    parameterType *parameter = (parameterType *)arg;
    pthread_mutex_t *mutex = parameter->mutex;
    pthread_cond_t *avail = parameter->avail;
    pthread_cond_t *empty = parameter->empty;
    Worklist *worklist = parameter->wl;
    int poolsize = parameter->poolsize;

    cout << "Producer == Parameter - wl: " << parameter->wl << " mutex: " << parameter->mutex << " threadid: " << parameter->threadid << endl;

    cout << "in producer thread with argument worklist size: " << parameter->wl->size() << endl;

    for (int i = 0; i < 100; i++)
    {
        Work *w = new Work(i, 0, 100 + 1);
        pthread_mutex_lock(mutex);
        pthread_cond_wait(empty, mutex);
        worklist->addWork(w);
        pthread_cond_signal(avail);
        pthread_mutex_unlock(mutex);
    }

    delete parameter;

    return ((void *)&success);
}

void *consumer(void *arg)
{
    parameterType *parameter = (parameterType *)arg;
    pthread_mutex_t *mutex = parameter->mutex;
    pthread_cond_t *avail = parameter->avail;
    pthread_cond_t *empty = parameter->empty;
    Worklist *worklist = parameter->wl;
    int poolsize = parameter->poolsize;

    int curSize;

    cout << "Consumer == Parameter - wl: " << parameter->wl << " mutex: " << parameter->mutex << " threadid: " << parameter->threadid << endl;
    curSize = parameter->wl->size();
    cout << "in consumer thread: " << parameter->threadid << " with argument worklist size: " << curSize << endl;

    while (true)
    {
        pthread_mutex_lock(mutex);
        if (parameter->wl->size() > 0)
        {
            //        pthread_cond_wait(avail, mutex);
            cout << "in consumer thread: " << parameter->threadid << " pid: " << getpid() << " thread_self: " << pthread_self() << " sysid: " << syscall(SYS_gettid) << endl;
            cout << "in consumer thread with argument worklist size: " << curSize << endl;
            Work *w = worklist->getWork();
            curSize = parameter->wl->size();
            //        pthread_cond_signal(avail);

            if (parameter->wl->size() > 0)
            {
                pthread_cond_signal(avail);
            }

            if (w != NULL)
            {
                delete w;
            }
        }
        else
        {
            if (parameter->wl->size() == 0)
            {
                pthread_cond_signal(empty);
            }
        }
        pthread_cond_wait(avail, mutex);
        pthread_mutex_unlock(mutex);
        //sleep(1);
    }
    delete parameter;

    return ((void *)&success);
}

int main(int argc, char *argv[], char *envp[])
{
    const int maxpool = 3; // producer and 2 consumers

    pthread_t pool[maxpool];
    void *poolstatusp[maxpool];
    pthread_t producerThread;
    void *statusp;
    int retval = 0;
    Worklist wl;
    parameterType *parameter;
    //    pthread_mutex_init(&global_data_mutex, NULL);

    if (0 == retval)
    {
        for (int i = 0; i < maxpool; i++)
        {
            parameter = new parameterType();
            parameter->wl = &wl;
            parameter->mutex = &global_data_mutex;
            parameter->avail = &data_avail;
            parameter->threadid = 1 + i;
            parameter->poolsize = maxpool - 1;

            retval = pthread_create(&pool[i], NULL, consumer, parameter);
            if (0 == retval)
            {
                retval = pthread_join(pool[i], &poolstatusp[i]);
            }
        }
    }

    parameter = new parameterType();
    parameter->wl = &wl;
    parameter->mutex = &global_data_mutex;
    parameter->avail = &data_avail;
    parameter->threadid = 0;
    parameter->poolsize = 0;

    cout << "Parameter - wl: " << parameter->wl << " mutex: " << parameter->mutex << " threadid: " << parameter->threadid << endl;

    retval = pthread_create(&producerThread, NULL, producer, parameter);

    retval = pthread_join(producerThread, &statusp);

    if (statusp == PTHREAD_CANCELED)
    {
        cout << "Thread was cancelled" << endl;
    }
    else
    {
        cout << "thread compled with exit status: " << *((int *)statusp) << endl;
        for (int i = 0; i < maxpool; i++)
        {
            cout << "thread compled with exit status: " << *((int *)poolstatusp[i]) << endl;
        }
    }

    return retval;
}