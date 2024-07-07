//[UTF-8 en/ru]
//problem - https://adventofcode.com/2023/day/3
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <map>
#include <set>

#include "../basic_solve.hpp"

const set<char> not_interesting_symbols {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '\n', '\r'};
string digits_str = "01234567890"s;

class cSolve: public BasicSolve<int, cSolve::solveLine>{
	public:
	int width, height;
	int i = 0;

	int solveLine(const string& line) {
		//string& line = lines[i];
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
		i++;
		return result;
	}

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

	public:
	cSolve(const string& _name):BasicSolve(_name){
		i = 0;
	}
};



int main(void) {
	cSolve example("example.input");
	// 4361
	cSolve cond("cond.input");
	// 518930 - too low
	// 521515 - correct
	example.solve();
	cond.solve();
	cout << endl;
	return 0;
}
