// 8 dec 2021 Week 50
/*
   Thread-safe initialization using std::call_once and std::once_flag
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

class MySingleton 
{
private:
    static std::once_flag initInstanceFlag;
    static MySingleton* instance;
    MySingleton()  = default;
    ~MySingleton() = default;
public:
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator=(const MySingleton&) = delete;

    static void initSingleton()
    {  
        if(!instance)
        {
            instance = new MySingleton();
            cout << "thread " << std::this_thread::get_id() << ": Singleton initialized\n";
        }
    }

    static MySingleton* getInstance()
    {
        std::call_once(initInstanceFlag, MySingleton::initSingleton);
        //initSingleton();
        return instance;
    }  
};

MySingleton* MySingleton::instance = nullptr;
std::once_flag MySingleton::initInstanceFlag;

void doSomething()
{
    static MySingleton *instance = MySingleton::getInstance();    
    cout << "thread " << std::this_thread::get_id() << ": do Something\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

}

void doSomething1()
{
    static MySingleton *instance = MySingleton::getInstance();    
    cout << "thread " << std::this_thread::get_id() << ": do Something1\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void doSomething2()
{
    static MySingleton *instance = MySingleton::getInstance();    
    cout << "thread " << std::this_thread::get_id() << ": do Something2\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main()
{
    std::thread t1(doSomething);
    std::thread t2(doSomething1);
    std::thread t3(doSomething2);
    std::thread t4(doSomething);
    std::thread t5(doSomething);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}

/*  O/P  when std::call_once not used

thread thread 140599887083264: Singleton initialized
140599878690560: Singleton initialized
thread 140599870297856: Singleton initialized
thread thread 140599878690560: do Something1
thread 140599870297856: do Something2
thread 140599861905152: do Something
140599887083264: do Something
thread 140599853512448: do Something

Singleton intialized 3 times

When std::call_once is used Singleton is initialized only once

thread 140587375937280: Singleton initialized
thread 140587367544576: do Something2
thread 140587384329984: do Something
thread 140587375937280: do Something1
thread 140587359151872: do Something
thread 140587231606528: do Something

*/