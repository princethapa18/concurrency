/*  Week33 14 Aug 2021
    std::timed_mutex
    -> provides mutex that allows timeout to be specified when acquiring mutex
    -> if mutex cannot be acquired in time specified, acquire operation fails 
       (i.e. does not lock mutex) and error is returned
    -> adds try_lock_for and try_lock_until member functions to try to lock mutex with 
       timeout

    "Acquiring Mutex with Timeout"   
*/

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::timed_mutex timed_mtx;

void doWork()
{
    for(int i=0; i < 10000; ++i)
    {
        std::unique_lock<std::timed_mutex> lock(timed_mtx, std::defer_lock);
        int count = 0;
        while (!lock.try_lock_for(std::chrono::microseconds(1)))
        {
            ++count;
        }
        std::cout << count << "\n";
    }
}

int main()
{
    std::vector<std::thread> workers;
    for(int i = 0; i < 16; ++i)
    {
        workers.emplace_back(doWork);
    }
    for (auto& t : workers)
    {
        t.join();
    }
}