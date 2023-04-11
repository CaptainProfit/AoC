//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#define ull unsigned long long

using namespace std;


vector<int> arr;
int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		arr.push_back(stoi(line));
	}
	return 0;
}

tuple<int, int,int> solve(){
	int d = arr.size();
	int z = 0;
	//mix!
	return make_tuple(arr[(z + 1000)%d], arr[(z + 2000)%d], arr[(z + 3000)%d]);	
}

int main(void){
	int r1, r2, r3, result;

	readFileToSMt();	
	tie(r1, r2, r3)= solve();
	result = r1 + r2 + r3;
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
