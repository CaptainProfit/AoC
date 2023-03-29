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
	void mov(char dir){
		switch(dir){
			case 'U': y++;break;
			case 'D': y--;break;
			case 'R': x++;break;
			case 'L': x--;break;
		}
	}
	int from(cPoint *dst){
		int dx = x - dst->x;
		int dy = y - dst->y;
		if(dx < 0)
			dx = -dx;
		if(dy < 0)
			dy = -dy;
		return max(dx,dy);
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
		int x = 0, y = 0;
		for(auto move:moves){
			switch(move.first){
				case 'U': y+=move.second;break;
				case 'D': y-=move.second;break;
				case 'R': x+=move.second;break;
				case 'L': x-=move.second;break;
			}
			if(y > mu){
				mu = y;
			}
			if(y < md){
				md = y;
			}
			if(x > mr){
				mr = x;
			}
			if(x < ml){
				ml = x;
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
		table[0][0] = false;
	}
	bool check(cPoint &point){
		if(table[ofsy + point.y][ofsx + point.x]){
			table[ofsy + point.y][ofsx + point.x] = false;
			return true;
		}
		return false;
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
			if(tail.from(&head) < 2){
				continue;
			}
			//move tail
			//horizontal
			if(tail.y < head.y)
				tail.mov('U');
			else
				tail.mov('D');
			//vertical
			if(tail.x < head.x)
				tail.mov('R');
			else
				tail.mov('L');
			// check tail
			if(grid.check(tail)){
				counter++;
			}
		}
	}
	return counter;
}

int main(void){
	readFileToSMt();
	ull result = solve();
	//now come from tree
	cout<<"there are "<<result<<" places where tail was"<<endl;
	//5599 too low
	return 0;
}
