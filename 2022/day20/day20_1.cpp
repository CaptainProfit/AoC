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
template class cTree<int>;

int readFileToSMt(cTree<int>& storage){
	string line;
	ifstream ifstr("testInsert.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		storage.push_back(stoi(line));
	}	
	return 0;
}

int solve(cTree<int>& storage){
	
	int d = storage.sizef();
	int z = 0;
	//формирую очередь указателей на объекты внутри стуктуры,
	// которые привязаны не к положению в структуре, а к положению
	// изначальному. таково условие задачи.
	vector<cTree<int>*> pointers(d);
	for(int i = 0; i < d; i++){
		pointers[i] = &(storage[i]);
	}

	for(int i = 0; i < d; i++){
		//вычислить где, что и куда надо переставить
		int oldIndex = pointers[i]->getIndex();
		int value = storage[oldIndex].getValue();
		int newIndex = (i + value + d) % d;
		//переставить значение на новое место.
		storage.remove(oldIndex);		
		storage.insert(newIndex, value);
	}

	//математическая формула из условия задачи
	int id1 = (z + 1000)%d;
	int id2 = (z + 2000)%d;
	int id3 = (z + 3000)%d;
	return storage[id1].getValue() + 
			storage[id2].getValue() +
		    storage[id3].getValue();
}

int main(void){
	cTree<int> storage;
	int result;
	readFileToSMt(storage);	
	result = solve(storage);
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
