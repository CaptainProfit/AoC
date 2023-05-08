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
#include <algorithm>
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

	vector<pair<int, int>>& getSprite(){return sprites[index];}
	int getIndex(){return index;}
	int getHeight(){return heights[index];}

}figures;

class cSolve{
	vector<char> directions;
	int dirIt = 0;
	ull moveCounter = 0;
	const int restCounterFinish = 2023; //2023	
	const string nether = "A=======B";
	const string etc = "|~~~~~~~|";
	const string chasteLine = "|       |";
	int height = 0;
	int restCounter = 1;
	vector<string> map;
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
		directions.pop_back(); //удалить нафиг перенос строки
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

			// если границы колодца не мешают и
			// если не уперся в кучу уже навалившихся
			switch(map[pos.first + dy][pos.second + dx]){
				case ' ':
				case '@': break;
				default: return false;
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
			for(int j = 1; j <= 7; j++){
				if(map[i][j] != ' '){
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
		
		map.push_back(nether);
		for(; restCounter < restCounterFinish; restCounter++){
			// bottom edge is three units above the highest rock
			figures.chooseNext();
			currentSprite = &(figures.getSprite());
			while(map.size() < height + 4 + figures.getHeight()){
				map.push_back(chasteLine);
			}
			// left edge is two units away from the left wall
			pos.second = 3; 
			pos.first = height + 4;// - figures.getHeight(); 
			
			paintSpriteAtPosAs('@');
			do{
				// print();
				if(directions[dirIt] == '>'){
					moveTo({0, 1});
				}
				else if(directions[dirIt] == '<'){
					moveTo({0, -1});
				} else {
					cout << " error: directions not correct" <<endl;
				}

				dirIt++;
				dirIt %= directions.size();
				moveCounter+=2;
				// print(); 
			}while(moveTo({-1, 0}));
			paintSpriteAtPosAs('0' + restCounter%10);
			// print();
			recalcHeight();
		}		
		// print(true);
		currentSprite = nullptr;
		cout << " moves done: " << moveCounter << endl;
		return height;
	}

	//выводит состояние колодца в данный момент времени.
	array<string, screenHeight> screen;
	void print(bool isFlush = false){
		static int k = 0;
		static const int kMax = (100/screenWidth); // states per line
		// system("cls");
		if(!isFlush){
			screen[screenHeight - 1] += "step " + to_string(restCounter);
			
			if(map.size() >= screenHeight){
				screen[0] += etc;
			}
			else{
				screen[0] += nether;
			}
			int ofset = map.size() - screenHeight;
			if(ofset < 0){
				ofset = 0;
			}
			for(int i = 1; i < screenHeight - 1; i++){
				if(i < map.size())
					screen[i] += map[ofset + i];
				else screen[i] += chasteLine;
				//метка высоты
				if( (ofset + i)%10 == 0){
					screen[i] += to_string(ofset + i);
				}
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
			for(int i = screenHeight - 1; i >=0; i--){
				cout<<screen[i]<<endl;
				screen[i] = "";
			}
			cout<<endl;
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
	// return -2;

	cSolve cond("cond.input");
	result = cond.solve();

	cout<<"there are "<<result<<" in result"<<endl;
	//3103 too low
	//3139 too high LUL
	//3203 too high
	//3137 correct WHAT?!
	return 0;
}
