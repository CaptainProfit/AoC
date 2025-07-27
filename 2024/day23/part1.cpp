#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <set>
using namespace std;

unordered_map<string, unordered_set<string>> graph; 
void ReadFile(string filename) {
	ifstream f(filename);
	string line;
	while(getline(f, line) && !f.eof() && !line.empty()) {
		string_view left(line), right(line);
		size_t delim = line.find("-");
		right.remove_prefix(delim + 1);
		left.remove_suffix(delim + 1);
		graph[string(right)].insert(string(left));
		graph[string(left)].insert(string(right));
	}
}

template<typename T> 
void Print(ostream& out, const vector<T>& v) {
	out << "[";
	bool first = true;
	for(auto& el : v) {
		if (!first) 
			out << ", ";
		else first = false;
		out << el;
	}
	out << "]";
}

template<typename U, typename V> 
void Print(ostream& out, const unordered_map<U, V>& m) {
	out << "{";
	bool first = true;
	for(auto& [key, val] : m) {
		if (!first) 
			out << ",\n";
		else first = false;
		out << "\"" << key << "\": " << "\"" << val << "\"";
	}
	out << "}";
}

template<typename U> 
void Print(ostream& out, const unordered_set<U>& m) {
	out << "{";
	bool first = true;
	for(auto& key : m) {
		if (!first) 
			out << ",";
		else first = false;
		out << key;
	}
	out << "}";
}

template<typename U> 
void Print(ostream& out, const set<U>& m) {
	out << "{";
	bool first = true;
	for(auto& key : m) {
		if (!first) 
			out << ",";
		else first = false;
		out << key;
	}
	out << "}";
}

template<typename T> 
ostream& operator<<(ostream& out, const vector<T>& v) {
	Print(out, v);
	return out;
}

template<typename U, typename V> 
ostream& operator<<(ostream& out, const unordered_map<U, V>& m) {
	Print(out, m);
	return out;
}
	
template<typename U> 
ostream& operator<<(ostream& out, const unordered_set<U>& m) {
	Print(out, m);
	return out;
}
	
template<typename U> 
ostream& operator<<(ostream& out, const set<U>& m) {
	Print(out, m);
	return out;
}
	
template<>
struct std::hash<set<string>>
{
    size_t operator()(const set<string>& s) const noexcept
    {
	size_t h = 0;
	for( const string& st : s) {
		h *= 31;
        	size_t h1 = hash<string>{}(st);
		h ^= h1;
	}
        return h;
    }
};

unordered_set<set<string>>  Walk() {
	unordered_set<set<string>> triplets;
	unordered_set<string> marked;
	unordered_set<string> que;
	que.insert(graph.begin()->first);
	while(!que.empty()) {
		string key = *que.begin();
		que.erase(que.begin());
		marked.insert(key);
		cout << "walking in " << key << endl;
		for (const string& next : graph[key]) {
			if (marked.count(next))
				continue;
			que.insert(next);				
		}
		for(auto it1 = graph[key].begin(); it1 != graph[key].end(); it1++) {
			auto it2 = it1;
			it2++;
			for(auto it2 = graph[key].begin(); it2 != graph[key].end(); it2++) {
				if (graph[*it1].count(*it2) == 0)
					continue;
				auto triple = {key, *it1, *it2};
				if (triple.size() < 3) 
					continue;
				triplets.insert(triple);
			}
		}
	}
	return triplets;
}

unsigned long Check(unordered_set<set<string>> triplets) {
	cout << "triplets" << endl;
	unsigned long marks = 0;
	for (auto triplet : triplets)  {
		bool mark = false;
		for (auto d : triplet) {
			if (d[0] == 't') {
				mark = true;
				break;
			}
		}
		if (!mark) {
			cout << "-";
		}
		else {
			cout << "+";
			marks++;
		}
		cout <<  triplet << endl;
	}
	cout << "total " << triplets.size() << endl;
	cout << "marks " << marks << endl;
	return marks;
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	cout << graph << endl;
	auto triplets = Walk();
	Check(triplets);
	// 1110 correct
}
