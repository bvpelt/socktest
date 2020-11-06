//
// https://thispointer.com/c-11-multithreading-part-1-three-different-ways-to-create-threads/
// https://riptutorial.com/cplusplus/example/2330/creating-a-std--thread
//
#include <iostream>
#include <thread>
using namespace std;

class DisplayThread
{
public:
    void operator()()     
    {
        cout << "Inside Thread :: ID  = " << this_thread::get_id() << ";" << endl;   
        for(int i = 0; i < 10000; i++)
            cout << "Display Thread Executing" << endl;
    }
};

void dotest02() {
    thread threadObj( (DisplayThread()) );
    for(int i = 0; i < 10000; i++) {
        cout << "Display From Main Thread " << endl;
    }
    cout << "Waiting For Thread to complete" << endl;
    threadObj.join();
    cout << "Exiting from Main Thread" << endl;
}

void thread_function()
{
    cout << "Inside Thread :: ID  = " << this_thread::get_id() << ";" << endl;   

    for(int i = 0; i < 10000; i++) {
        cout << "thread function Executing" << endl; 
        }
}

void dotest01() {
    thread threadObj(thread_function);
    for(int i = 0; i < 10000; i++) {
        cout << "Display From MainThread" << endl;
        }
    threadObj.join();    
    cout << "Exit of Main function" << endl;
}

void dotest03() {
    //int x = 9;
    thread threadObj([]{
            cout << "Inside Thread :: ID  = " << this_thread::get_id() << ";" << endl;   

            for(int i = 0; i < 10000; i++)
                cout << "Display Thread Executing" << endl;
            });
            
    for(int i = 0; i < 10000; i++) {
        cout << "Display From Main Thread" << endl;
    }

        
    threadObj.join();
    cout<<"Exiting from Main Thread" << endl;
}

void dotest04() {
     std::thread threadObj1(thread_function);
    std::thread threadObj2(thread_function);
 
    if(threadObj1.get_id() != threadObj2.get_id())
        std::cout<<"Both Threads have different IDs"<<std::endl;
 
        std::cout<<"From Main Thread :: ID of Thread 1 = "<<threadObj1.get_id()<<std::endl;    
    std::cout<<"From Main Thread :: ID of Thread 2 = "<<threadObj2.get_id()<<std::endl;    
 
    threadObj1.join();    
    threadObj2.join();    
}

int main() {
    dotest04();
    return 0;
}
