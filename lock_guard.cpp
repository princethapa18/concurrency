// Week 15 10 April 2021 Sat
// Lock Guard

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

std::mutex mtx;

void print_smth(std::string str)
{
    try {
        for(int i = 0; i < 5 ; ++i)
        {
            std::lock_guard<mutex> lg(mtx);
            //mtx.lock();
            cout << str[0] << str[1] << str[2] << std::endl;  // critical region
            //throw std::exception();
            //mtx.unlock();
            //std::this_thread::sleep_for(50ms); // gives compile error
            std::this_thread::sleep_for(chrono::milliseconds(50));   // non critical region
        }
    }
    catch (std::exception& e)
    {
        cout << "Exception caught\n";
        //mtx.unlock();
    }
}

int main() 
{
    std::thread t1 {print_smth, "abc"};
    std::thread t2 {print_smth, "def"};
    std::thread t3 {print_smth, "xyz"};
    t1.join();
    t2.join();
    t3.join();
    return 0;
}

// O/P

/*
When exception is called.
mutex for t1 remains locked.
xyz
terminate called after throwing an instance of 'std::exception'
  what():  std::exception
Aborted (core dumped)

When exception is called and caught
abc
Excpetion caught

The program is deadlocked as the mutex remains locked

Drawbacks of std::mutex
1. Calling lock() requires a corresponding call to unlock()
2. The programmer must ensure that unlock() is always called after the
   critical region so that other threads can enter it, even if
   -> Multiple paths through code
   -> An exception is thrown

3. If unlock() is not called, the mutex will remain locked after the thread ends
  --> Any threads that are waiting for the lock will be blocked
  --> If the main thread is joined on a blocked thread, the program is deadlocked
4. for these reasons, we do not normally use std::mutex directly


std::lock_guard

--> Its dtor calls the unlock() on the mutex aquired
--> Even when an exception is thrown , the std::lock_guard's dtor is called
    which unlocks the mutex.
--> The programmer doesn't have to worry abt unlocking the mutex. Its always unlocked.

However the non critical region is also locked.

    mtx.lock();
    cout << str[0] << str[1] << str[2] << std::endl; // Only Critical region locked
    mtx.unlock();
    std::this_thread::sleep_for(chrono::milliseconds(50));  // Non critical region not locked

    std::lock_guard<mutex> lg(mtx);
    cout << str[0] << str[1] << str[2] << std::endl;          // Critical region
    std::this_thread::sleep_for(chrono::milliseconds(50));    // Non critical region

*/