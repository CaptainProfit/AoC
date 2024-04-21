//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <chrono>
#include <fstream>
#include <mutex>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#define ull unsigned long long
#define THREADS_AMOUNT 4
#define RUNS_AMOUNT 30
//#define VERBOSE

using namespace std;
using namespace chrono;

mutex mux;
int common_iterator = 0;
const string digits = "0123456789"s;
const string digitWords[] = {"ExtremelyBadZero"s, "one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s, "eight"s, "nine"s};

static int solveLine(const int id, const string& line) {
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
	mux.lock();
	#ifdef VERBOSE
	cout << id << ": " << digit1 << digit0 << endl;
	#endif
	return digit1*10 + digit0;
}

static void solveThread(const vector<string> *lines, vector<int> *numbers) { //const string& line, int* result
	while(mux.lock(), common_iterator < lines->size()) {
		int task_id = common_iterator;
		common_iterator++;
		const string& line = (*lines)[task_id];
		mux.unlock();
		
		int out = -1;
		try {
			out = solveLine(task_id, line);
		}
		catch(...) {
			#ifdef VERBOSE
			cout << "thread " << this_thread::get_id() << "terminated with task_id " << task_id << endl;
			#endif
			//*result = out;
			mux.unlock();
			throw;
		}
		#ifdef VERBOSE
		cout << "thread " << this_thread::get_id() << " solved task for line " << task_id << endl;
		#endif
		(*numbers)[task_id] = out;
		mux.unlock();
	}
	mux.unlock();
}

void static solvePool(const vector<string>& lines, vector<int>& numbers) {
	thread threads[4];
	for (int i = 0; i < THREADS_AMOUNT; i++) {
		threads[i] = thread(solveThread, &lines, &numbers);
	}

	for (int i = 0; i < THREADS_AMOUNT; i++) {
		threads[i].join();
	}
}

class cSolve{
	vector<string> lines; // input values
	vector<int> numbers;// output values
	int _result;
	string name;
	high_resolution_clock::duration work_time;
	
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

	void solve(){
		high_resolution_clock::duration work_time_avg{0};
		for (int i = 0; i < RUNS_AMOUNT; i++) {
			const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
			common_iterator = 0;
			numbers.resize(lines.size());
			solvePool(lines, numbers);
			_result = accumulate(numbers.begin(), numbers.end(), 0);
			const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};
			work_time = end - start;
			work_time_avg += work_time;
		}

		cout << "test: " << name << endl;
		cout << "result: " << _result << endl;
		cout << "avg time(ms): " << work_time_avg.count()/1000000.0 << endl;
		cout << "--parallel----------" << endl;
	}

	int result() {
		return _result;
	}
};

int main(void) {
	cSolve example("example2.input");
	cSolve cond("cond.input"); // 55686 correct part2
	cSolve cond10("cond10.input"); // 556860 correct part2
	//time: 
	example.solve();
	cond.solve();
	cond10.solve();
	cout << endl;
	// cout << "there are " << example.result() << " in result" << endl;
	// cout << "there are " << cond.result() << " in result" << endl;
	return 0;
}