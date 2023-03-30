//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/10
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
//part two change - 
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#define ull unsigned long long

using namespace std;


vector<pair<char, int>> commands;

int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		stringstream s(line);
		string cmd;
		s>>cmd;
		commands.emplace_back();
		commands.back().first = cmd[0];
		if(cmd.compare("noop") == 0){
			//noop
			commands.back().second = 0;
		}
		if(cmd.compare("addx") == 0){
			//addx
			s>>commands.back().second;
		}		
	}
	return 0;
}

vector<string> answer;
void checkRay(int ticks, int x){
 	if(abs(ticks%40 - x) <= 1)
		answer[ticks/40][ticks%40] = '#';
}

void solve(){
	answer.resize(6);
	for(int i =0; i < answer.size(); i++){
		answer[i] = string(40, '.');
	}
	int x = 1;
	int ticks = 0;
	int tickTrig = 20;
	int trigCnt = 0;
	for(auto cmd:commands){
		switch(cmd.first){
			case 'n':
				checkRay(ticks, x);
				ticks++;
				break;
			case 'a':
				checkRay(ticks, x);
				ticks++;
				checkRay(ticks, x);
				ticks++;
		}
		x+=cmd.second;
		// checkRay(ticks, x);
		if((ticks)>240)
			return;
	}
}

int main(void){
	readFileToSMt();
	solve();
	//ull result = accumulate(answer.begin(), answer.end(), 0);
	ofstream ifstr("2.output", ios::binary);
	for(int i = 0; i< answer.size(); i++)
		ifstr<<answer[i]<<endl;
	

	//cout<<"there are "<<result<<" "<<endl;
	return 0;
}
