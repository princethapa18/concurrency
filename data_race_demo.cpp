/* Week 22 , 24 May 2021
 * Two shoppers adding items to a shared notepad
 *
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

std::mutex pencil;
unsigned int garlic_count = 0;

void shopper(int counter)
{
    //pencil.lock();
    for(int i = 0; i < counter; i++)
    {
        cout << "Shopper " << std::this_thread::get_id() << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        pencil.lock();
        garlic_count++; // because this is the only critical section which needs to be protected. We only need to protect this.
        pencil.unlock();  // Now execution time is reduced to 2502.12 ms
    }
    //pencil.unlock();
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int counter = 5; // 100000
    std::thread barron(shopper, 5); // barron starts . It enters a runnable state
    std::thread olivia(shopper, 5); // olivia starts.   ,,
    barron.join(); // main thread waits for barron to finish
    olivia.join(); //  ,,             ,,    olivia to finish
    cout << "We should buy " << garlic_count << std::endl;

    auto t2 = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1);

    /* Getting number of milliseconds as an integer. */
    //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    //std::cout << ms_int.count() << "ms\n";
    std::cout << "Executin time : " << ms_double.count() << " ms";

    return 1;
}

// Output when there is datarace
// garlic_count should be 200000
// but its different each time
//g++ -std=c++11 data_race_demo.cpp -lpthread -o data_race_demo
//./data_race_demo 
//We should buy 159246
//./data_race_demo 
//We should buy 129544
//./data_race_demo 
//We should buy 121250

/* after using mutex
./data_race_demo 
counter = 100000
We should buy 200000
[thapa@gns101 data_races]$ ./data_race_demo 
We should buy 200000
[thapa@gns101 data_races]$ ./data_race_demo 
We should buy 200000
*/

/*  Only lock the critical section so as to reduce the waiting time.

    pencil.lock();
    for(int i = 0; i < counter; i++)
    {
        cout << "Shopper " << std::this_thread::get_id() << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        garlic_count++; // This is the only critical section which needs to be protected. 
    }
    pencil.unlock();

    In this case pencil aquires a lock for thread barron. 
    Sleeps for 5 seconds then 
    increments garlic_count;

    When the thread barron is thinking (i.e asleep for 5 sec) the thread olivia can't increment garlic_count and 
    unnecessarily has to wait even when the thread barron is not modifying garlic_count.
    Which increases the execution time.
    The total execution time is 5 secs.

    
    for(int i = 0; i < counter; i++)
    {
        cout << "Shopper " << std::this_thread::get_id() << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        pencil.lock();
        garlic_count++; // This is the only critical section which needs to be protected. 
        pencil.unlock();
    }
   
    Now mutex pencil acquires a lock on thread barron. The thread barron overwrites the garlic_count
    then the mutex pencil release the lock on thread barron.
    Now olivia can modify the garlic_count.
    Therefore the thread olivia doesn't have to wait when the thread barron is thinking.

    Therefore the execution time is better for this i.e 2.5 secs.

*/