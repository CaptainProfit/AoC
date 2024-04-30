//[UTF-8 en/ru]
//problem - https://adventofcode.com/2023/day/2
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

const map<string_view, int> rgbStrings = {{"red", 0}, {"green", 1}, {"blue",2}};
const vector<string> sampleDelims = {"; "s, "\n"s, "\r"s, "\n\r"s, "\r\n"s};
const vector<string> cubeDelims = {"; "s, ", "s, "\n"s, "\r"s, "\n\r"s, "\r\n"s};

class cSolve{
	vector<string> lines;
	vector<int> numbers;
	int _result;
	string name;
	high_resolution_clock::duration work_time;

	pair<int, int> analyzeCube(const string_view& cube) {
		int delim = cube.find(" "s);
		string_view key_sw = cube.substr(delim + 1, cube.length());
		string_view value_sw = cube.substr(0, delim);
		int value; std::from_chars(value_sw.data(), value_sw.data() + value_sw.length(), value);
		return {rgbStrings.at(key_sw), value};
	}
	
	pair<int, size_t> findDelim(const string_view& line, int offset, const vector<string>& delims) {
		int result = line.length();
		size_t delimLength = 0;
		for (const string& delim : delims) {
			int newResult = line.find(delim, offset);
			if (newResult != -1 && newResult < result) {
				result = newResult;
				delimLength = delim.length();
			}
		}
		return {result, delimLength};
	}

	pair<int, string_view> getCube (const string_view& sample, int offset = 0) {
		auto [end, delimLength] = findDelim(sample, offset, cubeDelims);
		string_view cube =  sample.substr(offset, end - offset);
		offset += end - offset + delimLength;
		return {offset, cube};
	}

	pair<int, string_view> getSample (const string_view& line, int offset = 0) {
		auto [end, delimLength] = findDelim(line, offset, sampleDelims);
		string_view sample =  line.substr(offset, end - offset + delimLength);
		offset += end - offset + delimLength;
		return {offset, sample};
	}

	void updateMax(int rgb[], int max_rgb[]) {
		for (int i = 0; i < 3; i++) {
			max_rgb[i] = max(max_rgb[i], rgb[i]);
		}
	}

	int solveLine(const string& line) {
		int max_rgb[3] = {0};
		string_view sw1(line);
		int len = sw1.length();
		int a1 = sw1.find("Game "s);
		int a2 = sw1.find(": "s);
		string_view gameIndex(sw1.substr(a1 + "Game "s.length(), a2 - "Game "s.length()));
		string_view game(sw1.substr(a2 + ": "s.length(), sw1.length()));
		int number; std::from_chars(gameIndex.data(), gameIndex.data() + gameIndex.length(), number);
		for(auto [sample_iter, sample] = getSample(game); 
						sample.length() > 0; 
						tie(sample_iter, sample) = getSample(game, sample_iter)) {
			int rgb[3] = {0};
			for(auto [cube_iter, cube] = getCube(sample); cube.length() > 0; tie(cube_iter, cube) = getCube(sample, cube_iter)) {
				auto [rgb_index, value] = analyzeCube(cube);
				rgb[rgb_index] = value;
			}
			updateMax(rgb, max_rgb);
		}
		return max_rgb[0]*max_rgb[1]*max_rgb[2];
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
			numbers.clear();
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
	cSolve cond("cond.input"); 
	// 67953 correct
	
	//time: 
	example.solve();
	cond.solve();
	
	cout << endl;
	// cout << "there are " << example.result() << " in result" << endl;
	// cout << "there are " << cond.result() << " in result" << endl;
	return 0;
}
