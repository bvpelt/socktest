#ifndef WORK_INCLUDED
#include "work.h"
#define WORK_INCLUDED 1
#endif

#ifndef LINKEDLIST_INCLUDED
#include "linkedlist.h"
#define LINKEDLIST_INCLUDED 1
#endif

#include <unistd.h>

#include <iostream>
using namespace std;

void dotest01()
{
    LinkedList<Work> ll;

    for (int i = 0; i < 10; i++)
    {
        cout << "Add element: " << i << endl;
        Work *w = new Work(i, i, i);

        ll.addData(w);

        cout << "Linked list size: " << ll.getSize() << endl;
    }

    int size = ll.getSize();
    int loop = 0;
    while (size > 0)
    {
        cout << "Loop: " << loop << endl;

        Work *w = ll.getData();
        if (w != NULL)
        {
            delete w;
        }
        cout << "Linked list size: " << ll.getSize() << endl;
        loop++;
        size = ll.getSize();
    }
}

//======================================================================================

static const int success = 0;
static const int error = -1;

struct parameterType
{
    bool *goOn;
    LinkedList<Work> *linkedList;
    int threadid;
    pthread_mutex_t *output_mutex;
};

void *consume(void *arg)
{
    parameterType *parameter = (parameterType *)arg;
    LinkedList<Work> *linkedList = parameter->linkedList;
    int threadid = parameter->threadid;
    pthread_mutex_t *output_mutex = parameter->output_mutex;

    pthread_mutex_lock(output_mutex);
    cout << "Thread: " << threadid << " started (consumer)" << endl;
    pthread_mutex_unlock(output_mutex);

    while (*parameter->goOn)
    {
        Work *w = linkedList->getData();
        if (w == NULL)
        {
            usleep(500000);
        }
        else
        {
            pthread_mutex_lock(output_mutex);
            cout << "Thread: " << threadid << " Work: " << *w << endl;
            pthread_mutex_unlock(output_mutex);
            delete w;
        }
    }
    delete parameter;

    pthread_mutex_lock(output_mutex);
    cout << "Thread: " << threadid << " ended" << endl;
    pthread_mutex_unlock(output_mutex);

    return ((void *)&success);
}

void *produce(void *arg)
{
    parameterType *parameter = (parameterType *)arg;
    LinkedList<Work> *linkedList = parameter->linkedList;
    int threadid = parameter->threadid;
    pthread_mutex_t *output_mutex = parameter->output_mutex;

    pthread_mutex_lock(output_mutex);
    cout << "Thread: " << parameter->threadid << " started (producer)" << endl;
    pthread_mutex_unlock(output_mutex);

    for (int i = 0; i < 100; i++)
    {
        Work *w = new Work(i, 0, 100 + 1);
        linkedList->addData(w);
    }

    sleep(20);
    *parameter->goOn = false;

    delete parameter;

    pthread_mutex_lock(output_mutex);
    cout << "Thread: " << threadid << " ended" << endl;
    pthread_mutex_unlock(output_mutex);

    return ((void *)&success);
}

void dotest02()
{
    const int poolsize = 3;
    bool goOn = true;
    LinkedList<Work> ll;
    pthread_t producerThread;
    void *statusp;
    pthread_t consumer[poolsize];
    void *poolstatusp[poolsize];
    parameterType *parameter;
    pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

    int retval;

    parameter = new parameterType();
    parameter->linkedList = &ll;
    parameter->goOn = &goOn;
    parameter->threadid = 0;
    parameter->output_mutex = &output_mutex;
    retval = pthread_create(&producerThread, NULL, produce, parameter);

    for (int i = 0; i < poolsize; i++)
    {
        parameter = new parameterType();
        parameter->linkedList = &ll;
        parameter->goOn = &goOn;
        parameter->threadid = 1 + i;
        parameter->output_mutex = &output_mutex;

        retval = pthread_create(&consumer[i], NULL, consume, parameter);
        pthread_mutex_lock(&output_mutex);
        cout << "Created Thread: " << parameter->threadid << endl;
        pthread_mutex_unlock(&output_mutex);
    }

    for (int i = 0; i < poolsize; i++)
    {
        retval = pthread_join(consumer[i], &poolstatusp[i]);
    }
    retval = pthread_join(producerThread, &statusp);
}

int main(int argc, char *argv[], char *envp[])
{
    dotest02();
}