/* Week 33 13 Aug 2021 Friday 
   std::lock_guard is RAII class for mutexes
   template <class T> class lock_guard;
   -> T specifies type of mutex (Ex. std::mutex, std::recursive_mutex)
   -> Avoids problem of inadvertently forgetting to release mutex (Ex. due to exception or forgetting unlock call)
   -> constructor takes mutex as argument
   -> not movable and not copyable
   -> acquires mutex in constructor
   -> releases mutex in destructor
   
   -> use lock_guard instead of calling lock and unlock.
*/

#include <thread>
#include <iostream>
#include <set>
#include <mutex>

class IntSet {
    std::set<int> _s;
    std::mutex _mtx; // std::mutex is non copyable 
    // https://stackoverflow.com/questions/46295352/using-stdmutex-as-member-variable-in-a-class
    mutable std::mutex _mutable_mtx;
public:
    bool contains(int i) const 
    {
        /* std::lock_guard<std::mutex> lg(_mtx);
           This gives following compile error

           binding reference of type ‘std::lock_guard<std::mutex>::mutex_type& {aka std::mutex&}’ to 
           ‘const std::mutex’ discards qualifiers
           
           Im getting this error because this function is a const function and Im trying to modify a private member _mtx to lock and unlock

           https://stackoverflow.com/questions/48133164/how-to-use-a-stdlock-guard-without-violating-const-correctness

           Either make this function a non const or use a mutable std::mutex

        */
        std::lock_guard<std::mutex> lg(_mutable_mtx);  // this fixes the above compile error

        // this is a critical section therefore it should be protected 
        return _s.find(i) != _s.end(); // if one thread is searching an item and another is inserting an item to the set then 
                                       // there could be data race
    }
    void add(int i) 
    {
        std::lock_guard<std::mutex> lg(_mtx);
        // this is also a critical section therefore this should also be protected
        _s.insert(i);
    }
};

IntSet s;

int main()
{
    std::thread t1([]() {
        for (int i = 0; i < 10000; ++i) s.add(2 * i);
    } );
    std::thread t2([]() {
        for (int i = 0; i < 10000; ++i) s.add(2 * i + 1);
    } );
    t1.join();
    t2.join();
    std::cout << s.contains(1000) << "\n";
}

/*  3 threads are involved here
    1. Main thread
    and threads t1 and t2
*/

