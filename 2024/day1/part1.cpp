#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

void ReadFile(string filename, vector<int> &l1, vector<int> &l2) {
    ifstream f(filename);
    string line;
    for (getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
        int delim = line.find(' ');
        string right = line.substr(delim);
        line.resize(delim);
        l1.push_back(stod(line));
        l2.push_back(stod(right));
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    vector<int> l1;
    vector<int> l2;
    ReadFile(argv[1], l1, l2);
    sort(l1.begin(), l1.end());
    sort(l2.begin(), l2.end());
    int result = 0;
    for(int i = 0; i < l1.size(); i++) {
        result+=std::abs(l1[i] - l2[i]);
    }
    cout << result << endl;
    //test = 11
    //1722302
}