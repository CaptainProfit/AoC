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
};

struct Game {
	Point prize;
	Point a;
	Point b;
};
vector<Game> games;
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

void PrintGames();
void ReadFile(string filename) {
    ifstream f(filename);
    string line1, line2, line3;
    for(getline(f, line1); !f.eof() && !line1.empty(); getline(f, line1)) {
	getline(f, line2);
	getline(f, line3);
	games.emplace_back();
	
	games.back().a.x = stol(line1.c_str() + line1.find('X') + 1);
	games.back().a.y = stol(line1.c_str() + line1.find('Y') + 1);
	games.back().b.x = stol(line2.c_str() + line2.find('X') + 1);
	games.back().b.y = stol(line2.c_str() + line2.find('Y') + 1);
	games.back().prize.x = stol(line3.c_str() + line3.find('X') + 2);
	games.back().prize.y = stol(line3.c_str() + line3.find('Y') + 2);
	getline(f, line1);
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

ull Solve1(const Game& game) {
	int x1 = 0;
	int x2 = 0;
	ull a11 = game.a.x;
	ull a12 = game.b.x;
	ull a21 = game.a.y;
	ull a22 = game.b.y;
	ull b1 = game.prize.x;
	ull b2 = game.prize.y;
	ull det = a11*a22 - a12*a21;
	cout << a11 << " " << a12 << "|" << b1 << endl;
	cout << a12 << " " << a22 << "|" << b2 << endl;
	cout << det << endl;
	x1 = (a22*b1 - a12*b2)/det;
	x2 = (-a21*b1 + a11*b2)/det;
	if (x1 < 0 || x2 < 0 || a11*x1 + a12*x2 != b1 || a21*x1 + a22*x2 != b2) {
		return 0;
	}
	cout << "(" << x1 << ", " << x2 << ")" << endl;
	return 3*x1 + x2;
}

ull Solve(const Game& game) {
	int x1 = 0;
	int x2 = 0;

	return 3*x1 + x2;
}
ull Calculate() {
	ull result = 0;
	for (int i = 0; i < games.size(); i++) {
		result += Solve1(games[i]);
	}
	return result;
}

void PrintGames() {
    for (Game& game : games) {
        cout << "button A: " << game.a.x << "," << game.a.y << endl;
        cout << "button B: " << game.b.x << "," << game.b.y << endl;
        cout << "prize: " << game.prize.x << "," << game.prize.y << endl;
	cout << endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    PrintGames();
    
    cout << Calculate() << endl;
    //29388
}
