//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <orderedTree.hpp>
#define ull unsigned long long

using namespace std;

int readFileToSMt(cTree<int>& root){
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		root.push_back(stoi(line));		
	}	
	return 0;
}

int solve(cTree<int>& root){
	
	int d = root.sizef();
	int z = 0;
	//формирую очередь указателей на объекты внутри стуктуры,
	// которые привязаны не к положению в структуре, а к положению
	// изначальному. таково условие задачи.
	vector<cTree<int>*> pointers(d);
	for(int i = 0; i < d; i++){
		pointers[i] = &root[i];
	}

	for(int i = 0; i < d; i++){
		//вычислить где, что и куда надо переставить
		int oldIndex = pointers[i]->getIndex();
		int value = root[oldIndex].getValue();
		int newIndex = (i + value + d) % d;
		//переставить значение на новое место.
		root.remove(oldIndex);		
		root.insert(newIndex, value);
	}

	//математическая формула из условия задачи
	int id1 = (z + 1000)%d;
	int id2 = (z + 2000)%d;
	int id3 = (z + 3000)%d;
	return root[id1].getValue() + 
			root[id2].getValue() +
		    root[id3].getValue();
}

int main(void){
	cTree<int> root;
	int result;
	readFileToSMt(root);	
	result = solve(root);
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
