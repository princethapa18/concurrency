// 4 jan 2020

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>

// Using mutex to avoid data race

int counter = 0; //global mutable variable
std::mutex counter_mutex;   //  mutex has lock() and unlock() functions but instead its better to use RAII for handling mutex.

/* We will use RAII for handling the mutex.
 * Think of the mutex as a resource that always needs to unlocked when we are done using it.
 * Here we will use std::lock_guard<Mutex> template to ensure that we release the mutex safely.    
*/

void increment_counter(int n)
{
    for(int i = 0; i < n; ++i)
    {
      //std::lock_guard<std::mutex> lock(counter_mutex);
      counter_mutex.lock();
      ++counter;  // this is the critical section. It uses a shared mutable variable and can be modified by multiple threads.
      counter_mutex.unlock();
    }
}

int main()
{
    constexpr int  n_times = int{1000000};
    std::thread t1 = std::thread{increment_counter, n_times};
    std::thread t2 = std::thread{increment_counter, n_times};
    t1.join();
    t2.join();
    std::cout << counter << '\n';
    assert(counter == (n_times*2)); // If we don't have a data race , this assert should hold
    return 0;
}

// O/P
/*
 2000000
*/