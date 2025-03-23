#include <algorithm>
#include <charconv>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <map>
#include <vector>

using namespace std;
vector<string> input;
vector<vector<int>> mask;
int index = 0;
bool is_do = true;

void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        mask.emplace_back(line.length(), 0);
        input.emplace_back(move(line));
    }
}
const vector<pair<int, int>> directions = {
//  { 0,  0},
    //{ 0, -1},
    { 1, -1},
    //{ 1,  0},
    { 1,  1},
    //{ 0,  1},
    {-1,  1},
    //{-1,  0},
    {-1, -1},
};

bool CheckBoundary(int i, int j) {
    return 0 <= i && i < input.size() && 0 <= j && j < input[0].length();
}

int CountPoint(int i, int j) {
    const string pattern = "MAS";
    if (input[i][j] != pattern[0]) {
        return 0;
    }
    int result = 0;
    for (int l = 0; l < directions.size(); l++) {
        bool f = true;
        for (int k = 1; k < pattern.length(); k++) {
            int i1 = i + k*directions[l].first;
            int j1 = j + k*directions[l].second; 
            if (!CheckBoundary(i1, j1) || input[i1][j1] != pattern[k] ) {
                f = false;
                break;
            }
        }
        if (f) {
            //cout << i << ", " << j << ", " << l << endl;
            mask[i + directions[l].first][j + directions[l].second]++;
        }
    }
    return result;
}

long CountXMAS () {
    int result = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].length(); j++) {
            CountPoint(i, j);
        }
    }
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].length(); j++) {
            if (mask[i][j] >= 2) {
                result++;
            }
        }
    }
    return result;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    long long result = 0;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    result += CountXMAS();
    
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    cout << "result '" << result <<"'" << endl;
    chrono::duration delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "time " << delta.count()/1000.0 << "ms" << endl;
    // test 9
    // cond 1998
}