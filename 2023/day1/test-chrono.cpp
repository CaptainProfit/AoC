#include <chrono>
#include <iostream>
using namespace std;
using namespace chrono;

long Fibonacci(unsigned n)
{
    return n < 2 ? n : Fibonacci(n - 1) + Fibonacci(n - 2);
}

int main()
{
    const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
    long fb = Fibonacci(42);
    const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};;
    const high_resolution_clock::duration elapsed_ns{end - start};

    std::cout << "Fibonacci(42): " << fb << "\nElapsed time: ";
//  std::cout << elapsed_seconds.count() << "s\n"; // Before C++20
    std::cout << elapsed_ns.count() << '\n'; // C++20's chrono::duration operator<<
}