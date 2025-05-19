#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cassert>
#include <memory>

using namespace std;

class Trie{
	shared_ptr<Trie> kids[26];
	bool terminal;
	const string_view* pattern;
public:
	Trie(): terminal(false) {}
	Trie(set<string_view> words): 
		Trie() {
		for(const string_view& word : words) {
			AddWord(word);
		}
	}
	
	void AddWord(const string_view& word) {
		Trie* ptr(this);
		assert(!word.empty());
		for (int i = 0; i < word.length(); i++) {
			int it = word[i] - 'a';
			if (ptr->kids[it] == nullptr) {
				ptr->kids[it] = make_shared<Trie>();
			}
			if (i == word.length() - 1) {
				ptr->kids[it]->terminal = true;
				ptr->kids[it]->pattern = &word;
			}
			ptr = ptr->kids[it].get();
		}
	}

	void Print(ostream& out, int level = 0) const {
		string ofs(level, ' ');
		for (int i = 0; i < 26; i++) {
			if (!kids[i].get()) {
				continue;
			}
			string term;
			if (kids[i]->terminal) {
				term = *(kids[i]->pattern);
				term = "("s + term + ")"s;
			}
			out << ofs <<  char('a' + i) << term << ":" << endl;
			kids[i]->Print(out, level + 1);
		}
	}
	unsigned long long Count(const string_view& str, vector<string_view>& stack, Trie* parent = nullptr) {
		if (parent == nullptr) {
			parent = this;
		}
		unsigned long long accumulator = 0;
		int id = str[0] - 'a';
		Trie* next = kids[id].get();
		if (!next) {
			return 0;
		}
		if (next->terminal) {
			if (str.size() == 1) {
				return 1;
			}
			accumulator += parent->Count(str.substr(1), stack, parent);
		}
		if (str.size() != 1) {
			accumulator += next->Count(str.substr(1), stack, parent);
			return accumulator;
		}
		return 0;
	}

	~Trie() {}
};

ostream& operator<<(ostream& out, Trie tr) {
	tr.Print(out);
	return out;
}

string towel_entities;
set<string_view> towels;
vector<string> designs;

void ReadFile(string filename) {
	ifstream f(filename);
	getline(f, towel_entities);
	size_t p0 = 0;
	while(true) {
		size_t p1 = towel_entities.find(',', p0);
		string_view towel(towel_entities);
		towel = towel.substr(p0, p1 - p0);
		towels.emplace(move(towel));
		if (p1 == string::npos) {
			break;
		}
		p0 = p1 + 2;
	}
	string line;
	getline(f, line);
	assert(line.empty());
	for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
		designs.emplace_back(move(line));
	}
}

void Print() {
	cout << "{";
	for (const string_view& s : towels) {
		cout << "\"" << s << "\",";
	}
	cout << "}" << endl;
	for (const string& s : designs) {
		cout << s << endl;
	}
}

void Solve() {
	Print();
	Trie tr(towels);
	cout << tr << endl;
	int success = 0;
	for (const string& design : designs) {
		vector<string_view> solution;
		int a = tr.Count(design, solution);
		success += a;
		
		cout << design << ": " << boolalpha << a << endl;
	}
	cout << "# of available designs: " << success << endl;
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	Solve();
}
