// 4 jan 2020

#include <iostream>
#include <cassert>
#include <thread>

int counter = 0; //global mutable variable

void increment_counter(int n)
{
    for(int i = 0; i < n; ++i)
      ++counter;
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
 1038108
datarace: datarace.cpp:23: int main(): Assertion `counter == (n_times*2)' failed.
Aborted (core dumped) 
 */



//  terminate called after throwing an instance of 'std::system_error'
//  what():  Enable multithreading to use std::thread: Operation not permitted
//  Aborted (core dumped)
//  g++ -std=c++11 datarace.cpp -lpthread -o datarace   // to enable multithreading using c++11