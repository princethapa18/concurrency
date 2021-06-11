/* Week 24 11 June 2021
   Shared mutex demo
   Several users reading a calendar, but only a few users updating it
   Needs c++17  [use gcc 7.3.1]
   g++ -std=c++17 -lpthread shared_mutex_demo.cpp -o shared_mutex_demo

    When using mutex
    if a thread acquires a mutex then other thread will have to wait for this thread to unlock mutex.
    If these threads only want to read a data then waiting time is unnecessary 
    Therefore shared mutex comes handy
    It allows shared ownership when only reading a data
    2 or more threads can acquire shared mutex when only reading 
    When writing only 1 thread locks the mutex.

*/

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <shared_mutex>

char WEEKDAYS[7][10]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int today=0;
//std::mutex marker;
std::shared_mutex marker_shared;

void calendar_reader(const int id) 
{
    for (int i=0; i < 7; i++)
    {
        //marker.lock();
        marker_shared.lock_shared();
        printf("Reader-%d sees today is %s\n", id, WEEKDAYS[today]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //marker.unlock();
        marker_shared.unlock_shared();
    }
}

void calendar_writer(const int id) 
{
    for (int i=0; i < 7; i++)
    {
        //marker.lock();
        marker_shared.lock();
        today = (today + 1) % 7;
        printf("Writer-%d updated date to %s\n", id, WEEKDAYS[today]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //marker.unlock();
        marker_shared.unlock();
    }
}

int main()
{
    auto strt = std::chrono::high_resolution_clock::now();

    // create 10 reader threads ... but only 2 writer threads
    std::array<std::thread, 10> readers;
    for(unsigned int i=0; i < readers.size(); i++)
        readers[i] = std::thread(calendar_reader, i);

    std::array<std::thread, 2> writers;
    for(unsigned int i=0; i < writers.size(); i++)
        writers[i] = std::thread(calendar_writer, i);

    // wait for readers and writers to finish
    for(unsigned int i=0; i < readers.size(); i++)
        readers[i].join();

    for(unsigned int i=0; i < writers.size(); i++)
        writers[i].join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = end - strt;
    std::cout << "Executin time : " << ms_double.count() << " ms";

    return 0;
}

/*  With normal mutex

Reader 3 sees today is Sunday
Reader 3 sees today is Sunday
Reader 3 sees today is Sunday
Reader 3 sees today is Sunday
Reader 3 sees today is Sunday
Reader 3 sees today is Sunday
Reader 3 sees today is Sunday
Reader 9 sees today is Sunday
Reader 9 sees today is Sunday
Reader 9 sees today is Sunday
Reader 9 sees today is Sunday
Reader 9 sees today is Sunday
Reader 9 sees today is Sunday
Reader 9 sees today is Sunday
Reader 6 sees today is Sunday
Reader 6 sees today is Sunday
Reader 6 sees today is Sunday
Reader 6 sees today is Sunday
Reader 6 sees today is Sunday
Reader 6 sees today is Sunday
Reader 6 sees today is Sunday
Reader 1 sees today is Sunday
Reader 1 sees today is Sunday
Reader 1 sees today is Sunday
Reader 1 sees today is Sunday
Reader 1 sees today is Sunday
Reader 1 sees today is Sunday
Reader 1 sees today is Sunday
Writer 1 updated date to Monday
Writer 1 updated date to Tuesday
Writer 1 updated date to Wednesday
Writer 1 updated date to Thursday
Writer 1 updated date to Friday
Writer 1 updated date to Saturday
Writer 1 updated date to Sunday
Reader 0 sees today is Sunday
Reader 0 sees today is Sunday
Reader 0 sees today is Sunday
Reader 0 sees today is Sunday
Reader 0 sees today is Sunday
Reader 0 sees today is Sunday
Reader 0 sees today is Sunday
Reader 8 sees today is Sunday
Reader 8 sees today is Sunday
Reader 8 sees today is Sunday
Reader 8 sees today is Sunday
Reader 8 sees today is Sunday
Reader 8 sees today is Sunday
Reader 8 sees today is Sunday
Reader 4 sees today is Sunday
Reader 4 sees today is Sunday
Reader 4 sees today is Sunday
Reader 4 sees today is Sunday
Reader 4 sees today is Sunday
Reader 4 sees today is Sunday
Reader 4 sees today is Sunday
Reader 7 sees today is Sunday
Reader 7 sees today is Sunday
Reader 7 sees today is Sunday
Reader 7 sees today is Sunday
Reader 7 sees today is Sunday
Reader 7 sees today is Sunday
Reader 7 sees today is Sunday
Reader 5 sees today is Sunday
Reader 5 sees today is Sunday
Reader 5 sees today is Sunday
Reader 5 sees today is Sunday
Reader 5 sees today is Sunday
Reader 5 sees today is Sunday
Reader 5 sees today is Sunday
Reader 2 sees today is Sunday
Reader 2 sees today is Sunday
Reader 2 sees today is Sunday
Reader 2 sees today is Sunday
Reader 2 sees today is Sunday
Reader 2 sees today is Sunday
Reader 2 sees today is Sunday
Writer 0 updated date to Monday
Writer 0 updated date to Tuesday
Writer 0 updated date to Wednesday
Writer 0 updated date to Thursday
Writer 0 updated date to Friday
Writer 0 updated date to Saturday
Writer 0 updated date to Sunday
Executin time : 8424.86 ms 

*/   

/* Using shared_mutex

Reader-4 sees today is Sunday
Reader-2 sees today is Sunday
Reader-1 sees today is Sunday
Reader-5 sees today is Sunday
Reader-6 sees today is Sunday
Reader-8 sees today is Sunday
Reader-9 sees today is Sunday
Reader-7 sees today is Sunday
Reader-0 sees today is Sunday
Reader-3 sees today is Sunday
Reader-4 sees today is Sunday
Reader-6 sees today is Sunday
Reader-0 sees today is Sunday
Reader-7 sees today is Sunday
Reader-2 sees today is Sunday
Reader-9 sees today is Sunday
Reader-1 sees today is Sunday
Reader-5 sees today is Sunday
Reader-8 sees today is Sunday
Reader-3 sees today is Sunday
Reader-2 sees today is Sunday
Reader-1 sees today is Sunday
Reader-7 sees today is Sunday
Reader-5 sees today is Sunday
Reader-9 sees today is Sunday
Reader-4 sees today is Sunday
Reader-0 sees today is Sunday
Reader-6 sees today is Sunday
Reader-3 sees today is Sunday
Reader-8 sees today is Sunday
Reader-7 sees today is Sunday
Reader-6 sees today is Sunday
Reader-2 sees today is Sunday
Reader-1 sees today is Sunday
Reader-3 sees today is Sunday
Reader-8 sees today is Sunday
Reader-0 sees today is Sunday
Reader-9 sees today is Sunday
Reader-5 sees today is Sunday
Reader-4 sees today is Sunday
Reader-7 sees today is Sunday
Reader-8 sees today is Sunday
Reader-6 sees today is Sunday
Reader-3 sees today is Sunday
Reader-1 sees today is Sunday
Reader-2 sees today is Sunday
Reader-9 sees today is Sunday
Reader-0 sees today is Sunday
Reader-4 sees today is Sunday
Reader-5 sees today is Sunday
Reader-7 sees today is Sunday
Reader-6 sees today is Sunday
Reader-2 sees today is Sunday
Reader-1 sees today is Sunday
Reader-8 sees today is Sunday
Reader-3 sees today is Sunday
Reader-9 sees today is Sunday
Reader-0 sees today is Sunday
Reader-4 sees today is Sunday
Reader-5 sees today is Sunday
Reader-6 sees today is Sunday
Reader-8 sees today is Sunday
Reader-5 sees today is Sunday
Reader-2 sees today is Sunday
Reader-3 sees today is Sunday
Reader-0 sees today is Sunday
Reader-4 sees today is Sunday
Reader-1 sees today is Sunday
Reader-9 sees today is Sunday
Reader-7 sees today is Sunday
Writer-0 updated date to Monday
Writer-0 updated date to Tuesday
Writer-0 updated date to Wednesday
Writer-0 updated date to Thursday
Writer-0 updated date to Friday
Writer-0 updated date to Saturday
Writer-0 updated date to Sunday
Writer-1 updated date to Monday
Writer-1 updated date to Tuesday
Writer-1 updated date to Wednesday
Writer-1 updated date to Thursday
Writer-1 updated date to Friday
Writer-1 updated date to Saturday
Writer-1 updated date to Sunday
Executin time : 2107.91 ms

*/