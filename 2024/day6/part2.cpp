#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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
long critical = 0;
const vector<Point> dirs = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};

struct Board {
    Point guard;
    vector<string> board;
    int d = 0;
    bool OutOf(Point pt) {
        return pt.x < 0 || pt.y < 0 || pt.x >= board[0].length() || pt.y >= board.size();
    }
    
    char GetPos(Point pt) {
        return board[pt.y][pt.x];
    }
    
    void SetPos(Point pt, char c) {
        board[pt.y][pt.x] = c;
    }
    
    long WalkAndStone(bool stone = true) {
        long marks = 0;
        while (!OutOf(guard)) {
            if (!stone) {
                marks++;
            }
            Point new_guard = guard + dirs[d];
            while(!OutOf(new_guard) && GetPos(new_guard) == '#') {
                d++;
                d%=4;
                new_guard = guard + dirs[d];
            }
            if (stone && !OutOf(new_guard) && GetPos(new_guard) != 'O') {
                SetPos(new_guard, 'O');
                Board copy(*this);
                copy.SetPos(new_guard, '#');
                if (copy.WalkAndStone(false) > critical) {
                    marks++;
                }
            }
            guard = new_guard;
            if (marks > critical) {
                return marks;
            }
            //SetPos(guard, '^');
        }
        return marks;
    }
    
    void PrintBoard() {
        for (auto& line : board) {
            cout << line << endl;
        }
    }
    
};

void ReadFile(string filename, Board& start) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        if (line.find('^') != string::npos) {
            start.guard.y = start.board.size() - 1;
            start.guard.x = line.find('^');
        }
        start.board.emplace_back(move(line));
    }
    critical = start.board.size()*start.board[0].length();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    Board start;
    ReadFile(argv[1], start);
    cout << start.WalkAndStone() << endl;
    start.PrintBoard();
    //2140 too high
    //1911
}