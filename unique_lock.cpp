/*  Week 33 09 Aug 21 std::unique_lock
    manages the lifetime of the resources 


*/

#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

struct CriticalData {
    std::mutex mut;
}; 

// this function causes deadlock
// void deadLock(CriticalData& a, CriticalData& b)
// {
//     a.mut.lock();
//     std::cout << "get the first mutex\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     b.mut.lock();
//     std::cout << "get the second mutex\n";
//     // do something with a and b
//     a.mut.unlock();
//     b.mut.unlock();
// }
void deadLock(CriticalData& a, CriticalData& b)
{
    std::unique_lock<mutex> ul_a(a.mut, std::defer_lock); // std::defer_lock is used so that mutex is not locked automatically 
    std::cout << "Thread: " << this_thread::get_id() << " get the first mutex\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::unique_lock<mutex> ul_b(b.mut, std::defer_lock);
    cout << "Thread: " << this_thread::get_id() << " get the second mutex\n";

    cout << "    Thread: " << this_thread::get_id() << " get both mutex\n";
    std::lock(ul_a, ul_b);

    // do something with a and b
}

// void deadLock1(CriticalData& a, CriticalData& b)
// {
//    cout << "Thread: " << this_thread::get_id() << " first mutex" << endl;
//    this_thread::sleep_for(chrono::milliseconds(1));
//    cout << "  Thread: " << this_thread::get_id() << " second mutex" <<  endl;
//    cout << "    Thread: " << this_thread::get_id() << " get both mutex" << endl;
   
//    std::scoped_lock(a.mut, b.mut); //c++17
//    // do something with a and b
// }

int main()
{
    CriticalData c1;
    CriticalData c2;

    std::thread t1([&](){deadLock(c1, c2);});  // [capture](arguments)->return type{method body}
    std::thread t2([&](){deadLock(c2, c1);});

    t1.join();
    t2.join();
}