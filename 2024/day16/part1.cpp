#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <cmath>
using namespace std;
using namespace chrono;
using namespace literals;
using dur = duration<float, micro>;
template<>
struct std::hash<dur> {
	hash<float> proxy;
	size_t operator()(const dur& s) const noexcept {
		return proxy(s.count());
	}
};

static dur accum, il, ih;
static float accum2;
size_t cnt = 0;
map <dur, size_t> meases;

class TimeCounter{
	time_point<steady_clock> start;
	public:
	TimeCounter() :
		start(steady_clock::now()) {}
	~TimeCounter() {
		dur meas = duration_cast<dur>(steady_clock::now() - start);
		cnt++;
		accum += meas;
		accum2 += meas.count()*meas.count();
		meases[meas]++;
		if (il > meas || il == 0ms) {
			il = meas;
		}
		if (ih < meas) {
			ih = meas;
		}
	}
	static void PrintStatistics(ostream& out) {
		if (cnt == 0) {
			out << "no measurements";
			return;
		}
		float d = accum2/cnt - accum.count()/cnt*accum.count()/cnt;
		out << "dd: " << d << endl;
		d = sqrt(d);
		out << "total: " << accum.count()<< "us, average: " << accum.count()/cnt << "us (disp:" << d << "us , min: " << il.count() << "us, max:" << ih.count() << "us)";
		return;
	}
	static void PrintHist(ostream& out, int bars) {
		if (bars == 0) {
			return;
		}
		dur t0 = 0us;
		dur dif = (ih - il)/bars;
		for (int i = 1; i < bars; i++) {
			dur t1 = t0 + dif;
			size_t a = 0;
			for (auto it = meases.lower_bound(t0); it != meases.upper_bound(t1); it++) {
				a += it->second;
			}
			out << i << ": " << a << ": " << t0.count() << "us to " << t1.count() << "us" << endl;
			t0 = t1;
		}
	}
};

vector<string> board;
struct Point {
    int y;
    int x;
    Point& operator+=(const Point& lhs) {
        x += lhs.x;
        y += lhs.y;
        return *this;
    }
    Point& operator-=(const Point& lhs) {
        x -= lhs.x;
        y -= lhs.y;
        return *this;
    }
};

const map<char, Point> cdirs = {
	{'N', {-1, 0}},
	{'E', {0, 1}},
	{'S', {1, 0}},
	{'W', {0, -1}}
};
const map <char, char> cvdirs = {
	{'N', '^'},
	{'E', '>'},
	{'S', 'v'},
	{'W', '<'},
	
};
const string sdirs = "NESW";
const vector<Point> dirs = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};
struct Position {
	Point pos;
	char dir;
};
Position deer;
Position finish;
bool operator==(const Point& lhs, const Point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
bool operator==(const Position& lhs, const Position& rhs) {
	return lhs.pos == rhs.pos && lhs.dir == rhs.dir;
}
Point operator+(Point rhs, const Point& lhs) {
    return rhs += lhs;
}

Point operator-(Point rhs, const Point& lhs) {
    return rhs -= lhs;
}

bool operator<(const Point& lhs, const Point& rhs) {
	return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}
bool operator<(const Position& lhs, const Position rhs) {
	return lhs.pos < rhs.pos || (lhs.pos == rhs.pos && lhs.dir < rhs.dir);
}

ostream& operator<<(ostream& out, const Point& pt) {
	cout << "(" << pt.y << ", " << pt.x << ")";
	return out;
}
ostream& operator<<(ostream& out, const Position& pt) {
	cout << "(" << pt.pos << ", " << pt.dir << ")";
	return out;
}

void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
		size_t j = line.find('S');
		size_t i = board.size();
		if (j != string::npos) {
			deer.pos = {(int)i, (int)j};
			deer.dir = 'E';
		}
		j = line.find('E');
		if (j != string::npos) {
			finish.pos = {(int)i, (int)j};
			finish.dir = 'E';
		}
        	board.emplace_back(move(line));
	}
}

bool OutOf(Point pt) {
    return pt.x < 0 || pt.y < 0 || pt.x >= board[0].length() || pt.y >= board.size();
}

char Get(Point pt) {
    return board[pt.y][pt.x];
}

void Set(Point pt, char c) {
    board[pt.y][pt.x] = c;
}

Position RCW(Position old) {
	//cout << "CW:(" << old.dir << "->";
	old.dir = sdirs[(sdirs.find(old.dir) + 1)%sdirs.length()];
	//cout << old.dir << ")";
	return old;
}

Position RCCW(Position old) {
	//cout << "CCW:(" << old.dir << "->";
	old.dir = sdirs[(sdirs.find(old.dir) + 3)%sdirs.length()];
	//cout << old.dir << ")";
	return old;
}

Position Forward(Position old) {
	//cout << "F:(" << old.pos;
	old.pos += cdirs.at(old.dir);
	//cout << old.pos << "):(" << old.dir << "->" << cdirs.at(old.dir) << ")";
	return old;
}

template<>
struct std::hash<Point> {
	hash<int> proxy;
	size_t operator()(const Point& p) const noexcept {
		size_t h1 = proxy(p.y);
		size_t h2 = proxy(p.x);
		return h1 + h2 * 137;
	}
};
template<>
struct std::hash<Position> {
	hash<Point> proxyp;
	hash<int> proxyd;
	size_t operator()(const Position& p) const noexcept {
		size_t h1 = proxyp(p.pos);
		size_t h2 = proxyd(p.dir);
		return h2 + h2*137*137;
	}
};
void PrintBoard();
template<typename T>
struct GraphMover {
	T start;
	T target;
	GraphMover(T start_, T target_):
		start(start_),
		target(target_){
		
	}
	map<T, size_t> costs;
	map<T, T> froms;
	set<T> marks;
	multimap<size_t, T> que;
	size_t counter = 0;
	
	bool Comparer(const T& lhs, const T& rhs) {
		return false;
	}

	void Add(const T& prev, const T& node, size_t cost) {
		if (Get(node.pos) == '#') {
			return;
		}
		if (marks.count(node)) {
			return;
		}
		if (costs.count(node) == 0 || costs[node] > cost) {
			froms[node] = prev;
			costs[node] = cost;
		}		
		que.insert({cost, node});
	}

	void Action(const T& p) {
		size_t cost = costs.at(p);
		Add(p, RCW(p), cost + 1000);
		Add(p, RCCW(p), cost + 1000);
		Add(p, Forward(p), cost + 1);
	}
	
	void Walk() {
		que.insert({0, start});
		costs.insert({start, 0});
		while(!que.empty()) {
			TimeCounter tcr;
			counter++;
			T p = que.begin()->second;
			que.erase(que.begin());
			//cout << p;
			if (marks.count(p)) {
				continue;
			}
			marks.insert(p);
			//cout << "acts";
			Action(p);
			/*if (p.pos == target.pos) {
				//cout << endl;
				cout << " target reached" << endl;
				return;
			}*/
			//cout << endl;
		}
	}
	
	size_t Target() {
		size_t result = string::npos;
		for (char c : sdirs) {
			target.dir = c;
			if (costs.count(target)) {
				result = min(result, costs[target]);
			}
		}
		return result;
	}
	void PrintPath() {
		Set(target.pos, 'X');
		Set(start.pos, 'O');
		auto p = target;
		
		for (;froms.count(p); p = froms[p]) {
			if (p.pos == froms[p].pos) {
				continue;
			}
			Set(froms[p].pos, cvdirs.at(p.dir)); 
		}
	}
};

void PrintBoard() {
    for (auto& line : board) {
        cout << line << endl;
    }
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	PrintBoard();
	size_t area = 0;
	for (auto& l : board) {
		for(auto& c : l) {
			area += c != '#';
		}
	}
	cout << "map size: " << area << endl;
	cout << "state size: " << area*4 << endl;
	GraphMover<Position> gr(deer, finish);
	gr.Walk();
	gr.PrintPath();
	TimeCounter::PrintStatistics(cout);
	TimeCounter::PrintHist(cout, 10);
	cout << endl;
	cout << "nodes touched: " << gr.counter << endl;
	cout << "shortest path: " << gr.Target() << endl;
	PrintBoard();
	// 99448 but why so long... because i dont know hashes
}
