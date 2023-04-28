//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
// https://en.cppreference.com/w/cpp/string/multibyte/mbstowcs
// https://en.cppreference.com/w/cpp/string/basic_string/size
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <array>
#define ull unsigned long long
#define screenHeight 20
#define screenWidth 20
using namespace std;

using my_string=u32string;

//'-' '+' 'L' '|' '#' ;
//█ - 219
class cFigures{
	public:
	int index;
	vector<vector<pair<int, int>>> sprites;
	vector<int> heights;
	//просто запихивает фигурки
	cFigures(){
		index = 4;
		sprites.resize(5);
		heights.resize(5);

		// ████
		heights[0] = 1;
		sprites[0].push_back({0, 0});
		sprites[0].push_back({0, 1});
		sprites[0].push_back({0, 2});
		sprites[0].push_back({0, 3});
		
		//  █ 
		// ███
		//  █ 
		heights[1] = 3;
		sprites[1].push_back({0, 1});
		sprites[1].push_back({1, 0});
		sprites[1].push_back({1, 1});
		sprites[1].push_back({1, 2});
		sprites[1].push_back({2, 1});		
		
		//   █
		//   █
		// ███
		heights[2] = 3;
		sprites[2].push_back({0, 0});
		sprites[2].push_back({0, 1});
		sprites[2].push_back({0, 2});
		sprites[2].push_back({1, 2});
		sprites[2].push_back({2, 2});
				
		// █ 
		// █
		// █ 
		// █ 
		heights[3] = 4;
		sprites[3].push_back({0, 0});
		sprites[3].push_back({1, 0});
		sprites[3].push_back({2, 0});
		sprites[3].push_back({3, 0});		
		
		// ██
		// ██
		heights[4] = 2;
		sprites[4].push_back({0, 0});
		sprites[4].push_back({0, 1});
		sprites[4].push_back({1, 0});
		sprites[4].push_back({1, 1});
		
	}
	
	void chooseNext(){
		index++;
		index %= 5;
	}

	vector<pair<int, int>>& getSprite(){
		return sprites[index];
	}

	int getHeight(){
		return heights[index];
	}
}figures;

class cSolve{
	vector<char> directions;
	int dirIt = 0;
	
	const int restCounterFinish = 15; //2023	
	
	int height = 0;
	vector<vector<char>> map;
	pair<int, int> pos;	
	vector<pair<int, int>>* currentSprite;
	
	public:
	//1 init from file
	//cSolve():cSolve(){}
	cSolve(string filename){	
		string line;
		ifstream ifstr(filename, ios::binary);
		do{
			getline(ifstr, line);
			copy(line.begin(), line.end(), back_inserter(directions));
		}while(!ifstr.eof());
		ifstr.close();
	}

	void paintSpriteAtPosAs(char c){
		for(int i = 0; i < (*currentSprite).size(); i++){
			int dy = (*currentSprite)[i].first;
			int dx = (*currentSprite)[i].second;
			map[pos.first + dy][pos.second + dx] = c;
		}
	}
	// вернет false если движение продолжается
	bool moveTo(pair<int, int> dir){
		//check is move will succeed
		for(int i = 0; i < (*currentSprite).size(); i++){
			int dy = (*currentSprite)[i].first + dir.first;
			int dx = (*currentSprite)[i].second + dir.second;

			// если границы колодца не мешают
			if(pos.second + dx < 0)
				return false;
			if(pos.second + dx > 6)
				return false;			
			if(pos.first + dy < 0)
				return false;

			//если не уперся в кучу
			if( map[pos.first + dy][pos.second + dx] == '#'){
				return false;
			}
		}

		//remove old sprite
		paintSpriteAtPosAs(' ');		
		//new pos
		pos.first += dir.first;
		pos.second += dir.second;
		//print sprite
		paintSpriteAtPosAs('@');		
		return true;
	}

	void recalcHeight(){
		for(int i = height + 1; i < map.size(); i++){
			bool flag = false;
			for(int j = 0; j < 7; j++){
				if(map[i][j] == '#'){
					flag = true;
					break;
				}	
			}
			if(flag){
				height = i;
			}
			else{
				break;
			}			
		}
	}

	int solve(){
		//char emptyline[] = "       ";
		for(int restCounter = 0; restCounter < restCounterFinish; restCounter++){
			// bottom edge is three units above the highest rock
			figures.chooseNext();
			currentSprite = &(figures.getSprite());
			while(map.size() < height + 3 + figures.getHeight() + 2){
				map.push_back({' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'});
				//map.push_back(emptyline);
				//map[map.size() - 1];  
			}
			// left edge is two units away from the left wall
			pos.second = 2; 
			pos.first = map.size() - figures.getHeight(); 
			
			paintSpriteAtPosAs('@');
			do{
				if(directions[dirIt] == '>'){
					moveTo({0, 1});
				}
				if(directions[dirIt] == '<'){
					moveTo({0, -1});
				}
				dirIt++;
				dirIt %= directions.size();
				// print();
			}while(moveTo({-1, 0}));
			paintSpriteAtPosAs('#');
			print();
			recalcHeight();
		}		
		print(true);
		currentSprite = nullptr;		
		return height;
	}

	//выводит состояние колодца в данный момент времени.
	array<string, screenHeight> screen;
	void print(bool isFlush = false){
		static int j = 0;
		static int k = 0;
		static const int kMax = (100/screenWidth); // states per line
		// system("cls");
		if(!isFlush){
			screen[0] += "step " + to_string(j);
			
			if(height < screenHeight - 1){
				screen[screenHeight - 1] += "|-------|"; //╚ ═╝║
			}
			else
				screen[screenHeight - 1] += "|~~~~~~~|";
			int iter = map.size() - 1;
			if(iter < screenHeight - 2){
				iter = screenHeight - 2;
			}
			else{
				iter += 2;
			}
			for(int i = 1; i < screenHeight - 1; i++){
				string data(7, ' ');
				if(iter < map.size()){
					data = string(map[iter].data());
				}
				screen[i] +="|" + data + "|";
				//метка высоты
				if( i%10 == 9){
					screen[i] += to_string(i + 1);
				}
				//этот указатель опускаем глубже
				iter--;
			}
		}
		else{
			// заставить вывести остаток.
			if(k != 0){
				k = kMax;
			}
		}
		k++;
		if(k >= kMax){
			for(int i = 0; i < screenHeight; i++){
				cout<<screen[i]<<endl;
				screen[i] = "";
			}
			k = 0;
		}
		else{
			for(int i = 0; i < screenHeight; i++){
				string additional((k*20 - screen[i].size()), ' ');
				// for(int j = 0; j < screen[i].size(); j++){
				// 	cout<<static_cast<unsigned int>(static_cast<unsigned char>(screen[i][j]))<<" ";
				// }
				// cout<<endl;
				screen[i] += additional;
			}
		}
	}
};

int main(void){
	int result;

	cSolve test("test.input");
	result = test.solve();
	if(result != 3068){
		cout<<"test.input failed with "<<result<<" in result(must be 3068)"<<endl;	
		return -1;
	}
	cout<<"test.input passed: "<<result<<"(3068)"<<endl;	
	return -2;

	cSolve cond("cond.input");
	result = cond.solve();

	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
