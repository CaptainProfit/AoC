//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <map>
#define ull unsigned long long

using namespace std;

class cGraph{
	class cNode{
		public:
		bool mark;
		//list<cNode*> neighbours;
		list<string> neighboursLabels;
		int value;
	};
	map<string, cNode> nodes;
	
	public:
	// инициализаци€ графа из файла
	cGraph(){
		
	}

	// инициализаци€ графа из файла
	cGraph(string& filename){
		string line;
		ifstream ifstr(filename, ios::binary);
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			// им€ - слово после вальвы, 
			// мощность клапана - число между '=' и ';' 
			int t1 = line.find(' ', 0) + 1;
			int t2 = line.find(' ', t1) + 1;
			int t3 = line.find('=', t2) + 1;
			int t4 = line.find(';', t3) + 1;
			string name = line.substr(t1, t2 - t1);
			int value = stoi(line.substr(t3, t4 - t3));
			nodes.emplace(name, cNode(value));
		}
		ifstr.close();
		
		// прохожусь ещЄ раз, соседей 
		ifstr.open(filename, ios::binary);
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			// им€ - слово после вальвы, 
			// мощность клапана - число между '=' и ';' 
			int t1 = line.find(' ', 0) + 1;
			int t2 = line.find(' ', t1) + 1;
			string name = line.substr(t1, t2 - t1);

			
		}
		ifstr.close();
	}

	// проверка, что граф ненаправленый
	bool isDirectional(){
		for(auto n1: nodes){
			for(auto n2:n1.neighbours){
				if(!n2.neighbours.contains(n1)){
					return true;
				}
			}
		}
		return false;
	}
	
	
	
	// вывод получившегос€ пути из стека.
	void printPath(){
		for(auto iter = pathStack.begin(); iter != pathStack.end(); iter++){
			cout<<iter->name;
			if(iter->next() != pathStack.end()){
				cout<<"-";
			}
			else{
				cout<<endl;
			}
		}
	}
	
	// обход всевозможных путей из вершины с индексом id
	long backTrack(int id ){
		cNode* currentNode = &nodes[id];
		list<cNode*>::iterator iter;
		long acc = 0;
		long flow = 0;
		long maxAcc = 0;
		int depth = 1;
		// pathStack.push_back(tuple(currentNode, currentNode->neighbours.begin(), acc, flow);
		{
			current->mark = true;
			
			for(;iter != current.end(); iter++){
				if(!iter->mark){
					next = iter;
					break;
				}
			}
			
			if(iter == current.end()){
				if(depth >= nodes.size()){
					// это была последн€€ вершина.
					if(maxAcc < acc){
						maxAcc = acc;
					}
					cout<<"reach end"<<endl;
					cout<<"flow: "<<flow<<endl;
					cout<<"total: "<<acc<<endl;
					printPath();				
				}
				// перебрал всех соседей вершины
				// возвращаюсь во времени
				pathStack.pop_back(current, iter, acc, flow);
				current->mark = false;
				depth--;
			}
			else{
				// выбрал соседа вершины.
				stack_push_back(current, iter, acc, flow);
				
				acc += flow;
				flow += iter->value;
				current = &*iter;
				iter = current->neighbours.begin();
				depth++;
				
			}			
		}
	}
};

int main(void){
	int result;
	cGraph graph("test.input");
	readFileToSMt();	
	solve();
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
