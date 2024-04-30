//[UTF-8 en/ru]
//problem - https://adventofcode.com/2023/day/3
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
#define RUNS_AMOUNT 1
//#define VERBOSE

using namespace std;
using namespace chrono;

string digits_str = "01234567890"s;

class cSolve{
	vector<string> lines;
	vector<int> results;
	int total_result;
	int width, height;
	string name;
	high_resolution_clock::duration work_time;

	pair<int, int> findNumber(const string& line, int offset = 0) {
		int j_begin = line.find_first_of(digits_str, offset);
		int j_end = line.find_first_not_of(digits_str, j_begin);
		if (j_end == -1) {
			j_end = line.length();
		}
		return {j_begin,j_end};
	}

	vector<int> checkArea (int i, int j_left, int j_right) {
		vector<int> result;
		if (i < 0 || i >= height) {
			return {};
		}
		const string& line = lines[i];
		int offset = max(0, j_left);
		for(; offset > 0 && '0' <= line[offset] && line[offset] <= '9'; offset--);
		
		for(auto [j_begin, j_end] = findNumber(line, offset);
				j_begin != -1 && j_begin < j_end && j_begin <= j_right && j_end-1 >= j_left;
				offset = j_end, tie(j_begin, j_end) = findNumber(line, offset)) {
			int number;
			from_chars_result conversion_report = from_chars(&line[j_begin], &line[j_end], number);
			if (conversion_report.ec == std::errc{}) {
				result.push_back(number);
			}
		}
		return result;
	}

	int solveLine(int i) {
		string& line = lines[i];
		int n = line.length();
		int result = 0;
		for(int offset = 0, j_star = line.find("*"s, offset); j_star != -1; offset = j_star + 1, j_star = line.find("*"s, offset)) {
			
			vector<int> numbers_around;
			for (int number: checkArea(i, j_star - 1, j_star - 1))
				numbers_around.emplace_back(number);
			for (int number: checkArea(i, j_star + 1, j_star + 1))
				numbers_around.emplace_back(number);
			for (int number: checkArea(i - 1, j_star - 1, j_star + 1))
				numbers_around.emplace_back(number);
			for (int number: checkArea(i + 1, j_star - 1, j_star + 1))
				numbers_around.emplace_back(number);

			if (numbers_around.size() == 2) {
				cout << "\t" << i << ", " << j_star << ": " << numbers_around[0] << ", " << numbers_around[1] << endl;
			 	result += numbers_around[0] * numbers_around[1];
			}
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
	// 58910367 - too low
	// 62339684 - too low
	// 69527306 - correct
	example.solve();
	cond.solve();
	cout << endl;
	return 0;
}
