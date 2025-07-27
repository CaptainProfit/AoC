#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <cassert>

#include "walker.hpp"
#include "position.hpp"
using namespace std;
#define DEPTH 7
const Point invalid = {-1, -1};
const vector<string> panel_move = {
	" ^A"s,
	"<V>"
};
const Point pm0 = {0, 2};
const unordered_map<char, Point> pm2pt ={
	{' ', {0, 0}},
	{'^', {0, 1}},
	{'A', {0, 2}},
	{'<', {1, 0}},
	{'V', {1, 1}},
	{'>', {1, 2}}
};
const map<Point, char> pt2pm ={
	{{0, 0}, ' '},
	{{0, 1}, '^'},
	{{0, 2}, 'A'},
	{{1, 0}, '<'},
	{{1, 1}, 'V'},
	{{1, 2}, '>'}
};

const vector<string> panel_code = {
	"789"s,
	"456"s,
	"123"s,
	" 0A"s
};
const Point pc0 = {3, 2};
const unordered_map<char, Point> pc2pt = {
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

struct Panel {
	Panel(const vector<string>& pane) :
		pane_(pane){
	}
	~Panel() = default;
	char Get(Point pt) const {
		return pane_[pt.y][pt.x];
	}
	bool OutOfRange(Point pt) const {
		if (pt.x < 0 || pt.x >= pane_[0].length()) {
			return true;
		}
		if (pt.y < 0 || pt.y >= pane_.size()) {
			return true;
		}
		if (Get(pt) == ' ') {
			return true;
		}

		return false;
	}
	const char operator[](Point pt) const {
		return Get(pt);
	}
	const vector<string>& pane_;
};

#define CONCAT3(y, x) y ## x
#define CONCAT2(y, x) CONCAT3(y, x)
#define PANELS CONCAT2(panels, DEPTH)
#define START CONCAT2(start, DEPTH)
#define FINISH CONCAT2(finish, DEPTH)

const vector<Panel> panels7{panel_move, panel_move, panel_move, panel_move, panel_move, panel_move, panel_code};
const vector<Panel> panels6{panel_move, panel_move, panel_move, panel_move, panel_move, panel_code};
const vector<Panel> panels5{panel_move, panel_move, panel_move, panel_move, panel_code};
const vector<Panel> panels4{panel_move, panel_move, panel_move, panel_code};
const vector<Panel> panels3{panel_move, panel_move, panel_code};
const vector<Panel> panels2{panel_move, panel_code};
const vector<Panel>& panels = PANELS;


class Panels {

	Panels(...) = delete;
	static optional<Position> Press(Position current, char x, int pid = 0) {
		if (pid == 0) {
			assert(pm2pt.count(x) > 0);
			Point pt = pm2pt.at(x);
			current.pos[0] = pt;
			if (panels[0].OutOfRange(current.pos[0])) {
				return {};
			}
		}
		switch(x) {
			case 'A' :
				if (pid < DEPTH - 1) {
					char next_x = panels[pid + 1].Get(current.pos[pid + 1]);
					return Press(current, panels[pid + 1].Get(current.pos[pid + 1]), pid + 1);
				}
				else {
					return current;
				}
			case '^' :
			case '<' :
			case 'V' :
			case '>' :
				if (c2dirs.count(x) > 0) {
					current.pos[pid + 1] += c2dirs.at(x);
					if (panels[pid + 1].OutOfRange(current.pos[pid + 1])) {
						return {};
					}
				}
				return current;
			default : // '0-9
				return current;
		}
	}
public:
	static void PressAllButtons(const Position& current, std::function<void(Position next)> func) {
		for (int i = 0; i < panel_move.size(); i++) {
			for (int j = 0; j < panel_move[i].length(); j++) {
				char button = panel_move[i][j];
				optional<Position> next = Press(current, button);
				if (next) {
					func(next.value());
				}
			}
		}
	}
	
};

Position state;

vector<string> ReadFile(string filename) {
	vector<string> codes;
	ifstream f(filename);
	string line;
	for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
		codes.push_back(line);
	}
	return codes;
}

class PanelWalker : public Walker<Position> {
	Position finish_;
public:
	void SetFinish(Position finish) {
		finish_ = finish;
	}
protected:
	void VisitIndicies(const Position& current, std::function<void(const Position& next, const size_t cost)> func) override {
		size_t cost = costs_.at(current);
		Panels::PressAllButtons(current, [&func, &cost, this](Position next) {
			bool skip = (next.pos[DEPTH - 1] != finish_.pos[DEPTH - 1]);
			for (int i = 0; i < DEPTH - 1; i++) {
				if (!skip) {
					break;
				}
				skip &= panels[i][next.pos[i]] == 'A';
			}
			if (!skip) {
				func(next, cost + 1);
			}
		});
	}
};

template <typename U, typename V>
void PrintMap(map<U, V>& m) {
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

template <typename U>
void PrintMap(vector<U>& m) {
	bool first = true;
	cout << "{";
	for (auto& key : m) {
		if (!first) {
			cout << ", ";
		}
		first = false;
		cout << key;
	}
	cout << "}";
}

string ptpath2cpath(const vector<Position>& ptpath ) {
	string result;
	result.reserve(ptpath.size());
	for (int i = ptpath.size() - 2; i >= 0; i--) {
		Point pt =  ptpath[i].pos[0];
		result.push_back(pt2pm.at(pt));
	}
	return result;
}

Position SetStartPosition() {
	Position result;
	result.pos.resize(DEPTH);
	result.pos[0] = invalid;
	for (int i = 1; i < DEPTH - 1; i++) {
		result.pos[i] = pm0;
	}
	result.pos[DEPTH - 1] = pc0;
	return result;
}

Position SetFinishPosition(char x) {
	Position result;
	result.pos.resize(DEPTH);
	for (int i = 0; i < DEPTH - 1; i++) {
		result.pos[i] = pm0;
	}
	result.pos[DEPTH - 1] = pc2pt.at(x);
	return result;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return -1;
	}
	vector<string> codes = ReadFile(argv[1]);	
	long long result = 0;
	PanelWalker pw;
	for (string& code : codes) {
		cout << code << endl;
		string codepath;
		int num_part = stoi(code);
		Position start = SetStartPosition();
		char cstart = 'A';
		for (char c : code) {
			Position finish = SetFinishPosition(c);
			pw.SetFinish(finish);
			vector<Position> path = pw.FindPath(start, finish);
			auto reachable = pw.FindReachableSpace(start);
			string cpath = ptpath2cpath(path);
			//cout << cpath << endl;
			cout << cstart << " -> " << c << " cost " << cpath.length() << " \"" << cpath << "\"" << endl;
			codepath+=cpath;
			start = finish;
			cstart = c;
		}
		cout << " path \"" << codepath << "\""<< endl;
		cout << " length \"" << codepath.length() << "\""<< endl;
		result += num_part * codepath.length();
		cout << "  code complexity " << num_part * codepath.length() << endl;
	}
	cout << "total complexity is " << result << endl;
	// 177'814
}