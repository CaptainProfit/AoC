#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
using namespace std;
vector<string> board;
string moves;
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
const map<char, Point> mmoves = {
	{'<', {0, -1}},
	{'V', {1, 0}},
	{'v', {1, 0}},
	{'^', {-1, 0}},
	{'>', {0, 1}}};
Point robot;
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
	//cout << "(" << pt.y << ", " << pt.x << ")";
	return out;
}

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
	    if (line.find("@") != string::npos) {
		    robot.x = line.find('@');
		    robot.y = board.size();
	    }
        board.emplace_back(move(line));
    }
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
	    moves += line;
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

bool Move(Point pt, Point dir, char c) {
	//cout << "m" << pt << endl;
	Point new_pt = pt + dir;
	if (OutOf(new_pt)) {
		return false;
	}
	switch (Get(new_pt)) {
		case '#':
			return false;
		case 'O':
			if (!Move(new_pt, dir, Get(new_pt))) {
				return false;
			}
		case '.':
			//cout << "s" << pt << new_pt << c << endl;
			Set(pt, '.');
			Set(new_pt, c);
			//cout << "se" << endl;
	}
	return true;
}

void Tick(char move) {
	//cout << "c" << move << endl;
	Point dir = mmoves.at(move);
	//cout << dir << endl;
	if (Move (robot, dir, '@')) {
		robot += dir;
	}
}

void Simulate() {
	for (char c : moves) {
		Tick(c);
	}
}

unsigned long long Calculate() {
	unsigned long long result = 0;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].length(); j++) {
			if (Get({i, j}) != 'O') {
				continue;
			}
			result += 100*i + j;
		}
	}
	return result;
}

void PrintBoard() {
    for (auto& line : board) {
        cout << line << endl;
    }
    cout << moves << endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    PrintBoard();
    Simulate();
    PrintBoard();
    cout << Calculate() << endl;
    //1485257
}
