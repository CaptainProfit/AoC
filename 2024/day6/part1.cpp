#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
vector<string> board;
struct Point {
    int x;
    int y;
    Point& operator+=(const Point& lhs) {
        x+=lhs.x;
        y+=lhs.y;
        return *this;
    }
};

Point operator+(Point rhs, const Point& lhs) {
    return rhs+=lhs;
}

int d = 0;
const vector<Point> dirs = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};
Point guard;

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        if (line.find('^') != string::npos) {
            guard.y = board.size() - 1;
            guard.x = line.find('^');
        }
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

long Walk() {
    long marks = 0;
    while (!OutOf(guard)) {
        if (GetPos(guard) != 'X') {
            SetPos(guard, 'X');
            marks++;
        }
        Point new_guard = guard + dirs[d];
        while(!OutOf(new_guard) && GetPos(new_guard) == '#') {
            d++;
            d%=4;
            new_guard = guard + dirs[d];
        }
        guard = new_guard;
        //SetPos(guard, '^');
    }
    return marks;
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
    cout << Walk() << endl;
    PrintBoard();
    // cond 4903
}