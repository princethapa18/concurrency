// 4 jan 2020

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>

// Using mutex to avoid data race

std::mutex mtx;   //  mutex has lock() and unlock() functions but instead its better to use RAII for handling mutex.

/* We will use RAII for handling the mutex.
 * Think of the mutex as a resource that always needs to unlocked when we are done using it.
 * Here we will use std::lock_guard<Mutex> template to ensure that we release the mutex safely.    
*/

void print_block(int n, char c)
{
    std::lock_guard<std::mutex> lg(mtx);  // If I use the std::lock_guard   then I don't have to remember abt where to unlock.
    //mtx.lock();
    for(int i = 0; i < n; ++i)
	std::cout << c;
    std::cout<< '\n';
    //mtx.unlock();
}

int main()
{
    
    std::thread t1 = std::thread{print_block, 50, '*'};
    std::thread t2 = std::thread{print_block, 50, '$'};
    t1.join();
    t2.join();
    return 0;
}

// O/P 
/*
 Without mutex
 *******************************$*****************$$$$*$*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 With mutex
 **************************************************
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
    If I forget to unlock mutex then 
    **************************************************
    and gets stuck 
    
    
*/