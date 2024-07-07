//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
// g++ -O2 day1_2-thread2.cpp -o day1_2-thread2r.exe
#include <fstream>
#include <string>
#include <vector>

using namespace std;
class cExpander{
	vector<string> lines; // input values
	string name;
	
	public:
	cExpander(string _name)
		:name(_name){
		ifstream ifstr(name, ios::binary);
		for (string line; getline(ifstr, line);) {
			lines.push_back(line);
		}
		ifstr.close();
	}

	void expand(string _name, int width_expand, int height_expand){
		ofstream ofstr(_name, ios::binary);
		for (int i = 0; i < height_expand; i++ ) {
			for (string& line : lines) {
				string width_line;
				for (int j = 0; j < width_expand; j++ ) {
					width_line += line;
				}
				ofstr << width_line << endl;
			}
		}
		ofstr.close();
	}
};

int main(void) {
	cExpander cond100("cond.input");
	cond100.expand("cond50_1000.input", 50, 1000);
	return 0;
}