/* Week 33 13 Aug 2021
   std::unique_lock is another RAII class for mutexes

   The class unique_lock is a general-purpose mutex ownership wrapper allowing deferred locking, 
   time-constrained attempts at locking, recursive locking, transfer of lock ownership, and use with condition variables. 

   template <class T> class uniq_lock;
   T specifies type of mutex (Ex. std::mutex, std::recursive_mutex)

   unique_lock is more flexible than a lock_guard
   

   -> Unlike std::lock_guard, unique_lock do not have to hold mutex over entire lifetime of RAII object
   -> You have choice of whether to acquire mutex upon construction
      In std::lock_guard when u construct the std::lock_guard object you must acquire the 
      lock associated with the mutex and then you must hold that 
      until the std::lock_guard object is destroyed.
      lock_guard is not movable and copyable

      But in std::unique_lock you can defer the acquiring of the mutex to somepoint later 
      after the construction of the std::unique_lock object.
      
   -> also can acquire and release the mutex many times throughout the lifetime of the
      std::unique_lock object
   -> Upon destruction, if mutex is held, it is released
   -> movable but not copyable

   -> unique_lock is used in situations when RAII object is needed
      for mutex but you do not want to hold mutex over entire lifetime of RAII object

    Member Types     Description
    mutex_type     ---> underlying mutex type 

    Construction, Destruction and Assignment
    constructor --> constructs a unique_lock, optionally locking (i.e., taking ownership of) the supplied mutex 
    destructor  --> unlocks (i.e., releases ownership of) the associated mutex, if owned 
    operator=   --> unlocks (i.e., releases ownership of) the mutex, if owned, and acquires ownership of another (move assign)

    Locking Functions
    lock         --> locks (i.e., takes ownership of) the associated mutex  (blocking)
    try_lock     --> tries to lock (i.e., takes ownership of) the associated mutex without blocking 
    try_lock_for --> attempts to lock (i.e., takes ownership of) the associated TimedLockable mutex, 
                     returns if the mutex has been unavailable for the specified time duration 

*/


#include <thread>
#include <iostream>
#include <set>
#include <mutex>
#include <chrono>

class IntSet {
    std::set<int> _s;
    std::mutex _mtx; 
    mutable std::mutex _mutable_mtx;
public:
    bool contains(int i) const 
    {
        std::unique_lock<std::mutex> unq_lock(_mutable_mtx, std::defer_lock);  // std::defer_lock to defer acquiring of the mutex 
        // we defered locking so as to aqcuire the lock just before the critical section

        // do some non critical work
        //std::cout << "non critical work\n"; 
        //std::cout << "put thread : " << std::this_thread::get_id() << " to sleep for 100 ms";
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        unq_lock.lock(); // acquire mutex here
        // this is a critical section therefore it should be protected 
        return _s.find(i) != _s.end(); // if one thread is searching an item and another is inserting an item to the set then 
                                       // there could be data race
    }
    void add(int i) 
    {
        std::unique_lock<std::mutex> unq_lock(_mtx, std::defer_lock);
        // do some non critical work
        //std::cout << "non critical work\n"; 
        //std::cout << "put thread : " << std::this_thread::get_id() << " to sleep for 200 ms";
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));

        unq_lock.lock(); // acquire mutex here
        // critical section starts here
        _s.insert(i); 
        // critical section ends here
        
        // Here we no more need to lock the mutex after this as the remaining task is non critical 
        // In case of a lock_guard object we would have to wait for it to get destroyed but
        // for a unique_lock object we don't have to
        unq_lock.unlock() ; // just call unlock if you dont have to hold the mutex anymore

        // again do some non critical work
        //std::cout << "Again do non critical work after insertion \n"; 
        //std::cout << "put thread : " << std::this_thread::get_id() << " to sleep again for 500 ms";
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
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