//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/9
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <fstream>
#include <iostream> 
#include <sstream> 
#include <vector>
#include <string>
#define ull unsigned long long

using namespace std;

class cPoint{
	public:
	int x = 0;
	int y = 0;
	void mov(char dir, int len = 1){
		switch(dir){
			case 'U': y+=len;break;
			case 'D': y-=len;break;
			case 'R': x+=len;break;
			case 'L': x-=len;break;
		}
	}
	int dist(cPoint &dst){
		int dx = x - dst.x;
		int dy = y - dst.y;
		return max(max(dx,dy), max(-dx, -dy));
	}
	void pullTo(cPoint &dst){
				//horizontal
				if(y < dst.y)
					mov('U');
				else if(y > dst.y)
					mov('D');
				//vertical
				if(x < dst.x)
					mov('R');
				else if(x > dst.x)
					mov('L');
				// check tail

	}	
};
class cInfiniteGrid{
	//true - wasnt counted;
	//false - was counted;
	private:
	int ofsx = 0;
	int ofsy = 0;
	vector<vector<bool>> table;

	public:
	void init(vector<pair<char, int>> &moves){
		int ml = 0, mr = 0, mu =0, md =0;
		cPoint i;
		for(auto move:moves){
			i.mov(move.first, move.second);

			if(i.y > mu){
				mu = i.y;
			}
			if(i.y < md){
				md = i.y;
			}
			if(i.x > mr){
				mr = i.x;
			}
			if(i.x < ml){
				ml = i.x;
			}
					
		}
		ofsx = -ml;
		ofsy = -md;	
		int sx = mr - ml;
		int sy = mu - md;
		table.resize(sy);
		for(int i = 0; i < table.size(); i++){
			table[i].resize(sx, true);
		}
		table[ofsy][ofsx] = false;
	}
	bool check(cPoint &point){
		if(table[ofsy + point.y][ofsx + point.x]){
			table[ofsy + point.y][ofsx + point.x] = false;
			return true;
		}
		return false;
	}
	void print(){
		for(int i = 0; i<table.size(); i++){
			for(int j = 0; j<table[i].size(); j++)
				if(table[i][j])
					cout<<'.';
				else
					cout<<'#';
			cout<<endl;
		}
	}
}grid;
vector<pair<char, int>> moves;

int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		stringstream st(line);		
		moves.emplace_back();
		st>>moves.back().first>>moves.back().second;		
	}
	grid.init(moves);
	return 0;
}

ull solve(){
	ull counter = 1;//1ю позицию не обрабатываю - ну хоть сосчитаю
	cPoint head, tail;
	for(auto move:moves){
		for(int i = 0; i<move.second;i++){
			//move head
			head.mov(move.first);
			if(tail.dist(head) >1){
				tail.pullTo(head);
				//move tail
				if(grid.check(tail)){
					counter++;
				}
			}
		}
	}
	return counter;
}

int main(void){
	readFileToSMt();
	ull result = solve();
	//grid.print();
	cout<<"there are "<<result<<" places where tail was"<<endl;
	// 5599 too low 
	// 11460 total moves
	// 6367 correct
	return 0;
}
