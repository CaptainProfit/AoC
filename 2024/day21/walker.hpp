#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

template<typename Vertex>
class Graph {
	public:

	virtual vector<Vertex> GetVertices() = 0;
	virtual vector<pair<Vertex, Vertex>> GetEdges() = 0;
	virtual vector<Vertex> GetIndicies(Vertex v) = 0;
	virtual void VisitIndicies(Vertex v, std::function<void(void)>) = 0;
};

template<typename Vertex>
struct Walker {
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
		prevs_.clear();
		costs_.clear();
		set<Vertex> marks;
		multimap<size_t, Vertex> que;
		
		InitQue(que, start);
		
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
	map<Vertex, Vertex> prevs_;
protected:
	map<Vertex, size_t> costs_;
};
