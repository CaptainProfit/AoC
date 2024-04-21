//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

#define ull unsigned long long

using namespace std;

const string digs = "0123456789"s;
class cSolve{
	vector<string> lines;
	vector<int> numbers;
	int _result;
	string name;
	public:
	cSolve(string _name):name(_name) {
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
		for (const string& a : lines) {
			//a.find_first_of()
			int a1 = a.find_first_of(digs);
			int a0 = a.find_last_of(digs);
			cout << numbers.size() << ": " << a[a1] << a[a0] << endl;
			numbers.push_back((a[a1] - '0')*10 + a[a0]-'0');
		}
		_result = accumulate(numbers.begin(), numbers.end(), 0);
		cout << "result: " << _result << endl;
	}

	int result() {
		return _result;
	}
};

int main(void) {
	cSolve example("example.input");
	cSolve cond("cond.input"); // 55029 correct part1
	example.solve();
	cond.solve();
	cout << "there are " << example.result() << " in result" << endl;
	cout << "there are " << cond.result() << " in result" << endl;
	return 0;
}
