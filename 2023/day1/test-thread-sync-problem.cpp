#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
using namespace chrono;

void func(int a) {
	for (int i = 0; i < 3; i++) {
		std::cout << a + i;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << " ";
	}
	std::cout << "\n";
}

int main() {

	std::thread th1(func, 1);
	std::thread th2(func, 4);

	th1.join();
	th2.join();

	return 0;
}