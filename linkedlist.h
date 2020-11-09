#ifndef PTHREAD__INCLUDED
#include <pthread.h>
#define PTHREAD__INCLUDED 1
#endif

#ifndef ELEMENT_INCLUDED
#include "element.h"
#define ELEMENT_INCLUDED 1
#endif

#ifndef WORK_INCLUDED
#define WORK_INCLUDED 1
#include "work.h"
#endif

template <class T>
class LinkedList
{
private:
    Element<T> *head;
    Element<T> *tail;
    int size;
    pthread_mutex_t mutex;

    void addElement(Element<T> *work);
    Element<T> *getElement();

public:
    LinkedList();
    ~LinkedList();

    void addData(T *work);
    T *getData();
    int getSize();
};

template <class T>
LinkedList<T>::LinkedList()
{
    head = NULL;
    tail = NULL;
    size = 0;

    int retval = pthread_mutex_init(&mutex, NULL);
}

template <class T>
LinkedList<T>::~LinkedList()
{
    while (getSize() > 0)
    {
        Element<T> *element = getElement();
        if (element != NULL)
        {
            delete element;
        }
    }
}

template <class T>
void LinkedList<T>::addElement(Element<T> *element)
{
    if ((head == NULL) && (tail == NULL))
    {
        element->setNext(element);
        element->setPrev(element);
        head = element;
        tail = element;
    }
    else
    {                           // add element at tail
        element->setNext(head); // 3
        tail->setNext(element); // 1
        element->setPrev(tail); // 2
        head->setPrev(element); // 4

        tail = element; // increment tail
    }
    size++; // increment size;
}

template <class T>
void LinkedList<T>::addData(T *work)
{
    Element<T> *element = new Element<T>(work);
    int retval = pthread_mutex_lock(&mutex);
    addElement(element);
    retval = pthread_mutex_unlock(&mutex);
}

template <class T>
Element<T> *LinkedList<T>::getElement()
{
    Element<T> *element = NULL;

    if ((head != NULL) && (tail != NULL))
    {
        if (head == tail)
        { // only one element in the list
            element = head;
            head = NULL;
            tail = NULL;
        }
        else
        {
            element = head;
            head = element->getNext();
            tail->setNext(head);
            head->setPrev(tail);
        }
        size--;
    }
    return element;
}

template <class T>
T *LinkedList<T>::getData()
{
    int retval = pthread_mutex_lock(&mutex);
    Element<T> *element = getElement();
    retval = pthread_mutex_unlock(&mutex);
    if (element == NULL)
    {
        return NULL;
    }
    else
    {
        return element->getData();
    }
}

template <class T>
int LinkedList<T>::getSize()
{
    int retval = pthread_mutex_lock(&mutex);
    int cursize = size;
    retval = pthread_mutex_unlock(&mutex);
    return cursize;
}