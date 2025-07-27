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

class BinaryGenerator {
	set<string>& blob_raw_;
	vector<bool> mask;
	void ProgressMask() {
		for (int i = 0; i < mask.size(); i++) {
			if (mask[i]) {
				mask[i] = false;
			}
			else {
				mask[i] = true;
				break;
			}
		}
	}
public:
	BinaryGenerator(set<string>& blob_raw) :
		blob_raw_(blob_raw), 
		mask(blob_raw.size(), 0) {
	
	}
	~BinaryGenerator() = default;
	bool Finished() {
		for (int i = 0; i < mask.size(); i++) {
			if (!mask[i])
				return false;
		}
		return true;
	}
	set<string> Next() {
		ProgressMask();	
		set<string> new_iter;
		int i = 0;
		for (auto& key : blob_raw_) {
			if (mask[i++])
				new_iter.insert(key);
		}
		return new_iter;
	}
};

unordered_map<set<string>, unsigned long>  Solve() {
	unordered_map<set<string>, unsigned long> blobs;
	cout << "solving" << endl;
	cout << "graph size " << graph.size() << endl;
	for (auto vertice : graph) {
		set<string> blob_raw = {vertice.first};
		for (auto ind : vertice.second) {
			blob_raw.insert(ind);
		}
		cout << "blob: " << blob_raw << endl;
		BinaryGenerator gen(blob_raw);
		for (auto blob = gen.Next(); !gen.Finished(); blob = gen.Next()) {
			blobs[blob]++;
			cout << "\t" << blob << endl;
		}
		cout << "blobs size: " << blobs.size() << endl;
	}
	return blobs;
}

unsigned long Check(unordered_map<set<string>, unsigned long> blobs) {
	cout << "searching maximum blob" << endl;
	unsigned long max_blob = 0;
	const set <string>* ptr = nullptr;
	for (auto& blob : blobs)  {
		if (blob.first.size() != blob.second) {
			continue;
		}

		if (max_blob < blob.second) {
			ptr = &blob.first;
			max_blob = blob.second;
		}
	}
	cout << "password: \"" << *ptr<< "\"" << endl;
	return 0;
}

int main(int argc, char** argv) {
	if (argc < 2) {
        	return -1;
	}
	ReadFile(argv[1]);
	cout << graph << endl;
	auto blobs = Solve();
	Check(blobs);
}
