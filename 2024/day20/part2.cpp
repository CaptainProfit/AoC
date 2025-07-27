#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>

#include "timer.hpp"
#include "board.hpp"
using namespace std;
#define CHEAT_LENGTH 20

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

template<typename Vertex>
class Graph {
	public:

	virtual vector<Vertex> GetVertices() = 0;
	virtual vector<pair<Vertex, Vertex>> GetEdges() = 0;
	virtual vector<Vertex> GetIndicies(Vertex v) = 0;
	virtual void VisitIndicies(Vertex v, std::function<void(void)>) = 0;
};

template<typename Graph, typename Vertex>
struct Walker {
	const Graph& graph_;
	Walker(const Graph& graph)
		: graph_(graph) {
	}
	~Walker() = default;

	vector<Vertex> FindPath(const Vertex& start, const Vertex& finish) {
		TraceGraph(start);
		vector<Vertex> result_reversed;
		for (Vertex iter = finish; prevs_.count(iter) > 0 || iter == start; iter = prevs_.at(iter)) {
			result_reversed.push_back(iter);
			if (iter == start) {
				break;
			}
		}
		vector<Vertex> result;
		result.reserve(result_reversed.size());
		for (auto& pos : result_reversed) {
			result.push_back(pos);
		}
		return result;

	}

	set<Vertex> FindReachableSpace (Vertex& start) {
		TraceGraph(start);
		set<Vertex> result;
		for (auto& [k, _] : costs_) {
			result.insert(k);
		}
		return result;
	}

protected:
	virtual void VisitIndicies(const Vertex& current, std::function<void(const Vertex& next, const size_t cost)> func) = 0;
	virtual void InitQue(multimap<size_t, Vertex>& que, Vertex start ) {
		que.insert({0, start});
		costs_.insert({start, 0});
	}

	void TraceGraph (const Vertex& start) {
		/*if (!costs_.empty() && start_ == start) {
			return;
		}*/
		start_ = start;
		set<Vertex> marks;
		multimap<size_t, Vertex> que;
		
		InitQue(que, start_);
		que.insert({0, start});
		costs_.insert({start, 0});
		
		vector<Vertex> path;
		while(!que.empty()) {
			Vertex current = que.begin()->second;
			que.erase(que.begin());
			if (marks.count(current)) {
				continue;
			}
			marks.insert(current);
			VisitIndicies(current, [&marks, &que, this, current](const Vertex& next, const size_t cost){
				if (marks.count(next) != 0) {
					return;
				}
				
				if (costs_.count(next) == 0 || costs_[next] > cost) {
					prevs_[next] = current;
					costs_[next] = cost;
					que.insert({cost, next});
				}
			});
		}
		
	}
private:
	Vertex start_;
	map<Vertex, Vertex> prevs_;
protected:
	map<Vertex, size_t> costs_;
};

class BoardWalker : public Walker<Board, Point> {
public:
	BoardWalker(const Board &board) : Walker<Board, Point>(board) {
	}
protected:
	void VisitIndicies(const Point& current, std::function<void(const Point& next, const size_t cost)> func) override {
		size_t cost = costs_.at(current);
		for (int i = 0; i < 4; i++) {
			Point next(current);
			next += dirs[i];
			if (graph_.OutOf(next)) {
				continue;
			}
			if (graph_.Get(next) == '#') {
				continue;
			}
			func(next, cost + 1);
		}
	}
};

class CheatWalker : public Walker<Board, Point> {
public:
	CheatWalker(const Board &board) : Walker<Board, Point>(board) {
	}
	map<Point, size_t>& GetCheats(const Point& current) {
		cheat_outputs.clear();
		TraceGraph(current);
		return cheat_outputs;
	}
protected:
/*	void InitQue(multimap<size_t, Point>& que, Point start) override {
		for (int i = 0; i < 4; i++) {
			Point near = start + dirs[i];
			que.insert({1, near});
			costs_.insert({near, 1});
		}
	}*/
	void VisitIndicies(const Point& current, std::function<void(const Point& next, const size_t cost)> func) override {
		size_t cost = costs_.at(current);
		if (cost >= CHEAT_LENGTH) {
			return;
		}
		for (int i = 0; i < 4; i++) {
			Point next = current + dirs[i];
			if (graph_.OutOf(next)) {
				continue;
			}
			if (graph_.Get(next) != '#') {
				if (cheat_outputs.count(next) == 0 || cheat_outputs[next] > static_cast<size_t>(cost))
				cheat_outputs[next] = static_cast<size_t>(cost);
			}
			func(next, cost + 1);
		}
	}
	map<Point, size_t> cheat_outputs;
};

void PrintPath(Board& board, const vector<Point>& path) {
	for (const Point& pos :path) {
		board.Set(pos, 'O');
	}
}

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
	cout << "}" << endl;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return -1;
	}
	ReadFile(argv[1]);
	vector<vector<int>> image(board.Height(), vector<int>(board.Width(), -1));
	BoardWalker bw(board);
	vector<Point> path = bw.FindPath(board.GetStart(), board.GetFinish());
	for (int i = 0; Point p :path) {
		image[p.y][p.x] = i++;
	}
	long long result = 0;
	map<long long, long long> scores;
	board.Print(cout);
	for (int i = 0; Point A :path) {
		CheatWalker cw(board);
		auto cheats = cw.GetCheats(A);
		for (auto& [B, cost]: cheats) {
			long long benefit = image[B.y][B.x] - image[A.y][A.x] - cost - 1;
			if (benefit <= 0) {
				continue;
			}
			//cout << A << "->" << B << " saves " << benefit << endl;
			//Point C = B + A;
			//C.y /=2;
			//C.x /=2;
			//board.Set(C, 'X');
			scores[benefit]++;
			if (benefit >= 100) {
				result++;
			}
		}
	}
	PrintMap(scores);
	cout << result << endl;
	// 999107 too low
}
