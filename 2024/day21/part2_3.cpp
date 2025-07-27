#include <algorithm>
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
#define ITERS 25
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
static unordered_map<string, string> panel_mapper_unique;
static map<string, multiset<string>> panel_mapper_stat;
void RemoveByKV(const string& key, const string& value) {
	for (auto it = panel_mapper.lower_bound(key); it != panel_mapper.upper_bound(key); it++) {
		if (it->second == value) {
			panel_mapper.erase(it);
			break;
		}
	}
}

void Compress() {
	for (auto& [k, v] : panel_mapper) {
		if (panel_mapper_unique.count(k) > 0) {
			cerr << "uncertaincy!" << endl;
		}
		panel_mapper_unique[k] = v;
	}
}

void CompressEvenMore() {
	for (auto [k, v] : panel_mapper_unique) {
		v = "A"s + v; 
		for (int i = 0; i < v.length() - 1; i++) {
			panel_mapper_stat[k].insert({v[i], v[i + 1]});
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
	//RemoveByKV("^>"s, "V>A"s);
    RemoveByKV("^>"s, ">VA"s);
		
	// better
	//RemoveByKV(">^"s, "<^A"s);
    RemoveByKV(">^"s, "^<A"s);

	// <V better than V<
	//RemoveByKV("AV"s, "<VA"s); 
    RemoveByKV("AV"s, "V<A"s);

	// yep, it worse 
	RemoveByKV("VA"s, ">^A"s);
    //RemoveByKV("VA"s, "^>A"s);

	cout << panel_mapper.size() << endl;
	//Print(panel_mapper);
	//cout << endl;
	Compress();
	CompressEvenMore();
}

unordered_map<string, long long> IteratePath(unordered_map<string, long long> path_stat) {
	//path_stat[{'A', first_char}]++;
	unordered_map<string, long long>  result;
	for (auto& [key, amount] : path_stat) {
		for (auto& new_key : panel_mapper_stat[key]) {
			result[new_key] += amount;
		}
	}
	return result;
}

map <string, long long> costs;
void Calibrate() {
	for (auto& prev_button : {'V', '<', '>', '^', 'A'}) {
		for (auto& button : {'V', '<', '>', '^', 'A'}) {
			unordered_map<string, long long> ko_stat ={{{prev_button, button}, 1}};
			for (int iter = 0; iter < ITERS; iter++) {
				unordered_map<string, long long> new_ko_stat;
				new_ko_stat = IteratePath(ko_stat);
				ko_stat = move(new_ko_stat);
				
				long long acc = 0;
					for (auto& [k, v] : ko_stat) {
					acc += v;
				}
				costs[{prev_button, button}] = acc;
			}
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return -1;
	}
	vector<string> codes = ReadFile(argv[1]);	
	
	long long total_complexity = 0;
	cout << "init" << endl;
	Init();
	cout << "count button costs" << endl;
	Calibrate();
	cout << "count codes" << endl;
	
	// 480A < fails on 
	for (string& code : codes) {
		string codepath;
		int num_part = stoi(code);
		cout << code << endl;
		char start = 'A';

		long long total_path_len = 0;
		for (char c : code) {
			long min_path_len = - 1;
			auto paths = GetPaths(start, c, &panel_code, &pc2pt, &pt2pc);
			for(auto& path : paths) {
				string subpath;
				if (path.length() > 10) {
					subpath = path.substr(path.length() - 10);
				}
				long path_len = 0;
				for (int i = 0; i < path.length(); i++) {
					string key;
					if (i == 0) {
						key = {'A', path[0]};
					}
					else {
						key = {path[i - 1], path[i]};
					}
					path_len += costs[key];
				}
				if (min_path_len == -1 || min_path_len > path_len) {
					min_path_len = path_len;
				}
			}
			cout << "  " <<  start << " -> " << c << " len: " << min_path_len << endl;
			start = c;
			total_path_len += min_path_len;
		}
		cout << "  for code " << code << " length \"" << total_path_len << "\""<< endl;
		total_complexity += num_part * total_path_len;
		cout << "  code complexity " << num_part * total_path_len << endl;
	}
	cout << "answer total complexity: " << total_complexity << endl;
	// 661'178'790'579'123 - too high 26
	// 264'134'451'047'519 - too high 25
	// 251'582'081'668'838 - nope
	// 220'493'992'841'852 - correct
	// 105'519'163'466'651 - too low 24

	// 480A < fails on 4th iter: expected 216000
	//len 450
}
