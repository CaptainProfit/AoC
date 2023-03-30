//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/10
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
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

vector<int> answer;
void solve(){
	answer.resize(6);
	int x = 1;
	int ticks = 0;
	int tickTrig = 20;
	int trigCnt = 0;
	for(auto cmd:commands){
		switch(cmd.first){
			case 'n':
				ticks++;
				break;
			case 'a':
				ticks+=2;		
		}
		if(ticks >= tickTrig){
			answer[trigCnt] = tickTrig*x;
			tickTrig+=40;
			trigCnt++;
			if(trigCnt == 6)
				return;
		}
		x+=cmd.second;
	}
}

int main(void){
	readFileToSMt();
	solve();
	ull result = accumulate(answer.begin(), answer.end(), 0);
	cout<<"there are "<<result<<" "<<endl;
	//13840 too high
	//12560 correct
	return 0;
}
