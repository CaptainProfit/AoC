#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
using namespace std;
typedef long long ull;
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
    Point& operator%=(const Point& lhs) {
	    x+= lhs.x;
        x %= lhs.x;
	y += lhs.y;
        y %= lhs.y;
        return *this;
    }
};

struct Robot {
	Point p;
	Point v;
};
vector<Robot> robots;
Point board;

Point operator+(Point rhs, const Point& lhs) {
    return rhs += lhs;
}

Point operator-(Point rhs, const Point& lhs) {
    return rhs -= lhs;
}

bool operator<(const Point& lhs, const Point& rhs) {
	return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}

bool operator>(const Point& lhs, const Point& rhs) {
	return rhs < lhs;
}

ostream& operator<<(ostream& out, const Point& pt) {
	cout << "(" << pt.y << ", " << pt.x << ")";
	return out;
}

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    getline(f, line);
	board.x = stol(line.c_str());
	board.y = stol(line.c_str() + line.find(' '));
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
	robots.emplace_back();
	
	robots.back().p.x = stol(line.c_str() + line.find('=') + 1);
	robots.back().p.y = stol(line.c_str() + line.find(',') + 1);
	size_t p = line.find('v');
	robots.back().v.x = stol(line.c_str() + line.find('=', p) + 1);
	robots.back().v.y = stol(line.c_str() + line.find(',', p) + 1);
    }
}

ull Gcd(ull a, ull b) {
	ull r = a, q = b;
	ull gcd;
	while (r != 0) {
		q = a/b;
		r = a%b;
		a = b;
		b = q;
	}
	return q;
}

ull Lcm(ull a, ull b) {
	return (a/Gcd(a, b))*b;
}

void Tick() {
	for ( auto& robot : robots) {
		robot.p += robot.v;
		robot.p %= board;
	}
}
void Print();
ull Calculate();
void Solve() {
	/*cout << board << endl;
	for (Robot& r : robots) {
		cout << "p:" << r.p << " v:" << r.v << endl;
	}
	Print();*/

	for (int i = 0; i < 10000; i++) {
		Tick();
		if ( ((i + (board.x - 98)) % board.x == 0) &&
		     ((i + (board.y - 57)) % board.y == 0)) {
			cout << "----" << i << "----" << endl;
			Print();
		}
		//}
		char c;
		if (Calculate() == 0) {
			cin >> c;
		}
	}
}

void Print() {
	vector<string> field(board.y, string(board.x, '.'));
	for (Robot& r : robots) {
		field[r.p.y][r.p.x]++;
	}
	for (string& line : field) {
		cout << line << endl;
	}
}

ull Calculate() {
	ull q1 = 0, q2 = 0, q3 = 0, q4 = 0;
	for (Robot& r : robots) {
		if (r.p.x < board.x/2 && r.p.y < board.y/2 ) {
			q1++;
		}
		if (r.p.x > board.x/2 && r.p.y < board.y/2 ) {
			q2++;
		}
		if (r.p.x < board.x/2 && r.p.y > board.y/2 ) {
			q3++;
		}
		if (r.p.x > board.x/2 && r.p.y > board.y/2 ) {
			q4++;
		}
	}
	return q1 * q2 * q3 * q4;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    Solve(); 
    cout << Calculate() << endl;
    //1000 too low
    // 58000 too hight
    // 7370 - i saw it!
    // 7371 correct
}
