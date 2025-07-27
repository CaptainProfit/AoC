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
		else 
			first = false;
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
		else 
			first = false;
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
struct std::hash<set<string>> {
    size_t operator()(const set<string>& s) const noexcept {
		size_t h = 0;
		for( const string& st : s) {
			h *= 31;
			h ^= hash<string>{}(st);
		}
        return h;
    }
};

unordered_map<set<string>, unordered_set<string>> GroupInds;
unordered_set<string> GetInd(const set<string>& blob) {
	if (GroupInds.count(blob)) {
		return GroupInds[blob];
	}
	unordered_set<string> result;
	for (auto& vert : blob)
		for(auto& ind : graph[vert])
			result.emplace(ind);
	for (auto& vert : blob)
		result.erase(vert);
	GroupInds[blob] = result;
	return result;
}

unsigned long Solve() {
	unordered_set<set<string>> blob_level;
	cout << "solving" << endl;
	cout << "graph size " << graph.size() << endl;
	for (auto& vertice : graph) {
		for (auto& ind : vertice.second) {
			blob_level.insert({vertice.first, ind});
		}
	}
	int level = 3;

	cout << "level " << level - 1 << " blobs " << blob_level.size() << endl;
	unordered_set<set<string>> new_blob_level;
	do {
		for (auto& blob : blob_level) {
			for (auto& ind : GetInd(blob)) {
				if (graph[ind].size() < level) {
					continue;
				}
				bool ind_all = true;
				for (auto& vertice : blob) {
					if (graph[vertice].count(ind) == 0) {
						ind_all = false;
						break;
					}
				}
				if (ind_all) {
					set<string> new_blob = blob;
					new_blob.insert(ind);
					new_blob_level.insert(new_blob);
				}
			}
		}
		
		if (new_blob_level.empty())
			break;
		blob_level = move(new_blob_level);
		cout << "level " << level << " blobs " << blob_level.size() << endl;
		level++;
	} while(true);
	cout << "answer: \"" << *blob_level.begin() << "\"" << endl;
	return blob_level.begin()->size();
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	cout << graph << endl;
	auto blobs = Solve();
	// ej,hm,ks,ms,ns,rb,rq,sc,so,un,vb,vd,wd correct
}
