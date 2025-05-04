#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
using namespace std;
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

const vector<Point> dirs = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};
Point guard;
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

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
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

unsigned long long Calculate() {
	unsigned long long result = 0;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].length(); j++) {
			unsigned long long sides = 0;
			unsigned long long area = 0;
			map<int, set<int>> borders[4];
			Point pt = {i,j};
			char seed = Get(pt);
			if (seed == 0) {
				continue;
			}
			set<Point> queue;
			set<Point> mark;
			queue.insert(pt);
			while(!queue.empty()) {
				Point g = *queue.begin();
				queue.erase(g);
				if (mark.count(g)) {
					continue;
				}
				mark.insert(g);
				area++;
				for (int d = 0; d < 4; d++) {
					Point ng = g + dirs[d];
					if (OutOf(ng) || seed != Get(ng)) {
						int l;
						int k;
						if (d % 2 == 0) {
							l = g.x;
							k = g.y;
						}
						else {
							l = g.y;
							k = g.x;
						}
						borders[d][l].insert(k);
					}
					else {
						queue.insert(ng);
					}
				}
			}
			for (int d = 0; d < 4; d++) {
				for (auto& [l, segments] : borders[d]) {
					int old_k = *segments.begin();
					sides ++;
					for (int k : segments) {
						if ( k - old_k > 1) {
							sides++;
						}
						old_k = k;
					}
				}
			}
			result += area*sides;
			cout << seed << ":" << area<< "," << sides << endl;
			for (Point p : mark) {
				Set(p, 0);
			}
		}
	}
	return result;
}

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
    
    cout << Calculate() << endl;
//    PrintBoard();
    // 841078
}
