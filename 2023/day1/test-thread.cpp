#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
using namespace chrono;

void sleep(int duration) {
	// simulate expensive operation
	std::this_thread::sleep_for(std::chrono::seconds(duration));
}

int main(void) {
	std::cout << "starting first thread...\n";
	std::thread th1(sleep, 1);

	std::cout << "starting second thread...\n";
	std::thread th2(sleep, 2);

	std::cout << "waiting for threads to finish...\n";

	th1.join();
	std::cout << "the first thread finished\n";

	th2.join();
	std::cout << "the second thread finished\n";

	std::cout << "done\n";

	return 0;
}