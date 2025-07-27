#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>

#include "timer.hpp"
//#include "point.hpp"
#include "board.hpp"
using namespace std;

Board board;

void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	for(getline(f, line); !f.eof() && !line.empty(); getline(f, line)) {
		size_t j = line.find('S');
		size_t i = board.Height();
		if (j != string::npos) {
			board.SetStart({(int)i, (int)j});
		}
		j = line.find('E');
		if (j != string::npos) {
			board.SetFinish({(int)i, (int)j});
		}
        board.AddLine(move(line));
	}
}


template<typename T>
struct GraphMover {
	const T& start;
	const T& target;
	GraphMover(Board& board):
		board_(board),
		start(board.GetStart()),
		target(board.GetFinish()) {
		}
	~GraphMover() = default;
		
		void Add(const T& prev, T& node, size_t cost) {
		if (board_.OutOf(node.pos)) {
			return;
		}
		if (board_.Get(node.pos) == '#') {
			if (node.mana == 0) {
				return;
			}
			else {
				node.mana--;
			}
		}
		/*if (marks.count(node)) {
			return;
		}*/
		if (costs.count(node) == 0 || costs[node] > cost) {
			froms[node] = prev;
			costs[node] = cost;
			que.insert({cost, node});
		}		
	}
	
	void Action(const T& p) {
		size_t cost = costs.at(p);
		for (int i = 0; i < 4; i++) {
			Position new_p(p);
			new_p.pos += dirs[i];
			Add(p, new_p, cost + 1);
		}
	}
	
	void Walk() {
		que.insert({0, board_.GetStart()});
		costs.insert({board_.GetStart(), 0});
		while(!que.empty()) {
			T p = que.begin()->second;
			que.erase(que.begin());
			if (marks.count(p)) {
				continue;
			}
			marks.insert(p);
			Action(p);
		}
	}
	
	size_t Target() {
		size_t result = string::npos;
		if (costs.count(target)) {
			result = min(result, costs[target]);
		}
		return result;
	}
	
	void PrintPath() {
		image_ = vector<vector<int>>(board_.Width(), vector<int>(board_.Height(), -1));
		board_.Set(target.pos, 'X');
		board_.Set(start.pos, 'O');
		image_[target.pos.y][target.pos.x] = 0;
		auto p = target;
		for (;froms.count(p); p = froms[p]) {
			image_[froms[p].pos.y][froms[p].pos.x] = 
				image_[p.pos.y][p.pos.x] + 1;
			board_.Set(froms[p].pos, 'O');
		}
		/*for (auto [p, v]: froms) {
			board_.Set(v.pos, '*');
		}*/
	}

	bool IsHoris(const int y, const int x) {
		return image_[y][x - 1] != -1 &&
			   image_[y][x + 1] != -1;
	}

	bool IsVert(const int y, const int x) {
		return image_[y - 1][x] != -1 &&
			   image_[y + 1][x] != -1;
	}


	long long FilterPath() {
		int threshold = 100;
		threshold += 2;
		long long counter = 0;
		for (int i = 1; i < image_.size() - 1; i++) {
			for (int j = 1; j < image_[0].size() - 1; j++) {
				if (image_[i][j] != -1) {
					continue;
				}
				if (IsHoris(i, j) && std::abs(image_[i][j + 1] - image_[i][j - 1]) >= threshold) {
					counter++;
				}

				if (IsVert(i, j) && std::abs(image_[i - 1][j] - image_[i + 1][j]) >= threshold) {
					counter++;
				}
			}
		}
		return counter;
	}

private:
	map<T, size_t> costs;
	map<T, T> froms;
	set<T> marks;
	multimap<size_t, T> que;
	Board& board_;
	vector<vector<int>> image_;
};

int main(int argc, char** argv) {
	if (argc < 2) {
		return -1;
	}
	ReadFile(argv[1]);
	//board.Print(cout);
	GraphMover<Position> gr(board);
	gr.Walk();
	gr.PrintPath();
	board.Print(cout);
	cout << gr.FilterPath() << endl;
	// 1406
}
