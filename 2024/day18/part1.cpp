#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
using namespace std;

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
/*    Point& operator=(const Point& rhs) {
	x = rhs.x;
	y = rhs.y;
	return *this;
    }*/
};

const vector<Point> dirs = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};

bool operator==(const Point& lhs, const Point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
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

ostream& operator<<(ostream& out, const Point& pt) {
	cout << "(" << pt.y << ", " << pt.x << ")";
	return out;
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

int size_;
Point start;
Point finish;
set<Point> obstacles;
set<Point> path;
void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	long len = 0;
	getline(f, line);
	size_ = stol(line) + 1;
	line = line.substr(line.find(' ') + 1); 
	len = stol(line);
	for(getline(f, line); !f.eof() && !line.empty() && len-->0 ; getline(f, line)) {
		int x = stol(line);
		line = line.substr(line.find(',') + 1); 
		int y = stol(line);
        	obstacles.insert({y, x});
	}
	start = {0, 0};
	finish = {size_ - 1, size_ - 1};
}

bool OutOf(Point pt) {
    return pt.x < 0 || pt.y < 0 || pt.x >= size_ || pt.y >= size_;
}


void PrintBoard() {
	for (int i = 0; i < size_; i++) {
		for (int j = 0; j < size_; j++) {
			if (obstacles.count({i, j}) > 0) {
				cout << "#";
			}
			else if (path.count({i,j}) > 0 ) {
				cout << "O";
			}
			else {
				cout << ".";
			}
		}
		cout << endl;
	}
}

template<typename T>
struct GraphMover {
	T start;
	T target;
	GraphMover(T start_, T target_):
		start(start_),
		target(target_){
		
	}
	map<T, size_t> costs;
	set<T> marks;
	map<T, T> froms;
	multimap<size_t, T> que;
	
	bool Comparer(const T& lhs, const T& rhs) {
		return false;
	}

	void Add(const T& prev, const T& node, size_t cost) {
		if (obstacles.count(node) > 0 || OutOf(node)) {
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
		for (const Point& dir : dirs) {
			Add(p, p+dir, cost + 1);
		}
	}
	
	void Walk() {
		que.insert({0, start});
		costs.insert({start, 0});
		while(!que.empty()) {
			T p = que.begin()->second;
			que.erase(que.begin());
			if (marks.count(p)) {
				continue;
			}
			marks.insert(p);
			Action(p);
		}
	}
	
	size_t Target() {
		if (costs.count(target)) {
			return costs[target];
		}
		return 0;
	}
	void PrintPath() {
		auto p = target;
		
		for (;froms.count(p); p = froms[p]) {
			path.insert(p); 
		}
	}
};

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	PrintBoard();
	GraphMover<Point> gr(start, finish);
	gr.Walk();
	gr.PrintPath();
	cout << "shortest path: " << gr.Target() << endl;
	PrintBoard();
	// 284 
}
