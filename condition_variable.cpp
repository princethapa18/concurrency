/* 27 Feb 2022 Week 09  std::condition_variable


*/

#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

// global variables
mutex mut;
condition_variable cv;
string sdata{"Empty"};
bool condition{false};

// waiting thread
void reader()
{
    cout << "Reading thread starts ...\n";
    unique_lock<std::mutex> lk(mut);         // Acquire lock
    //cv.wait(lk);                           // Wait for condition variable to be notified
    // to fix the "lost wakeup" and "spurious wakeup"
    cv.wait(lk, [](){return condition;});   // initially condition is false so it won't block the thread. When the condition is true then only it blocks the thread
    cout << "Data is " << sdata << endl;    // wake up and use the new value
    cout << "Reading thread exits\n";
}
// modifying thread
void writer()
{
    cout << "Writing thread starts ...\n";
    {
        lock_guard<std::mutex> lg(mut);   // Acquire lock
        std::this_thread::sleep_for(1s);  // Pretend to be busy ...
        sdata = "Populated";              // Modify the shared data
        condition = true;
    }                                     // Release the lock
    cout << "Writing thread notifies\n";
    cv.notify_one();                      // Notify the contion variable
    cout << "Writing thread exits\n";
}
// We put the lock_guard and the critical seciton in their own scope
// to make sure that lock is released before notifying

int main()
{
    cout << "Data is " << sdata << endl;
    thread write{writer};
    std::this_thread::sleep_for(3s); 
    thread read{reader};
    write.join();
    read.join();
}

/*
------- Lost Wakeup ----------------
    thread write{writer};
    std::this_thread::sleep_for(3s); 
    thread read{reader};

Data is Empty
Writing thread starts ...
Writing thread notifies
Writing thread exits
Reading thread starts ...

By default, wait() will block until a notification is recieved
Here write thread notifies before the read thread calls wait()
Therefore the read thread is blocked forever.

------ Spurious (false) Wakeup --------------
Occasionaly , the waiting thread will be woken up even though the waiting thread has not
notified the condition variable.

----- Don't wake without a condition -----------------
Here is the rationale for the rule: 
"A wait without a condition can miss a wakeup or wake up simply to find that there is no work to do." 
What does that mean? Condition variables can be victims of two very serious issues: lost wakeup and spurious wakeup. 
The key concern about condition variables is that they have no memory.


Fixing the above issues using a wait condition
Data is Empty
Writing thread starts ...
Writing thread notifies
Writing thread exits
Reading thread starts ...
Data is Populated
Reading thread exits




*/




