#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
using namespace std;

vector<uint64_t> init_codes; 
void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	while(getline(f, line) && !f.eof() && !line.empty()) {
		stringstream ss(line);
		uint64_t val = 0;
		ss >> val;
		init_codes.push_back(val);
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

vector<uint64_t> GenerateCodes(uint64_t code, int iter = 1) {
	vector<uint64_t> result;
	result.push_back(code);
	for (int i = 0; i < iter; i++) {
		code ^= code << 6;
		code %= 16777216;
		code ^= code >> 5;
		code %= 16777216;
		code ^= code << 11;
		code %= 16777216;
		result.push_back(code);
	}
	return result;
}

vector<signed char> GenerateCosts(vector<uint64_t>&& codes) {
	vector<signed char> result;
	for (uint64_t code : codes) {
		result.push_back(code % 10);
	}
	return result;
}

/*bool operator<(const vector<signed char>& lhs, const vector<signed char>& rhs) {
	for (int i = 0; i < lhs.size() && i < rhs.size(); i++) {
		if (lhs[i] > rhs[i]) {
			return false;
		}
		if (lhs[i] < rhs[i]) {
			return true;
		}
	}
	return lhs.size() < rhs.size();
}*/
template<>
struct std::hash<vector<signed char>>
{
    size_t operator()(const vector<signed char>& s) const noexcept
    {
		assert(s.size() == 4);
        size_t h1 = hash<signed char>{}(s[0]);
		size_t h2 = hash<signed char>{}(s[1]);
		size_t h3 = hash<signed char>{}(s[2]);
		size_t h4 = hash<signed char>{}(s[3]);
        
        return h1 ^ (h2 * 31) ^ (h3 * 31 * 31) ^ (h4 * 31 * 31 * 31); // or use boost::hash_combine
    }
};

unordered_set<vector<signed char>> any_sequence;
unordered_map<vector<signed char>, signed char> GenerateSequences(vector<signed char>&& costs) {
	vector<signed char> seq;
	unordered_map<vector<signed char>, signed char> result; 
	optional<signed char> prev_cost;
	for (signed char cost : costs) {
		if (!prev_cost) {
			prev_cost = cost;
			continue;
		}
		signed char diff = cost - prev_cost.value();
		prev_cost = cost;

		seq.insert(seq.begin(), diff);
		if (seq.size() < 4) {
			continue;
		}
		if (seq.size() > 4) {
			seq.pop_back();
		}
		if(result.count(seq) != 0) {
			continue;
		}
		any_sequence.insert(seq);
		result.emplace(seq, cost);
	}
	return result;
}

uint64_t Solve() {
	vector<unordered_map<vector<signed char>, signed char>> buyers;
	//vector<uint64_t> init_codes = {123};
	for(uint64_t init_code : init_codes) {
		//cout << init_code << endl;
		/*auto codes(GenerateCodes(init_code, 2000));
		auto prices(GenerateCosts(codes));
		auto sequences(GenerateSequences(prices));
		buyers.push_back(sequences);*/
		buyers.emplace_back(
			move(
			GenerateSequences(
			GenerateCosts(
			GenerateCodes(init_code, 2000)
			))));
	}
	optional<uint64_t> result;
	/*{
		vector <signed char> seq = {3, -1, 1, -2};
		//vector <signed char> seq = {2, 0, -1, -1};
		uint64_t profit = 0;
		for (auto& buyer : buyers) {
			if (buyer.count(seq) == 0) {
				cout << "  none" << endl;
				continue;
			}
			profit += buyer[seq];
			cout << "  " << (int)buyer[seq] << endl;
		}
		if (!result || result < profit) {
			result = profit;
		}
		cout << "seq result: " << result.value() << endl;
	}*/
	cout << "testing sequences " << endl;
	for (auto& seq : any_sequence) {
		uint64_t profit = 0;
		for (auto& buyer : buyers) {
			if (buyer.count(seq) == 0) {
				continue;
			}
			profit += buyer[seq];
		}
		if (!result || result < profit) {
			result = profit;
		}
	}
	return result.value();
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	//Print(prices);
	uint64_t answer = Solve();
	cout << "result: " << answer << endl;
	// 1690 correct
}
