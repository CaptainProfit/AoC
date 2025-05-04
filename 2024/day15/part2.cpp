#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
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
	cout << "(" << pt.y << ", " << pt.x << ")";
	return out;
}

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
	    board.emplace_back();
	    for (char c : line) {
		    switch(c) {
			    case '#':
				    board.back() += "##";
				    break;
			    case '@':
				    board.back() += "@.";
				    	robot.y = board.size() - 1;
					robot.x = board.back().find('@');
				    break;
				case '.':
				board.back() += "..";
				break;
				case 'O' :
					board.back() += "[]";
					break;

		    }
	    }
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

bool Move(Point pt, Point dir, char c, bool test = false) {
	Point new_pt = pt + dir;
	if (OutOf(new_pt)) {
		return false;
	}
	switch (Get(new_pt)) {
		case '#':
			return false;
		case '[':
		case ']':
			{
				Point new_pt2 = new_pt;
				if (Get(new_pt) == '[') {
					new_pt2.x++;
				}
				else {
					new_pt2.x--;
				}
				if (dir.y != 0)
				{
					if (!Move(new_pt, dir, Get(new_pt), true) || !Move(new_pt2, dir, Get(new_pt2), true)) {
						return false;
					}
					Move(new_pt, dir, Get(new_pt), test);
					Move(new_pt2, dir, Get(new_pt2), test);
				}
				else if (!Move(new_pt, dir, Get(new_pt), test)) {
					return false;
				}
			}
		case '.':
			if (!test) {
				Set(pt, '.');
				Set(new_pt, c);
			}
	}
	return true;
}

void Tick(char move) {
	Point dir = mmoves.at(move);
	if (Move (robot, dir, '@')) {
		//Move (robot, dir, '@');
		robot += dir;
	}
}

void PrintBoard() {
    for (auto& line : board) {
        cout << line << endl;
    }
}

void Simulate() {
	int i = 1;
	cout << "---- start ----" << endl;
	PrintBoard();
	int x = 0;
	for (char c : moves) {
		Tick(c);
		/*PrintBoard();
		cout << "---- " << c << " " << i++ << " ----" << endl;
		std::this_thread::sleep_for(10ms);
		if (x-- <= 0) {
			try {
				cin >> x;
			}
			catch(...) {
				x = 10;
			}
		}//*/
	}
}

unsigned long long Calculate() {
	unsigned long long result = 0;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].length(); j++) {
			if (Get({i, j}) != '[') {
				continue;
			}
			result += 100*i + j;
		}
	}
	return result;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    //PrintBoard();
    Simulate();
    //PrintBoard();
    cout << Calculate() << endl;
    // 1485257
    // 1480327 too high
    // 1400327 too low
    // 1480000 nope
    // 1475512
} 
