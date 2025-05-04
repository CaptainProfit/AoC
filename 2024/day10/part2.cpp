#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
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

unsigned long long CalculateScore(Point start) {
	unsigned long long result = 0;
	map<int, map<Point, unsigned long long>> marks;
	marks[0][start] = 1;
	for(int h = 0; h < 9; h++)  {
		auto pts = marks[h];
		for (auto& [pt, score] : pts) {
			for (int d = 0; d < 4; d++) {
				Point new_pt = pt + dirs[d];
				//cout << new_pt << ":";
				if (OutOf(new_pt)) {
					//cout << ",";
					continue;
				}
				//cout << Get(new_pt);
				if ((Get(new_pt) - Get(pt)) == 1) {
					//cout << "!";
					marks[h + 1][new_pt] += score;
				}
				//cout << ",";
			}
		}
		//cout << ")" <<  endl;
	}
	for (auto& mark : marks[9]) {
		result += mark.second;
	}
	return result;
}

unsigned long long Calculate() {
	unsigned long long result = 0;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].length(); j++) {
			Point pt = {i,j};
			//cout << "c" << pt << endl;
			if (Get(pt) == '0') {
				result+= CalculateScore(pt);
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
    // 1735
}
