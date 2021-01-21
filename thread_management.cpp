// 21 jan 2021 Thursday Week 4
// Managing threads
// A thread must be joined or detached before its destroyed.
// If it goes to the thread destuctor before being joined or detached then it will call std::terminate and will terminate the program abruptly.

/* Exception in thread

std::thread t{func}; // t starts running the function func()
while(do_something())
{
  ...                       // What if this throws an exception. In that case thread object will be destroyed. Because join() is not called yet therefore the thread destructor will call std::terminate
}
t.join();

How to fix this :

try/catch block

try
{
  while(do_something())
  {
    ...  
  }
}
catch(std::exception &e)
{
    t.join();   // join thread t if an exception is caught
}
t.join();

// this fixes the above problem but it is verbose and not elegant

Use RAII idiom 

create a wrapper class which will call the join() in the destructor


*/
#include <iostream>
#include <thread>

void somefunc(int num)
{
  for(int j = 0; j < num; ++j)
    std::cout << j << std::endl;
}

class thread_guard 
{
  std::thread t;
public:
  ~thread_guard()
  {
    std::cout << "in the thread_guard dtor" << std::endl;
    if(t.joinable())
    {
      t.join();
      std::cout << "thread is joined" << std::endl;
    }
  }
  // Constructor std::thread is move only
  explicit thread_guard(std::thread &&th) : t(std::move(th)) { std::cout << "move ctor" <<std::endl; } // move constructor
  
  // Delete copy operators
  thread_guard(const thread_guard&) = delete;                   // copy constructor is deleted
  thread_guard& operator=(const thread_guard&) = delete;        // copy assignment operator is deleted
  
  // Provide move operators
  thread_guard(thread_guard&&) noexcept = default;
  thread_guard& operator=(thread_guard&&) noexcept = default;
};

int main()
{
   std::cout << "main thread started" << std::endl;
   std::thread t{somefunc, 100};
   thread_guard tg(std::move(t));  // tg.t now runs the somefunc()  t doesn't run somefunc() now
   std::cout << "main thread ended" << std::endl;
   return 0;
}












