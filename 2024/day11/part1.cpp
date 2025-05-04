#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<unsigned long long> stones;

void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	getline(f, line);
	size_t it = 0;
	while(it != string::npos) {
		stones.push_back(stol(line.c_str() + it));
		it = line.find(' ', it + 1);
	} 
}

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v) {
	cout << "[";
	bool flag = false;
	for (auto& a : v) {
		if (flag) {
			cout << ", ";
		}
		flag = true;
		cout << a;
	}
	cout << "]";
}

int Len(unsigned long long l) {
	if (l == 0) {
		return 1;
	}
	int r = 0;	
	while (l > 0) {
		r++;
		l /= 10;
	}
	return r;
}

unsigned long long Pow(int q) {
	unsigned long long result = 1;
	while (q-- > 0) {
		result *= 10;
	}
	return result;
}

unsigned long long Right(unsigned long long l) {
	return l % Pow(Len(l)/2);
}

unsigned long long Left(unsigned long long l) {
	return l / Pow(Len(l)/2);
}

void Blink() {
	vector<unsigned long long> new_stones;
	for (auto& stone : stones) {
		if (stone == 0) {
			stone = 1;
			continue;
		}
		if (Len(stone) % 2 == 0) {
			new_stones.push_back(Right(stone));
			stone = Left(stone);
			continue;
		}
		stone *= 2024;
	}
	for (auto stone : new_stones) {
		stones.push_back(stone);
	}
}

void Solve() {
	cout << stones << endl;
	for (int i = 0; i < 25; i++) {
		Blink();
	//	cout << stones << endl;
		cout << stones.size() << endl; 
		//char c;
		//cin >> c;
    	}
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    Solve();
    // 182081
}
