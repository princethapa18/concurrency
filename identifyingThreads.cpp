/* Week 33 10 Aug 2021
   Identifying threads
   
   The std::this_thread Namespace 

   get_id      --> get ID of current thread
   yield       --> suggest rescheduling current thread so as to allow other threads to run
   sleep_for   --> blocks execution of current thread for at least specified duration
   sleep_until --> blocks execution of current thread until specified time reached
*/

#include <thread>
#include <iostream>

// main thread ID

std::thread::id mainThread;

void func()
{
    if(std::this_thread::get_id() == mainThread)
        std::cout <<"called by main thread.\n";  // we can decide what to based on which thread called it.
    else
        std::cout <<"called by secondary thread.\n";
}

int main()
{
    mainThread = std::this_thread::get_id();
    std::thread t([](){func(); /* call func from secondary thread */});
    //call func from main thread;
    func();
    t.join();
}