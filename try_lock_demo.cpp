/* Week 22 , 27 May 2021
 * Try lock demo
 * Two shoppers adding items to a shared notepad
*/

/* std::mutex::try_lock
   bool try_lock();
   Tries to lock the mutex. Returns immediately. On successful lock acquisition returns true, otherwise returns false.
   This function is allowed to fail spuriously and return false even if the mutex is not currently locked by any other thread.

   If try_lock is called by a thread that already owns the mutex, the behavior is undefined.
   Prior unlock() operation on the same mutex synchronizes-with (as defined in std::memory_order) this operation if it returns true. 
   Note that prior lock() does not synchronize with this operation if it returns false. 
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

unsigned int items_on_notepad = 0;
std::mutex pencil;

void shopper(const char* name)
{
    int items_to_add = 0;
    while (items_on_notepad <= 20)
    {
        if(items_to_add && pencil.try_lock())  // add item(s) to shared items_on_notepad
        {
            //pencil.lock();
            items_on_notepad += items_to_add;
            cout << name << " added " << items_to_add << " to notepad.\n";
            items_to_add = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // time spent writing
            pencil.unlock();
        }
        else  // look for other things to buy
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // time spent searching
            items_to_add++;
            cout << name << " found something else to buy.\n";
        }
        
    }
}

int main()
{
    auto start_time = std::chrono::steady_clock::now();
    
    std::thread barron(shopper, "Barron"); 
    std::thread olivia(shopper, "Olivia"); 
    barron.join(); 
    olivia.join();

    auto end_time = std::chrono::steady_clock::now();

    /* Getting number of milliseconds as an integer. */
    //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);   
    //cout << "Elapsed time : " << ms_int.count()/1000 << " seconds.\n";

    /* Getting number of milliseconds as a double. */
    std::chrono::duration<double, std::milli> ms_double = end_time - start_time;
    cout << "Elapsed time : " << ms_double.count()/1000.0 << " seconds.\n";

    return 1;
}

/* O/P 
  Without try lock
  Elapsed time : 6.40843 seconds.

  With try lock
  Elapsed time : 2.30452 seconds.
*/