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
int index = 0;
bool is_do = true;

vector<pair<int, int>> rules;
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
        rules.emplace_back(i1, i2);
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
bool IsCorrect(vector<int>& update) {
    for (int i = 0; i < rules.size(); i++) {
        int i1 = -1, i2 = -1;
        for(int j = 0; j < update.size(); j++) {
            if (update[j] == rules[i].first) {
                i1 = j;
            }
            if (update[j] == rules[i].second) {
                i2 = j;
            }
        }
         if(i1 != -1 && i2 != -1 && i1 > i2) {
            return false;
        }
    }
    return true;
}

long CountCorrect () {
    int result = 0;
    for (int i = 0; i < updates.size(); i++) {
        if (IsCorrect(updates[i])) {
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
    result = CountCorrect();
    
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    cout << "result '" << result <<"'" << endl;
    chrono::duration delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "time " << delta.count()/1000.0 << "ms" << endl;
    //test 143
    // cond 5588
}