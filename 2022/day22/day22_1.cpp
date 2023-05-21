//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#define ull unsigned long long

using namespace std;

struct cPoint{

	int x;
	int y;
	char c;
};

const cPoint directions[] = {
	{ 1, 0, '>'}, // право
	{ 0, 1, 'v'},  // вниз
	{-1, 0, '<'}, // лево
	{ 0,-1, '^'}, // вверх
};


class cSolve{
	vector<pair<char, int>> moves;
	vector<string> surface;
	string border;
	string name;
	int x, y;
	map<pair<int, int>, pair<int, int>> ports;
	void printMap(ostream& iout = cout){
		if(border == ""){
			border.resize(surface[0].size() + 2, '=');
		}

		iout << "map:" << endl;
		iout << border << endl;
		for(int i = 0; i < surface.size(); i++){
			iout << '|' << surface[i] << '|' << endl;
		}
		iout << border << endl;
	}

	void printMoves(ostream& iout = cout){
		iout << "moves:" << endl;
		for(int i = 0; i < moves.size(); i++){
			iout << moves[i].first;
			if(moves[i].first == 'M'){
				iout << moves[i].second;
			}
			iout << " ";
		}
		iout << endl;
	}

	int move(const pair<char, int>& move){
		static int dirIt = 0;
		switch(move.first){
			case 'A':
				return dirIt;				
			case 'L': 
				dirIt--;
				break;
			case 'R': 
				dirIt++;
				break;
			case 'M': 
				for(int i = 0; i < move.second; i++){
					int newx = x + directions[dirIt].x, newy = y + directions[dirIt].y;
					pair<int ,int> src(newy, newx);
					if(ports.find(src) != ports.end()){
						pair<int ,int> dest = ports[src];
						newy = dest.first;
						newx = dest.second;
					}
					//1) если зашли за край
					
					//3) если не наткнулись на камень - шаг вперёд.
					if(surface[newy][newx] != '#'){
						x = newx;
						y = newy;
						surface[y][x] = directions[dirIt].c;
					}
					else{
						break;
					}
				}	
				break;
			default: cout << "error: incorrect move" << endl;
		}
		dirIt += 4;
		dirIt %= 4;
		surface[y][x] = directions[dirIt].c;
		return 0;
	}

	int horizFind(int xr, char c, int i0 = 0){
		int i = i0;
		for(; i < surface.size(); i++){
			if(surface[i][xr] == c){
				return i;
			}
		}
		return i;
	}
	void calculatePorts(){
		//1) горизонтальные телепорты.
		int x1 = surface[0].length()/2, x2 = surface[0].length()/2;
		for(int yr = 0; yr < surface.size(); yr++){
			if(surface[yr][x1] == ' ' &&
				surface[yr][x1 + 1] != ' '){
				int x3 = surface[yr].find('.');
				int x4 = surface[yr].find('#');
				if(x3 == -1 && x4 == -1);
				else if(x3 == -1){
					x1 = x4;					
				}
				else if(x4){
					x1 = x3;
				}
				else {
					x1 = min(x3, x4);
				}
			}

			if(surface[yr][x2 - 1] != ' ' &&
				surface[yr][x2] == ' '){
				x2 = surface[yr].find(' ', x1);
			}

			pair<int, int> a1(yr, x1), a2(yr, x2);
			ports.emplace(a1, a2);
			ports.emplace(a2, a1);
		}
		
		//2) горизонтальные телепорты. 
		int y1 = surface.size()/2, y2 = surface.size()/2;
		for(int xr = 0; xr < surface[0].length(); xr++){
			if(surface[y1][xr] == ' ' &&
				surface[y1 + 1][xr] != ' '){
				int y3 = horizFind(xr, '.');
				int y4 = horizFind(xr, '#');
				int y5 = min(y3, y4);
				if(y5 == surface.size()){
					continue;
				}
				y1 = y5;
			}

			if(surface[y2 - 1][xr] != ' ' &&
				surface[y2][xr] == ' '){
					y2 = horizFind(xr, ' ', y1);
				}
			pair<int, int> a1(y1, xr), a2(y2, xr);
			ports.emplace(a1, a2);
			ports.emplace(a2, a1);
		}
	}
	public:
	//LRM
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		this->name = name.substr(0, name.find('.'));
		int maxWidth = 0;
		surface.push_back(" ");
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			if(line.find('R') == -1 && line.find('L') == -1){
				// карта
				if( line != ""){
					surface.push_back(" " + line + " ");
					if(maxWidth < line.length()){
						maxWidth = line.length();
					}
				}
			}
			else{
				// последовательность ходов
				for(int it = 0; it < line.length(); it++){
					switch(line[it]){
						case '\r': break;
						case 'L':
						case 'R':
							moves.push_back(pair<char, int>(line[it], -1));
							break;
						default:
							int it1 = line.find('L', it);
							int it2 = line.find('R', it);
							if(it1 == -1 && it2 == -1){
								string subs = line.substr(it);
								int steps = stoi(subs);
								moves.push_back(pair<char, int>('M', steps));
							}
							else{
								if(it1 == -1){
									string subs = line.substr(it, it2 - it);
									int steps = stoi(subs);
									moves.push_back(pair<char, int>('M', steps));
									it = it2 - 1;
									continue;
								}
								if(it2 == -1){
									string subs = line.substr(it, it1 - it);
									int steps = stoi(subs);
									moves.push_back(pair<char, int>('M', steps));
									it = it1 - 1;
									continue;
								}
								int it3 = min(it1, it2);
								string subs = line.substr(it, it3 - it);
								int steps = stoi(subs);
								moves.push_back(pair<char, int>('M', steps));
								it = it3 - 1;
							}
					}
				}
			}
		}
		ifstr.close();
		maxWidth ++;
		for(int i = 0; i < surface.size(); i++){
			surface[i].resize(maxWidth, ' ');
			int j = surface[i].find('\r');
			while(j != -1){
				surface[i][j] = ' ';
				j = surface[i].find('\r', j);
			}
		}
		calculatePorts();
		//printMap();
		//printMoves();
	}

	void solve(){
		//1) установить стартовую локацию.
		
		y = 0;
		x = surface[0].find('.');
		surface[y][x] = 's';
		//printMap();
		for(int i = 0; i < moves.size(); i++){
			ofstream ofstr(name + to_string(i) + ".output", ios::binary);
			move(moves[i]);
			printMap(ofstr);
			ofstr.close();
		}
	}

	int answer(){
		int row = y + 1;
		int column = x + 1;
		int facing = move(pair<char, int>('A', 0));
		return row*1000 + 4*column + facing;
	}
};

int main(void){
	int result;
	cSolve test("test.input");
	test.solve();
	result = test.answer();
	if( result != 6032){
		cout << "test failed(6032): " << result << endl;
		return -2;
	}
	cout << "test passed" << endl;
	cSolve cond("cond.input");
	cond.solve();
	result = cond.answer();
	if( result != 123046){
		cout << "cond failed(123046): " << result << endl;
		return -2;
	}
	cout << "cond passed" << endl;
	// 20562 too low
	// 96342 too low
	// 123046 correct
	return 0;
}
