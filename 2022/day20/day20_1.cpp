//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include "orderedTree.hpp"
#define ull unsigned long long

using namespace std;
//template class cContainer<int>;

class cSolve{
	cContainer storage;
	public:
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			storage.push_back(stoi(line));
		}
		ifstr.close();
	}

	void solve(){
		int d = storage.sizef();
		storage.print();
		for(int i = 0; i < d; i++){
			
			storage.move(i);
			storage.print();
		}
	}

	int mix(){
		//математическая формула из условия задачи
		int zid = storage.find(0);
		int d = storage.sizef();
		int id1 = (zid + 1000)%d;
		int id2 = (zid + 2000)%d;
		int id3 = (zid + 3000)%d;
		return storage[id1].getValue() + 
				storage[id2].getValue() +
				storage[id3].getValue();
	}
}

int main(void){	
	int result;
	cSolve test("test.input");
	test.solve();
	result = test.mix();
	
	if(result != 3){
		cout << "test failed (3):" << result <<endl;
		return -2;
	}
	cout<<"test passed "<< result << endl;
	return 0;
}
