#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <unordered_map>
#include <set>
#include <vector>
#include <cassert>

//#include "walker.hpp"
//#include "position.hpp"
#include "point.hpp"
using namespace std;
#define DEPTH 10
const Point invalid = {-1, -1};
const Point pm0 = {0, 2};
const Point pc0 = {3, 2};

const vector<string> panel_move = {
	" ^A"s,
	"<V>"s
};
const vector<string> panel_code = {
	"789"s,
	"456"s,
	"123"s,
	" 0A"s
};
				
const map<char, const Point> pm2pt = {
	{' ', {0, 0}},
	{'^', {0, 1}},
	{'A', {0, 2}},
	{'<', {1, 0}},
	{'V', {1, 1}},
	{'>', {1, 2}}
};
const map<Point, const char> pt2pm = {
	{{0, 0}, ' '},
	{{0, 1}, '^'},
	{{0, 2}, 'A'},
	{{1, 0}, '<'},
	{{1, 1}, 'V'},
	{{1, 2}, '>'}
};

const map<char, const Point> pc2pt = {
	{'7', {0, 0}},
	{'8', {0, 1}},
	{'9', {0, 2}},
	{'4', {1, 0}},
	{'5', {1, 1}},
	{'6', {1, 2}},
	{'1', {2, 0}},
	{'2', {2, 1}},
	{'3', {2, 2}},
	{' ', {3, 0}},
	{'0', {3, 1}},
	{'A', {3, 2}}
};

const map<Point, const char> pt2pc = {
	{{0, 0}, '7'},
	{{0, 1}, '8'},
	{{0, 2}, '9'},
	{{1, 0}, '4'},
	{{1, 1}, '5'},
	{{1, 2}, '6'},
	{{2, 0}, '1'},
	{{2, 1}, '2'},
	{{2, 2}, '3'},
	{{3, 0}, ' '},
	{{3, 1}, '0'},
	{{3, 2}, 'A'}
};

const map<Point, const char> dir2pm = {
	{{-1, 0}, '^'},
	{{0, -1}, '<'},
	{{1, 0}, 'V'},
	{{0, 1}, '>'},
};

vector<string> ReadFile(string filename) {
	vector<string> codes;
	ifstream f(filename);
	string line;
	for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
		codes.push_back(line);
	}
	return codes;
}

template <typename U>
void Print(const vector<U>& m) {
	bool first = true;
	cout << "{";
	for (auto& elem : m) {
		if (!first) {
			cout << ", ";
		}
		first = false;
		cout << elem;
	}
	cout << "}";
}

template <typename U, typename V>
void Print(const multimap<U, V>& m) {
	bool first = true;
	cout << "{\n";
	for (auto& [key, value] : m) {
		if (!first) {
			cout << ",\n";
		}
		first = false;
		cout << "\t{" << key << ", " << value << "}";
	}
	if (!first) {
		cout << "\n";
	}
	cout << "}";
}

template <typename U, typename V>
void Print(const map<U, V>& m) {
	bool first = true;
	cout << "{\n";
	for (auto& [key, value] : m) {
		if (!first) {
			cout << ",\n";
		}
		first = false;
		cout << "\t{" << key << ", " << value << "}";
	}
	if (!first) {
		cout << "\n";
	}
	cout << "}";
}

bool InBound(const Point pt, const vector<string>* panel) {
	if (pt.y < 0 || pt.y >= panel->size()) {
		return false;
	}
	if (pt.x < 0 || pt.x >= (*panel)[pt.y].length()) {
		return false;
	}
	return (*panel)[pt.y][pt.x] != ' ';
}

vector <string> GetPaths(char b, char e, const vector<string>* const panel, 
									const map<char, const Point>* const pg2pt,
									const map<Point, const char>* const pt2pg) {
	multimap <long, Point> que;
	set<Point> marked;
	Point start = pg2pt->at(b);
	Point finish = pg2pt->at(e);
	multimap <Point, string> strpaths =  {{start, ""}};

	que.emplace(0, start);
	long cnt = -1;
	while(!que.empty()) {
		auto it = que.begin();
		if (cnt != -1 && cnt < it->first) {
			break;
		}
		
		Point pt = it->second;
		long lev = it->first;
		que.erase(it);
		if (marked.count(pt)) {
			continue;
		}
		if (pt == finish) {
			cnt = lev;
		}

		for (int i = 0; i < 4; i++) {
			Point next = pt + dirs[i];
			if (!InBound(next, panel)) {
				continue;
			}
			for(auto it = strpaths.lower_bound(pt); it != strpaths.upper_bound(pt); it++) {
				strpaths.emplace(next, it->second + dir2pm.at(dirs[i]));
			}
			if (!marked.count(next)) {
				que.emplace(lev + 1, next);
			}
		}
		marked.insert(pt);
	}
	vector<string> result;
	for (auto it = strpaths.lower_bound(finish); it != strpaths.upper_bound(finish); it++) {
		result.emplace_back(it->second + "A"s);
	}
	return result;
}

static multimap<string, string> panel_mapper;
void RemoveByKV(const string& key, const string& value) {
	for (auto it = panel_mapper.lower_bound(key); it != panel_mapper.upper_bound(key); it++) {
		if (it->second == value) {
			panel_mapper.erase(it);
			break;
		}
	}
}

void Init () {
	const string buttons = "<V>^A";
	for (char b : buttons) {
		for (char e : buttons) {
			string key = {b, e};
			for (string value : GetPaths(b, e, &panel_move, &pm2pt, &pt2pm)) {
				panel_mapper.emplace(key, value);
			}
		}	
	}

	//abuse doubling
	RemoveByKV("<A"s, ">^>A"s);
	//RemoveByKV("<A"s, ">>^A"s);

	//abuse doubling
	RemoveByKV("A<"s, "<V<A"s);
    //RemoveByKV("A<"s, "V<<A"s);

	// no diff?? less branchy
	RemoveByKV("^>"s, "V>A"s);
    //RemoveByKV("^>"s, ">VA"s);
		
	// better
	//RemoveByKV(">^"s, "<^A"s);
    RemoveByKV(">^"s, "^<A"s);

	// <V better than V<
	//RemoveByKV("AV"s, "<VA"s); 
    RemoveByKV("AV"s, "V<A"s);

	// ???
	//RemoveByKV("VA"s, ">^A"s);
    RemoveByKV("VA"s, "^>A"s);

	cout << panel_mapper.size() << endl;
	//Print(panel_mapper);
	//cout << endl;

}

// {"V"} -> {"V<A", "<VA"} -> {"V<A", "<VA"} -> {"V<A<A>>^A", "<VA<A>>^A", "V<A<A>^>A", "<VA<A>^>A", ...}
vector<string> IteratePath(const string& path) {
	vector<string> result={""};
	vector<string> result_temp;
	for (long i = 0; i < path.length(); i++) {
		string key;
		if (i == 0) {
			key = "A"s;
			key += string(1, path[0]);
		}
		else {
			key = {path[i - 1], path[i]};
		}
		for(auto path : result) {
			for(auto iter = panel_mapper.lower_bound(key); iter != panel_mapper.upper_bound(key); iter++) {
				result_temp.emplace_back(path + iter->second);
			}
		}
		result = move(result_temp);
	}
	return result;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return -1;
	}
	vector<string> codes = ReadFile(argv[1]);	
	
	long long result = 0;
	Init();	
	//Print(panel_mapper);
	vector<string> keyorders = {"V"s};
	for (int iter = 0; iter < 25; iter++) {
		vector<string> new_keyorders;
		long min_size = -1;
		for (auto& keyorder : keyorders) {
			for (auto& elem : IteratePath(keyorder)) {
				if (min_size == -1 || min_size > elem.length() ) {
					min_size = elem.length();
					new_keyorders.clear();
				}
				if (elem.length() == min_size) {
					new_keyorders.emplace_back(move(elem));
				}
			}
		}
		keyorders = move(new_keyorders);
		map <long, long> keyorders_hist;
		for (auto& keyorder : keyorders) {
			keyorders_hist[keyorder.length()]++;
		}
		cout << "iter : " << iter + 1 << endl;
		Print(keyorders_hist);
		cout << endl;
		cout << "------"s << endl;
		/*
		iter 1: {3, 2}
		iter 2: {9, 4}
		iter 3: {21, 8},
		iter 4: {55, 384},
		iter 5: {135, 1'114'112}
		iter 6: { , ...}
		...
		iter 13 {209'183 , 1}
		iter 14 {523'691 , 1}
		iter 14 {1'310'803 , 1}
		*/
	}
	return -1;
	for (string& code : codes) {
		string codepath;
		int num_part = stoi(code);
		Point start = pc2pt.at('A');
		for (char c : code) {
			Point checkpoint_ = pc2pt.at('A');
		}
		cout << "for code " << code << " combination \"" << codepath << "\""<< endl;
		result += num_part * codepath.length();
		cout << "code complexity " << num_part * codepath.length() << endl;
	}
}