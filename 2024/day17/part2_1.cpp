#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>
using namespace std;
typedef int ull;

struct State {
	int A;
	int B;
	int C;
	int ptr;
};
State state;
vector<ull> program;
vector<ull> output;

void PrintState();
void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	getline(f, line);
	state.A = stol(line.c_str() + line.find(':') + 1);
	getline(f, line);
	state.B = stol(line.c_str() + line.find(':') + 1);
	getline(f, line);
	state.C = stol(line.c_str() + line.find(':') + 1);
	getline(f, line, '\0');
	state.ptr = 0;

	for (size_t it = line.find(' '); it != string::npos; it = line.find(',', it + 1)) {
		const char* ptr = line.c_str() + it + 1;
		program.push_back(stol(ptr));
	}
}

ull combOp(ull x) {
	switch (x) {
	case 7: assert(0);
	case 4: return state.A;
	case 5: return state.B;
	case 6: return state.C;
	default: return x;
	}
}
void op0(ull x) {
	x = combOp(x);
	//cout << 0 << " :" << x << endl;
	state.A /= (1 << x);
	state.ptr += 2;
}

void op1(ull x) {
	//x = combOp(x);
	//cout << 1 << " :" << x << endl;
	state.B ^= x;
	state.ptr += 2;
}

void op2(ull x) {
	x = combOp(x);
	//cout << 2 << " :" << x << endl;
	state.B = x%8;
	state.ptr += 2;
}

void op3(ull x) {
	//cout << 3 << " :" << x << endl;
	if (state.A) {
		state.ptr = x;
	}
	else {
		state.ptr += 2;
	}
}

void op4(ull x) {
	//cout << 4 << " :" << x << endl;
	state.B ^= state.C;
	state.ptr += 2;
}

void op5(ull x) {
	x = combOp(x);
	//cout << 5 << " :" << x << endl;
	output.push_back(x%8);
	state.ptr += 2;
}

void op6(ull x) {
	x = combOp(x);
	//cout << 6 << " :" << x << endl;
	state.B = state.A/(1 << x);
	state.ptr += 2;
}

void op7(ull x) {
	x = combOp(x);
	//cout << 7 << " :" << x << endl;
	state.C = state.A/(1 << x);
	state.ptr += 2;
}


template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
	out << "[";
	bool flag = false;
	for (auto& a : v) {
		if (flag) {
			out << ",";
		}
		flag = true;
		cout << a;
	}
	out << "]";
	return out;
}

void (*ops[])(ull) {
	op0, op1, op2, op3, op4, op5, op6, op7
};
void Step() {
	void (*op)(ull) = ops[program[state.ptr]];
	ull arg = program[state.ptr + 1];
	op(arg);
}

void Reset() {
	state.A = 0;
	state.B = 0;
	state.C = 0;
	state.ptr = 0;
	output.clear();
}
void Solve() {
	unsigned long long x = 0;
	while(output != program) {
		cout << "trying " << x << endl;
		Reset();
		state.A = x++;
		while (state.ptr < program.size()) {
			Step();
			//PrintState();
		}
		PrintState();
	}
	cout << "min init: " << x << endl;
}

void PrintState() {
        cout << "Register A: " << state.A << endl;
        cout << "Register B: " << state.B << endl;
        cout << "Register C: " << state.C << endl;
        cout << "prog pointer: " << state.ptr << endl;
	cout << "prog: " << program << endl;
	cout << "out: " << output << endl;
	cout << endl;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return -1;
	}
	ReadFile(argv[1]);
	PrintState();
	Solve();
    //0,4,4,2,6,3,0,5,5 - not right
    //3,1,1,2,4,2,7,5,5 - not right
    //7,1,5,2,4,0,7,6,1
}
