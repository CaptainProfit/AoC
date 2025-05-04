#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

map<unsigned long long, unsigned long long> stones;

void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	getline(f, line);
	size_t it = 0;
	while(it != string::npos) {
		stones[stol(line.c_str() + it)]++;
		it = line.find(' ', it + 1);
	} 
}

/*
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
}*/

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
	map<unsigned long long, unsigned long long> new_stones;
	for (auto& [stone, amount] : stones) {
		if (stone == 0) {
			new_stones[1] += amount;
			continue;
		}
		if (Len(stone) % 2 == 0) {
			new_stones[Right(stone)] += amount;
			new_stones[Left(stone)] += amount;
			continue;
		}
		new_stones[stone*2024] += amount;
	}
	stones = move(new_stones);
}

void Solve() {
	//cout << stones << endl;
	for (int i = 0; i < 75; i++) {
		Blink();
	//	cout << stones << endl;
		unsigned long long result = 0;
		for (auto& [stone, amount] : stones) {
			result += amount;
		}
		cout << result << endl; 
    	}
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    Solve();
    // 1216318908621637
}
