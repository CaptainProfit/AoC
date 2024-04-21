//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
// g++ -O2 day1_2.cpp -o day1_2r.exe
#include <iostream> 
#include <chrono>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

#define ull unsigned long long
#define RUNS_AMOUNT 30
//#define VERBOSE

using namespace std;
using namespace chrono;

const string digits = "0123456789"s;
const string digitWords[] = {"ExtremelyBadZero"s, "one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s, "eight"s, "nine"s};
class cSolve{
	vector<string> lines;
	vector<int> numbers;
	int _result;
	string name;
	high_resolution_clock::duration work_time;
	
	int solveLine(const string& line) {
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
		#ifdef VERBOSE
		cout << numbers.size() << ": " << digit1 << digit0 << endl;
		#endif
		return digit1*10 + digit0;
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
		
	void solve(){
		high_resolution_clock::duration work_time_avg{0};
		for (int i = 0; i < RUNS_AMOUNT; i++) {
			const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
			for (const string& line : lines) {
				numbers.push_back(solveLine(line));
			}
			_result = accumulate(numbers.begin(), numbers.end(), 0);
			const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};
			work_time = end - start;
			work_time_avg += work_time;
		}

		cout << "test: " << name << endl;
		cout << "result: " << _result << endl;
		cout << "avg time(ms): " << work_time_avg.count()/1000000.0 << endl;
		cout << "--------------------" << endl;
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

// TODO вывести результаты в файл.