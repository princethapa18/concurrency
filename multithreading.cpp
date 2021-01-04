// 25,27,31 Aug 2020  ***** Multithreading ***********

#include <iostream>
#include <thread>
#include <string>
using namespace std;

void hello()
{
    cout << "Hello concurrent world!" << endl;
}

class A
{
public:
    void operator()()
    {
        for(int i = 0; i < 5; i++)
            std::cout << "print from another thread" << endl;
    }
};

void thread_function()
{
    cout << "Inside thread :: Id = " << std::this_thread::get_id() << endl; // To get the identifier for the current thread use
}

void fun(int a, std::string str)
{
    cout << "a : " << a << " str : " << str << endl;
}

void newThreadCallBack(int *p)
{
    cout << "Inside thread : " << std::this_thread::get_id() << endl;
    cout << "p = " << endl;
    std::chrono::milliseconds dura(1000);
    std::this_thread::sleep_for(dura);
    *p = 19;
}

/*
void startNewThread()
{
    int i = 10;
    cout << "Inside Main thread : " << endl;
    cout << " i = " << i << endl;
    std::thread t(newThreadCallBack, &i);
    t.detach();
    cout << "Inside Main thread : " << endl;
    cout << " i = " << i << endl;
}
*/

void startNewThread()
{
    int *i = new int;
    *i = 10;
    cout << "Inside Main thread : " << endl;
    cout << " *i = " << *i << endl;
    std::thread t(newThreadCallBack, i);  // passed a pointer
    t.detach();
    delete i;
    i = NULL;
}

int main(int argc, char *argv[])
{
//    1. using function pointer
//    std::thread t(hello);
//    t.join();

//    for(int i = 0; i < 5; i++)
//        std::cout << "print from main thread" << endl;

//    2. using function object (functor)
//    std::thread t{A()}; // std::thread t( (A()) );
//    std::cout << "waiting for thread t to complete" << endl;
//    t.join();

//    3. using lambda expr.
//    std::thread t1{[]
//                {
//                    for(int i = 0; i < 5; i++)
//                        std::cout << "print from another thread" << endl;
//                }};

//    std::cout << "waiting for thread t to complete" << endl;
//    t1.join();

//    std::thread thd1(thread_function);
//    std::thread thd2(thread_function);

//    if(thd1.get_id() != thd2.get_id())
//        cout << "Both threads have different Ids" << endl;

//    std::cout << "From Main Thread :: ID of Thread 1 = "<< thd1.get_id() << std::endl; // Member function, gives id of associated thread object
//    std::cout << "From Main Thread :: ID of Thread 2 = "<< thd2.get_id() << std::endl;

//    thd1.join();
//    thd2.join();

    ////-------------------------------------
    /* passing simple arguments to thread */
    //thread t1(fun, 5, "test");
    //t1.join();

    startNewThread();
    std::chrono::milliseconds dura(2000);
    std::this_thread::sleep_for(dura);

    cout << "Exiting from the main thread" << endl;
    return 0;
}
