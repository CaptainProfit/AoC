//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <ext/rope> //заголовочный файл с rope
#define ull unsigned long long

using namespace std;
using namespace __gnu_cxx; //пространство имен, в котором находится класс rope и все, что с ним связано

int readFileToSMt(rope<int> &storage){
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

int solve(rope<int> &storage){
	
	int d = storage.size();
	int z = 0;
	//формирую очередь указателей на объекты внутри стуктуры,
	// которые привязаны не к положению в структуре, а к положению
	// изначальному. таково условие задачи.
	vector<int*> pointers(d);
	for(int i = 0; i < d; i++){
		pointers[i] = &(storage[i]);
	}

	for(int i = 0; i < d; i++){
		//вычислить где, что и куда надо переставить
		int oldIndex = pointers[i];
		int value = storage[oldIndex];
		int newIndex = (i + value + d) % d;
		//переставить значение на новое место.
		rope<int> newEl = storage.substr(oldIndex, 1);
		storage.erase(oldIndex, 1);
		storage.insert(storage.mutable_begin() + newIndex, newEl);
	}

	//математическая формула из условия задачи
	int id1 = (z + 1000)%d;
	int id2 = (z + 2000)%d;
	int id3 = (z + 3000)%d;
	return storage[id1] + 
			storage[id2] +
		    storage[id3];
}

int main(void){
	rope<int> storage;
	int result;
	readFileToSMt(storage);	
	result = solve(storage);
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
