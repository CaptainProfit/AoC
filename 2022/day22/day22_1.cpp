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
	vector<string> map;
	string border;
	string name;
	int x, y;
	void printMap(ostream& iout = cout){
		if(border == ""){
			border.resize(map[0].size() + 2, '=');
		}

		iout << "map:" << endl;
		iout << border << endl;
		for(int i = 0; i < map.size(); i++){
			iout << '|' << map[i] << '|' << endl;
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
					int newx = x, newy = y;
					//1) если зашли за край
					do{
						newy += directions[dirIt].y;
						newx += directions[dirIt].x;
						newy += map.size();
						newy %= map.size();
						newx += map[0].size();
						newx %= map[0].size();
						//2) если не в пределах карты. 
						// TODO: оптимизировать кешированием. завести мапу с краями и переходами.
					}while(map[newy][newx] == ' ' 
						|| map[newy][newx] == '\r' 
						|| map[newy][newx] == '\n');

					//3) если не наткнулись на камень - шаг вперёд.
					if(map[newy][newx] != '#'){
						x = newx;
						y = newy;
						map[y][x] = directions[dirIt].c;
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
		map[y][x] = directions[dirIt].c;
		return 0;
	}

	public:
	//LRM
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		this->name = name.substr(0, name.find('.'));
		int maxWidth = 0;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			if(line.find('R') == -1 && line.find('L') == -1){
				// карта
				if( line != ""){
					map.push_back(line);
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
		for(int i = 0; i < map.size(); i++){
			map[i].resize(maxWidth, ' ');
			int j = map[i].find('\r');
			while(j != -1){
				map[i][j] = ' ';
				j = map[i].find('\r', j);
			}
		}
		//printMap();
		//printMoves();
	}

	void solve(){
		//1) установить стартовую локацию.
		
		y = 0;
		x = map[0].find('.');
		map[y][x] = 's';
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
	cout << "result "<< result << endl;
	// 20562 too low
	// 96342 too low
	// 123046 correct
	return 0;
}
