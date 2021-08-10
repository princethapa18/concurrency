/* Week33 10 Aug 2021    
   Thread Local Storage   https://www.youtube.com/watch?v=fqG8BgVbmcM

*/

#include <iostream>
#include <vector>
#include <thread>

thread_local int counter = 0;

void doWork(int id)
{
    static const char letters[] = "abcd";
    std::cout << "id : " << id << " thread id: " << std::this_thread::get_id() << "\n";
    for(int i = 0; i < 10; ++i)
    {
        std::cout << letters[id] << counter << "\n";
        ++counter;
    }
}

int main()
{
    std::vector<std::thread> workers;
    for(int i = 1; i <= 3; ++i)
    {
        // invoke doWork in new thread
        workers.emplace_back(doWork, i);
    }
    // invoke doWork in main thread
    doWork(0);
    for(auto& t: workers)
    {
        t.join();
    }
}