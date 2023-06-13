//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <chrono> 
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "log_duration.h"
#include <cassert>
#define ull unsigned long long
#define haveNotPath (INT_MAX)

using namespace std;
using namespace chrono;

const set<char> spaces = {
	' ',
	'\t',
	'\n',
	'\r',
	':',
	';',
	'.',
	',',
	'=',
};

class cNode{
	string name;
	int id = -1;
	int flowrate = -1;
	vector<int> adjacentIds;
	vector<string> adjacentNames;
	vector<string_view> parseToWords(const string& str){
		vector<string_view> result;		
		result.clear();
		bool isWord = false;
		int wordStart = 0;
		int i = 0;
		for(; i < str.length(); i++){
			if(isWord){
				if(spaces.find(str[i]) != spaces.end()){
					isWord = false;
					string_view newShit(str.c_str() + wordStart, i - wordStart);
					wordStart = i;
					result.push_back(newShit);
				}
			}
			else{
				if(spaces.find(str[i]) == spaces.end()){
					wordStart = i;
					isWord = true;
				}
			}
		}
		if(isWord){
			string_view newShit(str.c_str() + wordStart, i - wordStart);
			result.push_back(newShit);
		}
		return result;
	}

	public:
	cNode(void){
		int id = -1;
		int flowrate = -1;
	};
	cNode(string& str){
		int nameOffset = 1;
		int flowrateOffset = 5;
		int adjacencyListOffset = 10;

		vector<string_view> words(parseToWords(str));
		name = string(words[nameOffset]);
		id = -1;
		flowrate = stoi(string(words[flowrateOffset]));
		adjacentNames.reserve(words.size() - adjacencyListOffset);
		for(int i = adjacencyListOffset; i < words.size(); i++){
			adjacentNames.push_back(string(words[i]));
		}
	}

	void notate(map<string, int>& name2id){
		id = name2id[name];
		adjacentIds.reserve(adjacentNames.size());
		for(auto name:adjacentNames){
			adjacentIds.push_back(name2id[name]);
		}
	}

	void print(void){
		cout << "\tnode: " << name << "(" << id << ")" << endl;
		cout << "\tflowrate: " << flowrate << endl;
		cout << "\tNeighbours:" << endl;
		for(int i =0; i < adjacentIds.size(); i++){
			cout << "\t\t" << adjacentNames[i] << "(" << adjacentIds[i] << ")" <<endl;
		}
	}

	const string& getName(void){ return name; }
	const vector<int>& getNeighbours(void){ return adjacentIds; }
	int getId(void){ return id; }
	int getFlowrate(void){ return flowrate; }
};

class cGraph{	
	map<int, cNode> graph;
	vector<vector<int>> distances_;
	int timeLimit;
	public:
	map<string, int> name2id;
	vector<int> bestOrder;
	long long bestResult;
	void addNode(string& str){
		graph.emplace(graph.size(), str);
	}

	void notate(void){
		for(auto [key, value]: graph){
			name2id.emplace(value.getName(), key);
		}
		for(auto it = graph.begin(); it != graph.end(); it++){
			it->second.notate(name2id);
		}
	}

	void makeDistances(void){
		//thanks to ivv19041994
		size_t vertexCount = graph.size();
		distances_.resize(vertexCount, vector<int>(vertexCount, haveNotPath));
		for (size_t i = 0; i < vertexCount; ++i){
			distances_[i][i] = 0;
			for(auto j:graph[i].getNeighbours()){
				distances_[i][j] = 1;
			}
		}

		for (size_t k = 0; k < vertexCount; ++k){
			for (size_t i = 0; i < vertexCount; ++i){
				for (size_t j = 0; j < vertexCount; ++j){
					if (
						distances_[i][k] == haveNotPath ||
						distances_[k][j] == haveNotPath)
					{
						continue;
					}
					auto throwK = distances_[i][k] + distances_[k][j];
					distances_[i][j] = min(distances_[i][j], throwK);
				}
			}
		}
	}

	void print(void){
		cout << "distance matrix:" << endl;
		for(auto ity: distances_){
			for(auto itx:ity){
				cout << itx << "\t";
			}
			cout << endl;
		}
		cout << "----------" << endl;
		cout << "graph:" << endl;
		for(auto [k,v] :graph){
			cout << "\tid: " << k <<endl;
			v.print();
		}
		cout << "--------------------" << endl;
	}
	void printOrder(void){
		cout << "order: {";
		for(auto id:bestOrder){
			cout << graph[id].getName() <<", ";
		}
		cout << "}" << endl;
	}

	void setTimelimit(int time){ timeLimit = time; }
	int getTimelimit(void){ return timeLimit; }
	int getDistance(int i, int j){ return distances_[i][j]; }
	int size(void){ return graph.size(); }
	auto begin(void){ return graph.begin(); }
	auto end(void){ return graph.end(); }
	cNode& operator[](int id){ return graph[id];}
};

static vector<int> currentOrder;
struct cState{
	long long flow = 0;
	long long releasedPressure = 0;
	int time = 0;
	int currentNode;
	vector<bool> openValves;

	cState(cGraph& graph){
		currentOrder.clear();
		currentNode = graph.name2id["AA"];
		openValves.resize(graph.size(), false);
		for(auto [key, value]: graph){
			if(value.getFlowrate() == 0){
				openValves[key] = true;
			}
		}
	}

	void expand(cGraph* enclose){
		for(int nextNode=0; nextNode< openValves.size(); nextNode++){
			if(openValves[nextNode]){
				continue;
			}
			int dist = enclose->getDistance(currentNode, nextNode);
			if(time + dist >= enclose->getTimelimit() - 1){
				continue;
			}
			cState newState(*this);
			newState.currentNode = nextNode;
			newState.time += dist + 1;
			newState.releasedPressure += newState.flow*(dist + 1);
			newState.flow += (*enclose)[nextNode].getFlowrate();
			newState.openValves[nextNode] = true;
			currentOrder.push_back(nextNode);
			newState.expand(enclose);
			// if(enclose->bestResult < newState.releasedPressure){
			// 	enclose->bestResult = newState.releasedPressure;
			// 	enclose->bestOrder = currentOrder;
			// }
			currentOrder.pop_back();
		}
		int dist = enclose->getTimelimit() - time;
		releasedPressure += flow*dist;
		if(enclose->bestResult < releasedPressure){
			enclose->bestResult = releasedPressure;
			enclose->bestOrder = currentOrder;
		}
	}
};

class cSolve{
	string name;
	time_point<system_clock> tStart;
	time_point<system_clock> tEnd;
	cGraph graph;

	public:
	cSolve(string& filename){
		LOG_LIFE_TIME_DURATION("0) reading file");
		name = filename;
		tStart = chrono::system_clock::now();
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			if(line.compare("\r") == 0 || line.compare("") == 0){
				break;
			}
			graph.addNode(line);
			// do something
		}
		ifstr.close();
		graph.notate();
		graph.makeDistances();
		graph.print();
	}

	void solve(void){
		graph.setTimelimit(30);
		graph.bestResult = 0;
		cState state(graph);
		state.expand(&graph);
		tEnd = chrono::system_clock::now();
	}

	void printUsedTime(void){
		system_clock::duration timeInterval = tEnd - tStart;
		auto dta= microseconds(duration_cast<microseconds>(timeInterval));		
		long long dt = dta.count();
		cout << "time used: ";
		if(dt < 1000){
			cout << dt << "us" << endl;
			return;
		}

		dt /= 1000;
		if(dt < 1000){
			cout << dt << "ms" << endl;
			return;
		}
		
		dt /= 1000;
		if(dt < 60){
			cout << dt << "s" << endl;
			return;
		}

		dt /= 60;
			cout << dt << "min" << endl;
			return;
	}

	void printUsedMemory(void){
		cout << "i dont know how much memory was used" << endl;
	}

	int getResult(void){
		graph.printOrder();
		cout << "----------------" << endl;
		return graph.bestResult;
	}
};

int main(void){
	long long result;
	string names[] = {"test", "cond"};
	int answers[] = {1651, 2124}; 
	
	for(int i = 0; i < 2; i++){
		cSolve test1(names[i]);
		test1.solve();
		result = test1.getResult();
		test1.printUsedTime();
		test1.printUsedMemory();
		if(result != answers[i]){
			cout << names[i] + " failed: " << result << endl;
			return -2;
		}
		cout << names[i] + " passed" << endl;
		cout << "------------------------------------------------------------" << endl;
	}
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
