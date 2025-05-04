#include <algorithm>
#include <charconv>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
vector<string> input;
int index = 0;
bool is_do = true;

unordered_map<int, unordered_set<int>> rules;
vector<vector<int>> updates;
void ReadFile(string filename) {
    ifstream f(filename);
    string line;
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        int i1, i2;
        size_t ind = 0;
        ind = line.find('|');
        line[ind] = 0;
        ind++;
        i1 = stoi(line.c_str());
        i2 = stoi(line.c_str() + ind);
        rules[i1].emplace(i2);
    }
    for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        updates.emplace_back();
        updates.back().push_back(stod(line.c_str()));
        size_t ind = 0;
        while ((ind = line.find(',', ind + 1)) != string::npos) {
            int i = stod(line.c_str() + ind + 1);
            updates.back().push_back(i);
        }
    }
}

bool comp(int l, int r) {
    if (rules.count(l) > 0 && rules[l].count(r) > 0) {
        return true;
    }
    if (rules.count(r) > 0 && rules[r].count(l) > 0) {
        return false;
    }
    return true;
}

bool IsCorrect(vector<int>& update) {
    for (int il = 0; il < update.size(); il++) {
        for (int ir = il + 1; ir < update.size(); ir++) {
            int l = update[il];
            int r = update[ir];
            if (!comp(l, r)) {
                return false;
            }
        }
    }
    return true;
}

long CountIncorrect (void) {
    int result = 0;
    for (int i = 0; i < updates.size(); i++) {
        if (!IsCorrect(updates[i])) {
            sort(updates[i].begin(), updates[i].end(), comp);
            result += updates[i][updates[i].size()/2];
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
    result = CountIncorrect();
    
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    cout << "result '" << result <<"'" << endl;
    chrono::duration delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "time " << delta.count()/1000.0 << "ms" << endl;
    //test 123
    // cond 5331
}