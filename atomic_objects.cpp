/* Week 22 , 24 May 2021
 * Atomic objects
 * std::atomic
 *
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std;

std::mutex pencil;
//unsigned int garlic_count = 0;
std::atomic<unsigned int> garlic_count(0);  // atomic_uint

void shopper(int counter)
{
    //pencil.lock();
    for(int i = 0; i < counter; i++)
    {
        //cout << "Shopper " << std::this_thread::get_id() << " is thinking." << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //pencil.lock();
        garlic_count++; // because this is the only critical section which needs to be protected. We only need to protect this.
        //pencil.unlock();  // Now execution time is reduced to 2502.12 ms
    }
    //pencil.unlock();
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int counter = 100000; // 100000
    std::thread barron(shopper, counter); // barron starts . It enters a runnable state
    std::thread olivia(shopper, counter); // olivia starts.   ,,
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

/*

We don't have to use mutex here to avoid data race.
Instead we used an atomic object.
We can use atomic objects when we need to share something simple like integer, bool, char etc.

*/