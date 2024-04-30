//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
// g++ -pthread day1_2-thread3.cpp -o day1_2-thread3r.exe 
// g++ -O2 day1_2-thread3.cpp -o day1_2-thread3r.exe 
// clang -O2 day1_2-thread3.cpp -o day1_3-thread3r.exe 
// ./day1_2-thread3r.exe

#include <iostream> 
#include <chrono>
#include <fstream>
#include <future>
#include <mutex>
#include <numeric>
#include <cstdlib>
//#include <string>
#include <thread>
#include <vector>

#define ull unsigned long long
#ifndef THREADS_AMOUNT
#define THREADS_AMOUNT 4
#endif
#define RUNS_AMOUNT 30
//#define VERBOSE

using namespace std;
using namespace chrono;

mutex mux;
const char digits[] = "0123456789";
const char* digitWords[] = {"ExtremelyBadZero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
const int digitWordsLengths[] = {sizeof("ExtremelyBadZero"), sizeof("one"), sizeof("two"), sizeof("three"), sizeof("four"), sizeof("five"), sizeof("six"), sizeof("seven"), sizeof("eight"), sizeof("nine")};
char* linesBuffer;
int* linesLengths;
int* linesStarts;

static int my_find(const int id, const int tmp_id) {
	// return line.find(digitWords[i]);
	int n = linesLengths[id];
	int m = digitWordsLengths[tmp_id];
	for (int i = 0; i < n - m; i++) {
		bool isEqual = true;
		for (int j = 0; j < m ; j++) {
			if (linesBuffer[linesStarts[id]+ i + j] != digitWords[tmp_id][j]) {
				isEqual = false;
				break;
			}
		}
		if (isEqual) {
			return i;
		}
	}
	return -1;
}

static int my_rfind(const int id, const int tmp_id) {
	// return line.rfind(digitWords[i]);
	int n = linesLengths[id];
	int m = digitWordsLengths[tmp_id];
	for (int i = n - m - 1; i >= 0; i--) {
		bool isEqual = true;
		for (int j = 0; j < m ; j++) {
			if (linesBuffer[linesStarts[id]+ i + j] != digitWords[tmp_id][j]) {
				isEqual = false;
				break;
			}
		}
		if (isEqual) {
			return i;
		}
	}
	return -1;
}

static int solveLine(const int id) {
	//line.find_last_of()		
	int a0;
	int n = linesLengths[id];
	char* line = &linesBuffer[linesStarts[id]];
	//line.find_last_of(digits);
	for(a0 = n - 1 ; a0 >= 0; a0--) {
		if(line[a0] >= '0' && line[a0] <= '9') {
			break;
		}
	}
	
	int b0 = -1;
	int digit0 = 0;
	for (int i = 0; i < 10; i++) {
		
		int b = my_rfind(id, i);
		if (b > b0) {
			b0 = b;
			digit0 = i;
		}
	}
	if (a0 > b0) {
		digit0 = line[a0] - '0';
	} 

	//line.find_first_of()
	//int a1 = line.find_first_of(digits);
	int a1;
	for(a1 = 0; a1 < n; a1++) {
		if(line[a1] >= '0' && line[a1] <= '9') {
			break;
		}
	}
	int b1 = n;
	int digit1 = 0;
	for (int i = 0; i < 10; i++) {
		
		int b = my_find(id, i);
		if (b >= 0 && b < b1) {
			b1 = b;
			digit1 = i;
		}
	}
	if (a1 >= 0 && a1 < b1) {
		digit1 = line[a1] - '0';
	}
	#ifdef VERBOSE
	mux.lock();
	cout << id << ": " << digit1 << digit0 << endl;
	#endif
	return digit1*10 + digit0;
}

static void solveThread(int first_id,
 						 int size,
						 int* result
						) { //const string& line, int* result
	int acc = 0;
	for (int task_id = 0; task_id < size; task_id++) {
		acc += solveLine(first_id + task_id);
	}
	*result = acc;
}

int static solvePool(const vector<string>& lines, vector<int>& numbers) {
	thread threads[4];
	int len = lines.size();
	int len_thread = len/THREADS_AMOUNT;
	int results[]={0, 0, 0, 0};
	for (int i = 0; i < THREADS_AMOUNT; i++) {
		threads[i] = thread(solveThread, len_thread*i, min(len_thread, len - len_thread*i), results + i );
	}

	for (int i = 0; i < THREADS_AMOUNT; i++) {
		threads[i].join();
	}
	return results[0] + results[1] + results[2] + results[3];
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
		int total = 0;
		linesStarts = new int[lines.size()];
		linesLengths = new int[lines.size()];
		for(int i = 0; i < lines.size(); i++) {
			linesStarts[i] = total;
			linesLengths[i] = lines[i].length();
			total += lines[i].length() + 1;
		}
		linesBuffer = new char[total];
		int index = 0;
		for(int i = 0; i < lines.size(); i++) {
			memcpy(linesBuffer + linesStarts[i], lines[i].c_str(), linesLengths[i]);
			*(linesBuffer + linesStarts[i] + linesLengths[i]) = 0;
		}
		for (int i = 0; i < RUNS_AMOUNT; i++) {
			const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
			numbers.resize(lines.size());
			_result = solvePool(lines, numbers);
			//_result = accumulate(numbers.begin(), numbers.end(), 0);
			//_result = parallel_sum(numbers.begin(), numbers.end());
			const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};
			work_time = end - start;
			work_time_avg += work_time;
		}
		delete linesLengths;
		delete linesStarts;
		delete linesBuffer;
		cout << "test: " << name << endl;
		cout << "result: " << _result << endl;
		cout << "avg time(ms): " << work_time_avg.count()/1000000.0 << endl;
		cout << "--parallel" << THREADS_AMOUNT << "---------" << endl;
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