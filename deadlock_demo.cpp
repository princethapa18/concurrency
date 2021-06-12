/* Week 24 12 June 2021
   Deadlock demo
   2 philosophers, thinking and eating sushi

   simpler version of a Dining philosophers problem

   Ways to avoid deadlock :
   1. prioritizing locks  (may not work in some situations)
      All threads lock the mutexes in same order
   2. use std::scoped_lock (since C++17)
*/
#include <thread>
#include <mutex>

int sushi_count = 5000;

void philosopher(std::mutex &first_chopstick, std::mutex &second_chopstick)
{
    while(sushi_count > 0)
    {
        std::scoped_lock lock(first_chopstick, second_chopstick); // since C++17 std::scoped_lock destructor releases mutexes in reverse order
        //first_chopstick.lock();
        //second_chopstick.lock();
        if(sushi_count)
            sushi_count--;
        //second_chopstick.unlock();
        //first_chopstick.unlock();
    }
}

int main()
{
    std::mutex chopstick_a, chopstick_b;
    std::thread t1(philosopher, std::ref(chopstick_a), std::ref(chopstick_b));  //leads to deadlock when order is different
    std::thread t2(philosopher, std::ref(chopstick_b), std::ref(chopstick_a));
    /* No deadlock when using locking mutexes in same order
       Prioritizing locks
       chopstick_a --> first(or higher) priority 
       chopstick_b --> second(or lower) priority 
    */
    //std::thread t1(philosopher, std::ref(chopstick_a), std::ref(chopstick_b)); 
    //std::thread t2(philosopher, std::ref(chopstick_a), std::ref(chopstick_b));
    t1.join();
    t2.join();
    printf("The philosophers are done eating");
    return 0;
}

// using scoped_lock with different mutex order still avoids deadlock.