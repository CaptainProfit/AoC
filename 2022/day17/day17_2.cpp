//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
// https://en.cppreference.com/w/cpp/string/multibyte/mbstowcs
// https://en.cppreference.com/w/cpp/string/basic_string/size
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
//       1514285714288 - 
//		 1514285714288
// 10091 is prime
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>
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
	ull index;
	vector<vector<pair<ull, ull>>> sprites;
	vector<ull> heights;
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

	vector<pair<ull, ull>>& getSprite(){return sprites[index];}
	ull getIndex(){return index;}
	ull getHeight(){return heights[index];}

}figures;

class cSignatures{
	public:
	vector<vector<char>> a;
	map<string, ull> theSignatures;

	cSignatures(){
		a.resize(128);
		for(int i = 0; i < 128; i ++){
			a[i].resize(128);
			for(int j = 0; j < 128; j ++){
				char c = i&j;
				char left = c;
				char right = c;
				for(int k = 0 ; k < 7; k++){
					left = j & (left<<1);
					right = j & (right>>1);
					c|= left|right;
				}
				a[i][j] = c;
			}
		}
	}

	char toSign(string& str){
		char x = 0;
		for(int i = 1; i < 8; i++){
			if(str[i] != ' '){
				x |= 1<<(i - 1);
			}
		}
		return x;
	}
	
	string makeSign(ull height, vector<string>& tetris ){
		string s;
		char cur = 0x7F;
		for(ull h = height; h >= 0; h--){
			char next = 0x7F^toSign(tetris[h]);
			if(a[cur][next] == 0){
				break;
	
			}
			else{
				cur = a[cur][next];
				s+=cur;
			}
		}
		return s;
	}

	bool findSign(string& s){
		return theSignatures.find(s) != theSignatures.end();
	}

	void addSign(string& s, ull height){
		theSignatures.emplace(s, height);
	}

} signs;

class cSolve{
	vector<char> directions;
	ull dirIt = 0;
	//1_000_000_000_000 = 1E12
	const ull restCounterFinish = 1000000000000; //2023
	ull period = 0;
	const string nether = "A=======B";
	const string etc = "|~~~~~~~|";
	const string chasteLine = "|       |";
	ull height = 0;
	ull restCounter = 1;
	vector<string> tetris;
	pair<ull, ull> pos;	
	vector<pair<ull, ull>>* currentSprite;
	
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
		period = directions.size()  * figures.sprites.size();
	}

	void paintSpriteAtPosAs(char c){
		for(ull i = 0; i < (*currentSprite).size(); i++){
			ull dy = (*currentSprite)[i].first;
			ull dx = (*currentSprite)[i].second;
			tetris[pos.first + dy][pos.second + dx] = c;
		}
	}
	// вернет false если движение продолжается
	bool moveTo(pair<ull, ull> dir){
		//check is move will succeed
		for(ull i = 0; i < (*currentSprite).size(); i++){
			ull dy = (*currentSprite)[i].first + dir.first;
			ull dx = (*currentSprite)[i].second + dir.second;

			// если границы колодца не мешают и
			// если не уперся в кучу уже навалившихся
			switch(tetris[pos.first + dy][pos.second + dx]){
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
		for(ull i = height + 1; i < tetris.size(); i++){
			bool flag = false;
			for(ull j = 1; j <= 7; j++){
				if(tetris[i][j] != ' '){
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

	ull solve(){
		//char emptyline[] = "       ";
		
		tetris.push_back(nether);
		for(; restCounter < restCounterFinish; restCounter++){
			
			if(restCounter % period == 0){
				string newSign = signs.makeSign(height, tetris);
				bool result = signs.findSign(newSign);
				if(!result){
					signs.addSign(newSign, height);
				}
				else{
					//bingo!
					cout<<" there are defiuinetyle sceeses was come" <<endl;
					break;
				}
			}
			// bottom edge is three units above the highest rock
			figures.chooseNext();
			currentSprite = &(figures.getSprite());
			while(tetris.size() < height + 4 + figures.getHeight()){
				tetris.push_back(chasteLine);
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
				if(directions[dirIt] == '<'){
					moveTo({0, -1});
				}
				dirIt++;
				dirIt %= directions.size();
				// print(); 
			}while(moveTo({-1, 0}));
			paintSpriteAtPosAs('0' + restCounter%10);
			// print();
			recalcHeight();
		}
		// print(true);
		currentSprite = nullptr;		
		return height;
	}

	//выводит состояние колодца в данный момент времени.
	array<string, screenHeight> screen;
	void print(bool isFlush = false){
		static ull k = 0;
		static const ull kMax = (100/screenWidth); // states per line
		// system("cls");
		if(!isFlush){
			screen[screenHeight - 1] += "step " + to_string(restCounter);
			
			if(tetris.size() >= screenHeight){
				screen[0] += etc;
			}
			else{
				screen[0] += nether;
			}
			ull ofset = tetris.size() - screenHeight;
			if(ofset < 0){
				ofset = 0;
			}
			for(ull i = 1; i < screenHeight - 1; i++){
				if(i < tetris.size())
					screen[i] += tetris[ofset + i];
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
			for(ull i = screenHeight - 1; i >=0; i--){
				cout<<screen[i]<<endl;
				screen[i] = "";
			}
			cout<<endl;
			k = 0;
		}
		else{
			for(ull i = 0; i < screenHeight; i++){
				string additional((k*20 - screen[i].size()), ' ');
				// for(ull j = 0; j < screen[i].size(); j++){
				// 	cout<<static_cast<unsigned int>(static_cast<unsigned char>(screen[i][j]))<<" ";
				// }
				// cout<<endl;
				screen[i] += additional;
			}
		}
	}
};

int main(void){
	long long result;

	cSolve test("test.input");
	result = test.solve();
	if(result != 1514285714288){
		cout<<"test.input failed with "<<result<<" in result(must be 1514285714288)"<<endl;	
		return -1;
	}
	cout<<"test.input passed: "<<result<<"(3068)"<<endl;	
	// return -2;

	cSolve cond("cond.input");
	result = cond.solve();

	cout<<"there are "<<result<<" in result"<<endl;
	
	return 0;
}
