//[UTF-8 en/ru]
//problem - https://adventofcode.com/2023/day/2
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream> 
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#define ull unsigned long long
#define RUNS_AMOUNT 30
//#define VERBOSE

using namespace std;
using namespace chrono;
int maxRed = 12;
int maxBlue = 13;
int maxGreen = 14;
const string delims[] = {"Game "s, ": "s, "; "s, ", "};

class cSolve{
	vector<string> lines;
	vector<int> numbers;
	int _result;
	string name;
	high_resolution_clock::duration work_time;
	
	int solveLine(const string& line) {
		string_view sw1(line);
		int len = sw1.size();
		int a1 = sw1.find(delims[0]);
		int a2 = sw1.find(delims[1]);
		string_view sw2(sw1.substr(a1 + delims[0].size(), a2 - delims[0].size()));
		int number; std::from_chars(sw2.data(), sw2.data() + sw2.size(), number);
		cout << line << endl;
		while (a2 < sw1.size() - 1){
			a1 = a2;
			a2 = sw1.find(delims[2], a1 + 1);
			if(a2 == -1) {
				a2 = sw1.size() - 1;
			}
			string_view sw2(sw1.substr(a1 + delims[2].length(), a2 - a1 - delims[2].length()));
			cout << "	" << sw2 << endl;
			int b1 = 0;
			int b2 = 0;
			while (b2 < sw2.size()) {
				b1 = b2;
				b2 = sw2.find(delims[3], b1 + 1);
				if(b2 == -1) {
					b2 = sw2.size();
				}
				string_view sw3(sw2.substr(b1, b2 - b1));
				cout << "		" << sw3 << endl;
			}
		}
		return 0;
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
	cSolve example("example.input");
	cSolve cond("cond.input"); // 
	//time: 
	example.solve();
	cond.solve();
	
	cout << endl;
	// cout << "there are " << example.result() << " in result" << endl;
	// cout << "there are " << cond.result() << " in result" << endl;
	return 0;
}
