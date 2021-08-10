/* Week 33 10 Aug 2021
   Be careful with life time of variables when using in threads

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

void threadFunc(const std::vector<int>* v)
{
    std::cout << std::accumulate(v->begin(), v->end(), 0) << "\n";
}

void startThread()
{
    std::vector<int> v(1000000, 1);
    std::thread t(threadFunc, &v);
    t.detach(); 
    // because thread t is detached startThread may return even if thread t is not finished.
    // v is destroyed here but detached thread t 
    // may still be using v
    // So it causes Segmentation fault (core dumped)

    //t.join(); This won't cause any problem as startThread will wait for thread t to finish
}

int main()
{
    startThread();
    // Give the thread started by startThread
    // sufficient time to complete its work.
    std::this_thread::sleep_for(std::chrono::seconds(5));
}