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
#include <set>
#include <string>
#include <string_view>
#include <vector>

#define ull unsigned long long
#define RUNS_AMOUNT 30
//#define VERBOSE

using namespace std;
using namespace chrono;

const set<char> not_interesting_symbols {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '\n', '\r'};
string digits_str = "01234567890"s;
class cSolve{
	vector<string> lines;
	vector<int> results;
	int total_result;
	int width, height;
	string name;
	high_resolution_clock::duration work_time;

	bool checkSymbol (int i, int j_begin, int j_last) {
		if (i < 0 || i >= height) {
			return false;
		}
		for (int j = max(j_begin, 0); j <= j_last && j < width; j++) {
			if (not_interesting_symbols.find(lines[i][j]) ==
				not_interesting_symbols.end()) {
				return true;
			}
		}
		return false;
	}

	pair<int, int> findNumber(const string& line, int offset = 0) {
		int j_begin = line.find_first_of(digits_str, offset);
		int j_end = line.find_first_not_of(digits_str, j_begin);
		if (j_end == -1) {
			j_end = line.length();
		}
		return {j_begin,j_end};
	}

	int solveLine(int i) {
		string& line = lines[i];
		int n = line.length();
		int offset = 0;
		int result = 0;
		for(auto [j_begin, j_end] = findNumber(line, offset); j_begin != -1 && j_begin < j_end; tie(j_begin, j_end) = findNumber(line, offset)) {
			int number;
			std::from_chars(&line[j_begin], &line[j_end], number);
			if (checkSymbol(i, j_begin - 1, j_begin - 1) ||
			checkSymbol(i, j_end, j_end) ||
			checkSymbol(i - 1, j_begin - 1, j_end) ||
			checkSymbol(i + 1, j_begin - 1, j_end)) {
				result += number;
			}
			offset = j_end;
		}
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
			height = lines.size();
				if (height > 0) {
				width = lines[0].length();
			}
			else {
				cout << "wrong input" << endl;
				throw;
			}
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
			for (int i = 0; i < lines.size(); i++) {
				results.push_back(solveLine(i));
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
	// 518930 - too low
	example.solve();
	cond.solve();
	cout << endl;
	return 0;
}
