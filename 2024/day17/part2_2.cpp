#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <csignal>

using namespace std;
typedef int ull;

struct State {
	int A;
	int B;
	int C;
	int ptr;
};

struct RevState {
	State state;
	int cycle;
};

enum Comb {A, B, C};
struct Generator {
	RevState rstate;
	int gen;
	int offset;
	int step;
	int limit;
	Comb ent;
	bool operator()() {
		if (limit != 0 && gen >= limit) {
			return false;
		}
		switch(ent) {
			case Comb::A:
				rstate.state.A = offset + step*gen;
				//cout << "A: " << rstate.state.A;
				break;
			case Comb::B:
				rstate.state.B = offset + step*gen;
				break;
				//cout << "B: " << rstate.state.B;
			case Comb::C:
				rstate.state.C = offset + step*gen;
				break;
				//cout << "C: " << rstate.state.C;
			default:
				assert(0);
				break;
		}
		gen++;
		return true;
	}
};

bool operator<(const State& lhs, const State& rhs) {
	return lhs.ptr < rhs.ptr || ( lhs.ptr == rhs.ptr &&
		lhs.A < rhs.A    || ( lhs.A == rhs.A && 
		lhs.B < rhs.B    || ( lhs.B == rhs.B &&
		lhs.C < rhs.C )));
}

bool operator==(const State& lhs, const State& rhs) {
	return !(lhs<rhs) && !(rhs<lhs);
}

bool operator<(const RevState& lhs, const RevState& rhs) {
	return  lhs.cycle < rhs.cycle || ( lhs.cycle == rhs.cycle &&
		lhs.state < rhs.state);
}

bool operator==(const RevState& lhs, const RevState& rhs) {
	return !(lhs<rhs) && !(rhs<lhs);
}

bool operator<(const Generator& lhs, const Generator& rhs) {
	return  lhs.rstate < rhs.rstate || (lhs.rstate == rhs.rstate &&  
			lhs.gen < rhs.gen);
}

set<Generator> generators;
void AddGenerator(RevState& rstate, int offset, int step, int limit, Comb entity) {
	Generator g;
	g.rstate = rstate;
	g.offset = offset;
	g.step = step;
	g.limit = limit;
	g.ent = entity;
	g.gen = 0;
	generators.insert(g);
}

vector<ull> program;
ull possibleA;
set<RevState> rstates;

void PrintRevState(const RevState& rstate);
void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	getline(f, line);
//	state.A = stol(line.c_str() + line.find(':') + 1);
	getline(f, line);
//	state.B = stol(line.c_str() + line.find(':') + 1);
	getline(f, line);
//	state.C = stol(line.c_str() + line.find(':') + 1);
	getline(f, line, '\0');
//	state.ptr = 0;

	for (size_t it = line.find(' '); it != string::npos; it = line.find(',', it + 1)) {
		const char* ptr = line.c_str() + it + 1;
		program.push_back(stol(ptr));
	}
}

//comb:
//0..3 - 0..3
//4..6 - A..C
//7 - Reserved
ull combOp(ull x, State& state) {
	switch (x) {
	case 7: assert(0);
	case 4: return state.A;
	case 5: return state.B;
	case 6: return state.C;
	default: return x;
	}
}

void StepBack(RevState& rstate);
bool RevPtr(RevState& rstate) {
	assert(rstate.state.ptr >= 0);
	rstate.state.ptr -= 2;
	if (rstate.state.ptr <= 0) {
		rstate.cycle--;
		if (rstate.cycle == 0) {
			//if (rstate.state.B == 0 && rstate.state.C == 0) {
				if (possibleA == 0 || possibleA > rstate.state.A) {
					possibleA = rstate.state.A;
				}
			//}
			PrintRevState(rstate);
			return false;
		}
		else {
			rstate.state.ptr = program.size();
		}
	}
	return true;
}

void AddRevState(RevState& rstate) {
	if (possibleA == 0 || rstate.state.A < possibleA) {
		rstates.insert(rstate);
	}
}

// A <<= comb, A += r
void rop0(ull x, RevState& rstate) {
	assert (x != 4);
	ull y = combOp(x, rstate.state);
	rstate.state.A <<= y;
	cout << "op0::" << rstate.state.A << ", y:" << y<< endl;
	if (RevPtr(rstate)) {
		AddGenerator(rstate, rstate.state.A, 1, 1 << y, Comb::A);
	}
}

// B ^= comb
void rop1(ull x, RevState& rstate) {
	ull y = combOp(x, rstate.state);
	assert(y != 5);
	rstate.state.B ^= y;
	if (RevPtr(rstate)) {
		AddRevState(rstate);
	}
}

// B == comb %8
// B = int;
void rop2(ull x, RevState& rstate) {
	ull y = combOp(x, rstate.state);
	if ( rstate.state.B != y%8) {
		return;
	}
	// generate B
	if (RevPtr(rstate)) {
		AddGenerator(rstate, 0, 1, 0, Comb::B);
	}
}

// ungoto A
void rop3(ull x, RevState& rstate) {
	if (RevPtr(rstate)) {
		AddRevState(rstate);
	}
}

// B ^= C
void rop4(ull x, RevState& rstate) {
	rstate.state.B ^= rstate.state.C;
	if (RevPtr(rstate)) {
		AddRevState(rstate);
	}
}

// check comb%8 == program[cycle - 1];
void rop5(ull x, RevState& rstate) {
	assert (x>3);
	bool flag = false;
	ull y = combOp(x, rstate.state);
	assert(x != 7);
	cout << "rop5: "<< y << " vs " << program[rstate.cycle - 1] << endl;
	if (y%8 == program[rstate.cycle - 1]) {
		if (RevPtr(rstate)) {
			AddRevState(rstate);
		}
	}
}

// B == A >> comb;
// B = int
void rop6(ull x, RevState& rstate) {
	assert (x != 5);
	ull y = combOp(x, rstate.state);
	if ( rstate.state.B != A >> y ) {
		return;
	}
	if (RevPtr(rstate)) {
		AddGenerator(rstate, 0, 1, 0, Comb::B);
	}
}

// C == A >> (comb);
// C = int
void rop7(ull x, RevState& rstate) {
	assert ( x != 6);
	ull y = combOp(x, rstate.state);
	if (rstate.state.C != rstate.state.A >> (1 << y)) {
		return;
	}
	if (RevPtr(rstate)) {
		AddGenerator(rstate, 0, 1, 0, Comb::C);
	}
}

ostream& operator<<(ostream& out, const State& v) {
	out << "A:" << v.A << ", B:" << v.B << ", C:" << v.C << ", ptr: " << v.ptr;
	return out;
}

ostream& operator<<(ostream& out, const RevState& v) {
	out << v.state << ", cycle: " << v.cycle;
	return out;
}

ostream& operator<<(ostream& out, const Comb& v) {
	switch (v) {
		case Comb::A: out << "A"; break;
		case Comb::B: out << "B"; break;
		case Comb::C: out << "C"; break;
		default : out << "Nan";
	}
	return out;
}

ostream& operator<<(ostream& out, const Generator& v) {
	out << v.rstate << endl;
	out << v.ent << " = " << v.offset << " + " << v.step << "*" << v.gen;
	if (v.limit != 0) {
		out << "(" << v.limit <<  ")";
	}
	out << endl;
	return out;
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
		out << a;
	}
	out << "]";
	return out;
}

void (*rops[])(ull, RevState&) {
	rop0, rop1, rop2, rop3, rop4, rop5, rop6, rop7
};

void StepBack(RevState& rstate) {
	void (*rop)(ull, RevState&) = rops[program[rstate.state.ptr - 2]];
	ull arg = program[rstate.state.ptr -1];
	rop(arg, rstate);
	PrintRevState(rstate);
}

void Reset(State& state) {
	state.A = 0;
	state.B = 0;
	state.C = 0;
	state.ptr = program.size();
}

void Solve() {
	unsigned long long x = 0;
	RevState start;
	Reset(start.state);
	start.cycle = program.size();
	rstates.insert(start);

	while (!rstates.empty()) {
		while (!rstates.empty()) {
			auto a = rstates.begin();
			RevState p = *a;
			rstates.erase(a);
			StepBack(p);
		}
		set<Generator> new_gens;
		for(auto g : generators) {
			if (g()) {
				rstates.insert(g.rstate);
				new_gens.insert(g);
			}
		}
		generators = move(new_gens);
	}
	cout << "min init: " << possibleA << endl;
}

void PrintRevState(const RevState& rstate) {
        cout << "Register A: " << rstate.state.A << endl;
        cout << "Register B: " << rstate.state.B << endl;
        cout << "Register C: " << rstate.state.C << endl;
        cout << "prog pointer: " << rstate.state.ptr << endl;
	cout << "cycle: " << rstate.cycle << endl;
	cout << "prog: " << program << endl;
	// cout << "out: " << output << endl;
	cout << endl;
}

void MySignalHandler(int sig) {
	cout << " process halted by signal(" << sig << "), possibleA: " << possibleA << endl;
	exit(-1);
}

int main(int argc, char** argv) {
	signal(SIGINT, MySignalHandler);
	signal(SIGTERM, MySignalHandler);
	if (argc < 2) {
		return -1;
	}
	ReadFile(argv[1]);
	Solve();
}
