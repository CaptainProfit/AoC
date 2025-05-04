#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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
map<char, vector<Point>>antenna_ranges;
//unordered_set<Point> focuses;

Point operator+(Point rhs, const Point& lhs) {
    return rhs += lhs;
}

Point operator-(Point rhs, const Point& lhs) {
    return rhs -= lhs;
}

bool operator==(const Point& rhs, const Point& lhs) {
	return rhs.x == lhs.x && rhs.y == lhs.y;
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

char GetPos(Point pt) {
    return board[pt.y][pt.x];
}

void SetPos(Point pt, char c) {
    board[pt.y][pt.x] = c;
}

void Arrange() {
	int i = 0;
	for (auto& line : board) {
		int j = 0;
		for (char c : line) {
			if (isalnum(c)) {
				antenna_ranges[c].push_back({i,j});
			}
			j++;
		}
		i++;
	}
}

long t = 0;
void CalculateFocuses() {
	for (auto& antennas : antenna_ranges) {
		cout << antennas.first;
		for (Point a1 : antennas.second) {
			cout << " (" << a1.y << "," << a1.x << ")";
		}
		cout << endl;
		for (Point a1 : antennas.second) {
			for (Point a2 : antennas.second) {
				if (a1 == a2) {
					continue;
				}
				for (Point pt = a1; !OutOf(pt); pt += (a1 - a2)) {
					cout << pt.y << "," << pt.x << endl;
					if (GetPos(pt) == '#') {
						continue;
					}
					SetPos(pt, '#');
					t++;
				}
				//focuses.insert(pt);
			}
		}
	}
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
    Arrange();
    CalculateFocuses();
    cout << t << endl;
    PrintBoard();
    // 1115
}
