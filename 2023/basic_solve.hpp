#pragma once
#include <charconv>
#include <chrono>
#include <fstream>
#include <iostream> 
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

//#define VERBOSE
#define ull unsigned long long
#define RUNS_AMOUNT 1

using namespace std;
using namespace chrono;

template<typename t_string, typename u_string>
vector<string_view> split (const t_string& str,const u_string& tmp, bool skipEmpty = true) {
	string_view _str(str);
	string_view _tmp(tmp);
	vector<string_view> result;
	int leftIter = 0;
	for (int iter = 0; iter < _str.length(); iter++) {
		if (_tmp.find(_str[iter]) != _tmp.end()) {
			if (!skipEmpty || leftIter != iter) {
				result.push_back(_str.substr(leftIter, leftIter - iter));
			}
			leftIter = iter + 1;
		}
	}
	return result;
}

template<typename t_string>
int toInt (const t_string& str) {
	string_view _str(str);
	int value;
	from_chars_result conversion_report = from_chars(_str.data(), _str.data() + _str.length(), value);
	if (conversion_report.ec != std::errc{}) {
		cout << "conversion to int failed" << endl;
		cerr << "conversion to int failed" << endl;
		throw;
	}
	return value;
}

template<typename t_result>
class BasicSolve{
	vector<string> lines;
	vector<t_result> results;
	t_result total_result;
	string name;
	high_resolution_clock::duration work_time;

	virtual t_result solveLine(const string& line);

	public:
	BasicSolve(string _name) 
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
