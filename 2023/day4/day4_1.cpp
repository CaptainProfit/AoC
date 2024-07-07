//[UTF-8 en/ru]
//problem - https://adventofcode.com/2023/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <map>
#include "../basic_solve.hpp"

class cSolve : public BasicSolve<int>{
	int solveLine(const string& line) {
		return 1;
	}
};

int main(void) {
	cSolve example("example.input"s);
	cSolve cond("cond.input"s); 
	//example.solve();
	//cond.solve();
	cout << endl;
	return 0;
}
