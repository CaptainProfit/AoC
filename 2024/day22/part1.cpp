#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>
using namespace std;

vector<uint64_t> prices; 
void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	while(getline(f, line) && !f.eof() && !line.empty()) {
		stringstream ss(line);
		uint64_t val = 0;
		ss >> val;
		prices.push_back(val);
	}
}

template<typename T>
void Print(const vector<T>& v) {
	bool first = true;
	cout << "[";
	for (const T& el : v) {
		if (!first) {
			cout << endl;
//			cout << ", ";
		}
		else {
			first =  false;
		}
		cout << el;
	}
	cout << "]" << endl;
}

uint64_t Next(uint64_t price, int iter = 1) {
	for (int i = 0; i < iter; i++) {
		price ^= price << 6;
		price %= 16777216;
		price ^= price >> 5;
		price %= 16777216;
		price ^= price << 11;
		price %= 16777216;
	}
	return price;
}

vector<uint64_t> Solve() {
	vector<uint64_t> result;
	for(uint64_t price : prices) {
		result.emplace_back(Next(price, 2000));
	}
	return result;
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	//Print(prices);
	vector<uint64_t> result = Solve();
	//Print(result);
	unsigned long long answer = accumulate(result.begin(), result.end(), 0ul);
	
	cout << answer << endl;
	// 14180628689 - correct
}
