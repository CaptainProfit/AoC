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
string input;
int index = 0;
bool is_do = true;

void ReadFile(string filename) {
    ifstream f(filename);
    getline(f, input, '\0');
}

long Mul(int offs, int len) {
    int a1 = input.find('(', offs);
    int a2 = input.find(',', a1);
    int a3 = input.find(')', a2);
    string_view a(input);
    string_view b(input);
    a = a.substr(a1 + 1, a2 - a1 - 1);
    b = b.substr(a2 + 1, a3 - a2 - 1);
    long x = 0, y = 0;
    from_chars(a.data(), a.data() + a.size(), x);
    from_chars(b.data(), b.data() + b.size(), y);
    //cout << "mul(" << x << "," << y << ")" << endl;
    return x * y* is_do;
}

const map<int, char> states {
    {0, 'm'},
    {1, 'u'},
    {2, 'l'},
    {3, '('},
};

bool issign(const char c) {
    return c =='+' || c == '-';
}

bool isnumsign(const char c) {
    return ('0' <= c && c <= '9') || issign(c);
}

bool isnumcom(const char c) {
    return ('0' <= c && c <= '9') || c ==',';
}

bool isnumbracket(const char c) {
    return ('0' <= c && c <= '9') || c ==')';
}

long GetNext () {
    int begin = index;
    int num_state = 0;
    for (; index < input.length(); index++) {
        char c = input[index];
        switch(num_state) {
            case 0:
                begin = index;
            case 1:
            case 2:
            case 3:
                if (c == states.at(num_state)) {
                    num_state++;
                }
                else {
                    if (c == 'd') {
                        num_state = 20;
                    }
                    else {
                        num_state = 0;
                    }
                }
                break;
            case 4:
                if (isnumsign(c)) {
                    num_state++;
                }
                else {
                    num_state = 0;
                }
                break;
            case 5:
                if (isnumcom(c)) {
                    if (c == ',') {
                        num_state++;
                    }
                }
                else {
                    num_state = 0;
                }
                break;
            case 6:
                if (isnumsign(c)) {
                    num_state++;
                }
                else {
                    num_state = 0;
                }
                break;
            case 7:
                if (isnumbracket(c)) {
                    if(c == ')') {
                        return Mul(begin, index + 1 - begin);
                    }
                }
                else {
                    num_state = 0;
                }
                break;
            case 20:
                if (c == 'o') {
                    num_state = 21;
                }
                else {
                    num_state = 0;
                }
                break;
            case 21:
                if (c == 'n') {
                    num_state = 22;
                }
                else {
                    is_do = true;
                    num_state = 0;
                }
                break;
            case 22:
                if (c == '\'') {
                    num_state = 23;
                }
                else {
                    is_do = true;
                    num_state = 0;
                }
                break;
            case 23:
                if (c == 't') {
                    is_do = false;
                }
                num_state = 0;
                break;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    long long result = 0;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    while (index < input.size()) {
        result += GetNext();
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    cout << "result '" << result <<"'" << endl;
    chrono::duration delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "time " << delta.count()/1000.0 << "ms" << endl;
    //test 42
    // mul\([+-]?[1-9][0-9]*\,[+-]?[1-9][0-9]*\)
    // 92626942 correct
}