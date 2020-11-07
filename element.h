#include <cstddef>

template <class T>
class Element
{
private:
    Element *prev;
    Element *next;
    T *data;

public:
    Element();
    Element(T *data);
    ~Element();

    void setData(T *data);
    T *getData();

    void setPrev(Element *element);
    Element *getPrev();
    void setNext(Element *element);
    Element *getNext();
};

template <class T>
Element<T>::Element()
{
    next = NULL;
    prev = NULL;
    data = NULL;
}

template <class T>
Element<T>::Element(T *data)
{
    next = NULL;
    prev = NULL;
    this->data = data;
}

template <class T>
Element<T>::~Element()
{
}

template <class T>
void Element<T>::setPrev(Element *element)
{
    prev = element;
}

template <class T>
Element<T> *Element<T>::getPrev()
{
    return prev;
}

template <class T>
void Element<T>::setData(T *data)
{
    this->data = data;
}

template <class T>
T *Element<T>::getData()
{
    return data;
}

template <class T>
void Element<T>::setNext(Element *element)
{
    next = element;
}

template <class T>
Element<T> *Element<T>::getNext()
{
    return next;
}
