// 6 jan 2020 Week2

#include <iostream>
#include <thread>

void fun(int n)
{
    std::cout << "\nfun started\n";
    for(int i = 0; i < n; ++i)
      std::cout << i << " " ;
    std::cout << std::endl;
    std::cout << "\nfun ended\n";
}

void fun1(int n)
{
    std::cout << "\nfun1 started\n";
    for(int i = n; i >= 0; --i)
      std::cout << i << " " ;
    std::cout << std::endl;
    std::cout << "\nfun1 ended\n";
}

int main()
{
    std::thread t1; // t1 is not running any thread
    std::thread t2(fun, 100); // t2 is running fun
    // the destructor of thread t2 object will try to destroy it even if the t2 is joinable() therefore the program is terminated (std::terminate is called)
    // O/P
    // terminate called without an active exception
    // Aborted (core dumped)
    //t2.join(); // will wait for thread t2 to finish its execution . // Now after the t2 finishes its execution then the t2 becomes not joinable hence it can be destroyed safely
    std::thread t3(fun1, 50);
    //t3 = std::move(t2) ;// tryning to move from t2 to t3 will call std::terminate as t3 is already running function fun1.
    // terminate called without an active exception
    // Aborted (core dumped)
    t1 = std::move(t2); // move assignment operator. now t1 runs the fun() . t2 is not running any thread.
    std::thread t4(std::move(t3)); //  move constructed. now t4 runs the fun1(). t3 is not running any thread.
     
    std::cout << "\nt1.joinable() " << t1.joinable() << std::endl;
    std::cout << "\nt2.joinable() " << t2.joinable() << std::endl;
    std::cout << "\nt3.joinable() " << t3.joinable() << std::endl;
    std::cout << "\nt4.joinable() " << t4.joinable() << std::endl;
    
    if(t1.joinable()) t1.join();
    if(t4.joinable()) t4.join();
    
    return 0;
}

// O/P
/*
t1.joinable() 1
fun started
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 


t2.joinable() 0

t3.joinable() 0

t4.joinable() 1

fun1 started
50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 

fun1 ended

fun ended
*/