/* Week 33 14 Aug 2021
   

    std::shared_mutex     
    1. shared locking (read access)  (multiple threads can read the shared data.)
       read krte waqt data race ki koi bakchodi nhi hai
    2. exclusive locking (write access)
       Ek thread hi exclusive lock acquire kar sakta hai nahi to data race ho jayega

    std::shared_timed_mutex   bas time ka funda hai [Acquiring mutex with Timeout]
    kuch der tak try karega lock krne ki . Nahi kar paya to chod dega
    Additional functions
    try_lock_for()    
    try_lock_until()

    try_shared_lock_for()
    try_shared_lock_until()

     Jaise std::mutex aur std::timed_mutex (sirf exclusive locking provide karte hai (write access)) unke liye
    wrapper RAII class hai std::unique_lock and std::lock_guard

    std::shared_lock is RAII class for shared mutexes
    template <class T > class shared_lock;
    -> unique_lock ke jaisa hi hai bas shared locking hai isme

*/

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <shared_mutex>
#include <chrono>

std::mutex coutMutex;
int counter = 0;
std::shared_timed_mutex counterMutex;

void writer()
{
    std::cout << "**************** writer starts *****************\n";
    for(int i = 0; i < 10; ++i)
    {
        std::lock_guard<std::shared_timed_mutex> lock(counterMutex);
        ++counter;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "********* writer ends ******************\n";
}

void reader()
{
    std::cout << "**************** reader starts *****************\n";
    for(int i = 0; i < 100; ++i)
    {
        int c;
        {
            std::shared_lock<std::shared_timed_mutex> lock(counterMutex);
            c = counter;
        }
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << std::this_thread::get_id() << " " << c << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } 
    std::cout << "**************** reader ends *****************\n";
}

int main()
{
    std::vector<std::thread> threads;
    threads.emplace_back(writer);
    for(int i = 0; i < 16; ++i)
    {
        threads.emplace_back(reader);
    }
    for(auto & t: threads)
    {
        t.join();
    }
}