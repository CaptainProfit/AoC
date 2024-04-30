//[UTF-8 en/ru]
//problem - https://adventofcode.com/2023/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream> 
#include <numeric>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#define ull unsigned long long
#define RUNS_AMOUNT 30
//#define VERBOSE

using namespace std;
using namespace chrono;

class cSolve{
	vector<string> lines;
	vector<int> results;
	int total_result;
	string name;
	high_resolution_clock::duration work_time;

	int solveLine(const string& line) {
		string_view sw1(line);
		int n = line.length();
		int result;
		// ...
		return result;
	}

	public:
	cSolve(string _name) 
		:name(_name){
		total_result = -2;
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
		total_result = -1;
	}
		
	void solve(){
		high_resolution_clock::duration work_time_avg{0};
		for (int i = 0; i < RUNS_AMOUNT; i++) {
			results.clear();
			const time_point<high_resolution_clock> start{std::chrono::high_resolution_clock::now()};
			for (const string& line : lines) {
				results.push_back(solveLine(line));
			}
			total_result = accumulate(results.begin(), results.end(), 0);
			const time_point<high_resolution_clock> end{std::chrono::high_resolution_clock::now()};
			work_time = end - start;
			work_time_avg += work_time;
		}

		cout << "test: " << name << endl;
		cout << "result: " << total_result << endl;
		cout << "avg time(ms): " << work_time_avg.count()/1000000.0 << endl;
		cout << "--------------------" << endl;
	}

	int result() {
		return total_result;
	}
};

int main(void) {
	cSolve example("example.input");
	cSolve cond("cond.input"); 
	example.solve();
	cond.solve();
	cout << endl;
	return 0;
}
