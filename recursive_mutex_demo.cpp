/* Week 22 , 25 May 2021
 * Two shoppers adding garlic and potatoes to a shared notepad
 * Recursive mutex
*/
#include <iostream>
#include <thread>
#include <mutex>
//#include <chrono>

using namespace std;
std::recursive_mutex recursive_pencil;
std::mutex pencil;
unsigned int garlic_count = 0;
unsigned int potato_count = 0;

void add_garlic()
{
    //pencil.lock();
    recursive_pencil.lock();
    garlic_count++;
    //pencil.unlock();
    recursive_pencil.unlock();
}

void add_potato()
{
    //pencil.lock();
    recursive_pencil.lock();
    potato_count++;
    add_garlic();      // pencil locked twice in a row. When trying to unlock garlic. It can't be unlocked as it is waiting for potato to unlock but potato also can't 
                       // be unlocked as it is waiting for garlic to get unlocked.
    //pencil.unlock();
    recursive_pencil.unlock();
}

void shopper(int counter)
{
    for(int i = 0; i < counter; i++)
    {
        add_garlic();
        add_potato();
    }
}

int main()
{
    //auto t1 = std::chrono::high_resolution_clock::now();
    int counter = 10000;
    std::thread barron(shopper, counter); 
    std::thread olivia(shopper, counter); 
    barron.join(); // main thread waits for barron to finish
    olivia.join(); //  ,,             ,,    olivia to finish
    cout << "We should buy " << garlic_count << " garlics\n";
    cout << "We should buy " << potato_count << " potatoes\n";

    //auto t2 = std::chrono::high_resolution_clock::now();
    //auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1);

    /* Getting number of milliseconds as an integer. */
    //auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    //std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    //std::cout << ms_int.count() << "ms\n";
    //std::cout << "Executin time : " << ms_double.count() << " ms";

    return 1;
}

