//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
// g++ -O2 day1_2-thread2.cpp -o day1_2-thread2r.exe
#include <iostream> 
#include <chrono>
#include <fstream>
#include <future>
#include <numeric>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#define ull unsigned long long
#define RUNS_AMOUNT 1

using namespace std;
using namespace chrono;

const string digits = "0123456789"s;
const string digitWords[] = {"ExtremelyBadZero"s, "one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s, "eight"s, "nine"s};

/*static int solveLine(const int id, const string& line) {
	//line.find_last_of()		
	int a0 = line.find_last_of(digits);
	int b0 = -1;
	int digit0 = 0;
	for (int i = 0; i < 10; i++) {
		int b = line.rfind(digitWords[i]);
		if (b > b0) {
			b0 = b;
			digit0 = i;
		}
	}
	if (a0 > b0) {
		digit0 = line[a0] - '0';
	} 

	//line.find_first_of()
	int a1 = line.find_first_of(digits);
	int b1 = line.length();
	int digit1 = 0;
	for (int i = 0; i < 10; i++) {
		int b = line.find(digitWords[i]);
		if (b >= 0 && b < b1) {
			b1 = b;
			digit1 = i;
		}
	}
	if (a1 >= 0 && a1 < b1) {
		digit1 = line[a1] - '0';
	}
	return digit1*10 + digit0;
}//*/
static int solveLine(const int id, const string& line) {
	//line.find_last_of()		
	int digit1 = 0;
	int digit0 = 0;
	bool is_first = true;
	for (int i = 0; i < line.length(); i++) {
		if ('0' <= line[i] && line[i] <= '9') {
			if (is_first) {
				digit1 = line[i] - '0';
				is_first = false;
			}
			digit0 = i;
		}
	}
	return digit1*10 + digit0;
}
high_resolution_clock::duration thread_work_times[20];
static void solveThread(int id, const vector<string> *lines,
						 vector<int> *numbers,
						 int offset,
						 int size) { //const string& line, int* result
	SetThreadAffinityMask(GetCurrentThread(), 1 << id);
	const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
	for (int task_id = offset; task_id < offset + size; task_id++) {
		const string& line = (*lines)[task_id];
		(*numbers)[task_id] = solveLine(task_id, line);
	}
	const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};
	thread_work_times[id] = end - start;
}

void static solvePool(int threads_amount, const vector<string>& lines, vector<int>& numbers) {
	thread* threads = new thread[threads_amount];
	int len = lines.size();
	int ofs = 0;
	int len_thread = len/threads_amount;
	for (int i = 0; i < threads_amount; i++) {
		threads[i] = thread(solveThread, i, &lines, &numbers, len_thread*i, min(len_thread, len - len_thread*i ));
	}

	for (int i = 0; i < threads_amount; i++) {
		threads[i].join();
	}
	delete[] threads;
}

template<typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end)
{
	auto len = end - beg;
	if (len < 2000)
		return std::accumulate(beg, end, 0);

	RandomIt mid = beg + len / 2;
	auto handle = std::async(std::launch::async,
							parallel_sum<RandomIt>, mid, end);
	int sum = parallel_sum(beg, mid);
	return sum + handle.get();
}

class cSolve{
	vector<string> lines; // input values
	vector<int> numbers;// output values
	int _result;
	string name;
	high_resolution_clock::duration work_time;
	
	double getMaxThreadTime(int threads_amount) {
		int id = 0;
		for (int i = 0; i < threads_amount; i++) {
			if (thread_work_times[id].count() < thread_work_times[i].count()) {
				id = i;
			}
		}
		return thread_work_times[id].count();
	}

	double getAverageThreadTime(int threads_amount) {
		high_resolution_clock::duration average_thread_work_time{0};
		for  (int i = 0; i < threads_amount; i++) {
			average_thread_work_time+=thread_work_times[i];
		}
		return average_thread_work_time.count() / (double)threads_amount;
	}

	public:
	cSolve(string _name) 
		:name(_name){
		_result = -2;
		try {
			ifstream ifstr(name, ios::binary);
			for (string line; getline(ifstr, line);) {
				lines.push_back(line);
			}
			ifstr.close();
		}
		catch(...) {
			cout << "something gone wrong in reading data" << endl;
			throw;
		}
		_result = -1;
	}

	void solve(int threads_amount ){
		high_resolution_clock::duration work_time_avg{0};
		for (int i = 0; i < RUNS_AMOUNT; i++) {
			const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
			numbers.resize(lines.size());
			solvePool(threads_amount, lines, numbers);
			//_result = accumulate(numbers.begin(), numbers.end(), 0);
			_result = parallel_sum(numbers.begin(), numbers.end());
			const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};
			work_time = end - start;
			work_time_avg += work_time;
		}

		cout << "--day 1: " << threads_amount << " threads---------" << endl;
		cout << "test: " << name << endl;
		cout << "result: " << _result << endl;
		cout << "time_avg(ms): " << work_time_avg.count()/1000000.0/RUNS_AMOUNT << endl;
		cout << "processes_time_max(ms): " << getMaxThreadTime(threads_amount)/1000000.0/RUNS_AMOUNT << endl;
		cout << "processes_time_avg(ms): " << getAverageThreadTime(threads_amount)/1000000.0/RUNS_AMOUNT << endl;
	}

	int result() {
		return _result;
	}
};

int main(void) {
	//cSolve example("example2.input");
	//cSolve cond("cond.input"); // 55686 correct part2
	cSolve cond1000("cond20_100.input"); // 556860 correct part2
	//time: 
	//example.solve();
	//cond.solve();
	cond1000.solve(1);
	cond1000.solve(4);
	cout << endl;
	// cout << "there are " << example.result() << " in result" << endl;
	// cout << "there are " << cond.result() << " in result" << endl;
	return 0;
}