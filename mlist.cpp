#ifndef WORK_INCLUDED
#include "work.h"
#define WORK_INCLUDED 1
#endif

#ifndef ELEMENT_INCLUDED
#include "element.h"
#define ELEMENT_INCLUDED 1
#endif

#ifndef LINKEDLIST_INCLUDED
#include "linkedlist.h"
#define LINKEDLIST_INCLUDED 1
#endif

#include <iostream>
using namespace std;

int main(int argc, char *argv[], char *envp[])
{
    LinkedList<Work> ll;

    for (int i = 0; i < 10; i++)
    {
        cout << "Add element: " << i << endl;
        Work *w = new Work(i, i, i);
        // Element<Work> *element = new Element<Work>();
        // element->setData(w);
        // ll.addElement(element);
        ll.addData(w);

        cout << "Linked list size: " << ll.getSize() << endl;
    }

    int size = ll.getSize();
    int loop = 0;
    while (size > 0)
    {
        cout << "Loop: " << loop << endl;
        //Element<Work> *element = ll.getElement();
        //delete element;
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