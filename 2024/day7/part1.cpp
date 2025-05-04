#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
struct Equation{
	unsigned long result;
	vector<unsigned long> vars;
};
vector<Equation> calibrations;

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        calibrations.emplace_back();
	size_t it = line.find(':');
	line[it++] = 0;
	line[it] = 0;
	calibrations.back().result = stol(line.c_str());
	while(it != string::npos) {
		calibrations.back().vars.push_back(stol(line.c_str() + it + 1));
		it = line.find(' ', it);
		line[it] = 0;
	} 
    }
}

bool SolveEq(Equation& eq, unsigned long val, vector<unsigned long>::iterator it) {
	if (it == eq.vars.end()) {
		return val == eq.result;
	}
	return SolveEq(eq, val*(*it), it+1) || SolveEq(eq, val + (*it), it+1);
}

unsigned long long Solve() {
	unsigned long long result = 0;
	for (auto& eq : calibrations) {
	    	if (eq.vars.empty()) {
			continue;
		}
		if (SolveEq(eq, eq.vars[0], eq.vars.begin() + 1)) {
			result += eq.result;
		}
    	}
	return result;
}
void PrintEquations() {
	for (auto& eq : calibrations) {
		cout << eq.result << ":";
		for (unsigned long x :eq.vars) {
			cout << " " << x;
		}
		cout << endl;
	}
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    PrintEquations();
    cout << Solve() << endl;
    // cond 932137732557
}
