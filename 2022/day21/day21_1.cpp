//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
#define ull unsigned long long

using namespace std;


int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
	}
	return 0;
}

void solve(){
}

int main(void){
	int result;
	readFileToSMt();	
	solve();
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
