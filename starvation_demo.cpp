/* Week 24 12 June 2021
   Resource starvation demo
*/
#include <thread>
#include <mutex>

int sushi_count = 5000;

void philosopher(std::mutex &chopstick)
{
    int sushi_eaten = 0;
    while(sushi_count > 0)
    {
        std::scoped_lock lock(chopstick); 
        if(sushi_count)
        {
            sushi_count--;
            sushi_eaten++;
        }
        
        printf("Philosopher %d ate %d.\n", std::this_thread::get_id(), sushi_eaten);
    }
}

int main()
{
    std::mutex chopstick;
    std::array<std::thread, 200> philosophers; // when there are only 2 threads so there is no starvation
    // But when there are a lot of threads then there maybe some threads which haven't eaten any sushi (basically starvation)
    for(size_t i = 0; i < philosophers.size(); i++)
        philosophers[i] = std::thread(philosopher, std::ref(chopstick));

    for(size_t i = 0; i < philosophers.size(); i++)
        philosophers[i].join();

    printf("The philosophers are done eating");
    return 0;
}

