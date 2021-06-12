/* Week 24 12 June 2021
   abandoned lock demo
*/
#include <thread>
#include <mutex>

int sushi_count = 5000;

void philosopher(std::mutex &chopstick)
{
    while(sushi_count > 0)
    {
        std::scoped_lock lock(chopstick); 
        //chopstick.lock();
        if(sushi_count)
            sushi_count--;
        
        if(sushi_count == 10)
        {
            printf("This philosopher has had enough!\n");
            break;  
        }
        
        //chopstick.unlock();
    }
}

int main()
{
    std::mutex chopstick;
    std::thread t1(philosopher, std::ref(chopstick));
    std::thread t2(philosopher, std::ref(chopstick));   
    t1.join();
    t2.join();
    printf("The philosophers are done eating");
    return 0;
}

// using scoped_lock with different mutex order still avoids deadlock.