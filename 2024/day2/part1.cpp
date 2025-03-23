#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;
vector<vector<int>> input;

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for (getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        input.emplace_back();
        size_t old_delim = 0;
        for (size_t delim = line.find(' '); ; delim = line.find(' ', old_delim + 1)) {
            string sub = line.substr(old_delim, delim - old_delim);
            input.back().push_back(stod(sub));
            old_delim = delim;
            if (delim == string::npos) {
                break;
            }
        }
    }
}

bool Check (const vector<int>& line) {
    if (line.size() < 2) {
        return true;
    }
    int a = line[0];
    int b = line[0];
    bool is_inc = (a < b);
    for (int i = 1; i < line.size(); i++) {
        a = b;
        b = line[i];
        if ((a < b) != is_inc) {
            return false;
        }
        if (a == b || std::abs(a - b) > 3) {
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    int result = 0;
    for (const auto& line : input) {
        result += Check(line);
    }
    
    cout << result << endl;
    // test 2
    // 331 too low
    // 355 too high
}