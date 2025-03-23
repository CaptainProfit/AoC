#include <algorithm>
#include <chrono>
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

bool CheckWithSkip(const vector<int>& line, size_t skip_index) {
    if (line.size() < 2) {
        return true;
    }
    int start_i = 1;
    int a;
    int b = line[0];
    if (skip_index < 2) {
        start_i = 2;
        if (skip_index == 0) {
            b = line[start_i - 1];
        }
    }
    bool is_inc = (b < line[start_i]);
    for (int i = start_i; i < line.size(); i++) {
        if (i == skip_index) {
            continue;
        }
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

bool CheckWithAnySkip(const vector<int>& line) {
    if (line.size() < 2) {
        return true;
    }
    int a;
    int b = line[0];
    bool is_inc = (b < line[1]);
    bool flag = true;
    for (int i = 1; i < line.size(); i++) {
        a = b;
        b = line[i];
        if ((a < b) != is_inc ||
             a == b ||
             std::abs(a - b) > 3) {
            return CheckWithSkip(line, i) || CheckWithSkip(line, i - 1);
        }
    }
    return true;
}

bool Check(const vector<int>& line) {

    for (int i = 0; i < 2; i++)
    if (CheckWithSkip(line, i)) {
        return true;
    }

    return CheckWithAnySkip(line);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    int result = 0;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int j = 0;
    for (const auto& line : input) {
        result += Check(line);
        if (Check(line)) {
            cout << j << "+ ";
        }
        else {
            cout << j << "- ";
        }
        if (j > 0 && j % 10 == 0) {
            cout << endl;
        }
        j++;
    }
    cout << endl;
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    cout << "result '" << result <<"'" << endl;
    chrono::duration delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "time " << delta.count()/1000.0 << "ms" << endl;
    // test 4
    // random: 662 too high
    // 398
}