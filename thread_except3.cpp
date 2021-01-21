// 21 jan 2021 , thursday, week 4
#include <thread>
#include <iostream>

class thread_guard {
    std::thread t;
public:
    explicit thread_guard(std::thread t) : t(std::move(t)) { std::cout << "thread_guard move ctor\n";}   // explicit constructor prevents conversions

    ~thread_guard() {
        std::cout << "thread_guard dtor" << std::endl;
        if (t.joinable())
	{
	  t.join();                   // Join thread if not already joined
	  std::cout << "thread t is joined" << std::endl;
	}
    }

    thread_guard(const thread_guard&) = delete;       // Deleted copy operators prevent copying
    thread_guard& operator=(const thread_guard&) = delete; 
    thread_guard(thread_guard&&) noexcept = default;       // Default move operators to allow moving
    thread_guard& operator=(thread_guard&&) noexcept = default;
};

// Callable object - thread entry point
void hello() {
	std::cout << "Hello, Thread!\n";
}

int main() {
	try {
		//std::thread t{ hello };
		thread_guard tg{ std::thread(hello) };
		throw std::exception();
	}
	catch (std::exception& e) {
		std::cout << "Exception caught\n";
	}
}

/* O/P
thread_guard move ctor
Hello, Thread!
thread_guard dtor
thread t is joined
Exception caught

*/