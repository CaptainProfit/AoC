//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/6
//second part find not 4 symbols but 14
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
using namespace std;
#define ull unsigned long long

string input;
int markerLen = 14;
//markerLen = 4 start-of-packet marker
//markerLen = 14 start-of-message marker


int readFileToSMt(){
	
	ifstream ifstr("y2022d6.input", ios::binary);
	ifstr>>input;	
	ifstr.close();
	return 0;
}

int main(void){
	readFileToSMt();
	int counters[26];
	int status = 0;
	for(int i = 0; i < 26; i++)
		counters[i] = 0;
	vector<char> inp(input.begin(), input.end());
	int i = 0;
	for(; i < inp.size(); i++){
		inp[i] -= 'a';
		counters[inp[i]]++;
		if(counters[inp[i]] > 1)
			status++;
		//первые markerLen символа пропускаю
		if(i >= markerLen){
			counters[inp[i - markerLen]]--;
			if(counters[inp[i - markerLen]] >= 1)
				status--;
			if(status == 0)
				break;
		}
	}
	//+1 потому что после него позиция
	cout<<"start-of-packet marker "<<i+1<<endl;
	//1622 wrong
	//1623 correct
	//1624 wrong
	return 0;
}
