/* 8 Dec 2021 Week 50
   Thread-safe initialization using Static variables with block scope


*/
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class MySingleton 
{
private:
    MySingleton()  = default;
    ~MySingleton() = default;
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator=(const MySingleton&) = delete;
public:
    static MySingleton& getInstance()
    {
        static MySingleton instance;
        return instance;
    }  
};

void doSomething()
{
    cout << "thread " << std::this_thread::get_id() << ": do Something " << &MySingleton::getInstance() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

}

void doSomething1()
{
    cout << "thread " << std::this_thread::get_id() << ": do Something1 " << &MySingleton::getInstance() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void doSomething2()
{
    cout << "thread " << std::this_thread::get_id() << ": do Something2 " << &MySingleton::getInstance() << "\n";
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