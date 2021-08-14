/* Week 33 14 Aug 2021
   std::lock is a variadic template function that can acquire multiple locks simultaneously without 
   risk of deadlock

   template< class Lockable1, class Lockable2, class... LockableN >
   void lock( Lockable1& lock1, Lockable2& lock2, LockableN&... lockn );
		
   Locks the given Lockable objects lock1, lock2, ..., lockn using a 
   deadlock avoidance algorithm to avoid deadlock.
   The objects are locked by an unspecified series of calls to lock, try_lock, and unlock. 
   If a call to lock or unlock results in an exception, unlock is called for any locked objects before rethrowing. 

   "Acquiring two Locks for Swap"
*/

#include <thread>
#include <vector>
#include <mutex>

class BigBuf // A Big Buffer
{
   std::vector<char> data_;
   mutable std::mutex m_;
public:
   static constexpr int size() {return 16 * 1024 *1024;}
   BigBuf() : data_(size()) {}
   BigBuf& operator=(const BigBuf&) = delete;
   BigBuf& operator=(BigBuf&&) = delete;
   void swap(BigBuf& other)
   {
      if(this == &other)
         return;
      // acquiring the 2 mutexes in this way 
      // can result in deadlock
      /*
      std::lock_guard<std::mutex> lock1(m_);
      std::lock_guard<std::mutex> lock2(other.m_);
      std::swap(data_, other.data_);
      */
      
      std::unique_lock<std::mutex> ul1(m_, std::defer_lock);
      std::unique_lock<std::mutex> ul2(other.m_, std::defer_lock);
      std::lock(ul1, ul2);
      std::swap(data_, other.data_);
   }
};

BigBuf a;
BigBuf b;

int main()
{
   std::thread t1([](){
      for(int i = 0; i < 100000; ++i) a.swap(b);
   });
   std::thread t2([](){
      for(int i = 0; i < 100000; ++i) b.swap(a);
   });
   t1.join(); t2.join();

}