//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/23
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#define ull unsigned long long

using namespace std;

class cContainer{
};

class cSolve{
	string name;
	map<pair<int, int>, int >elves;
	int width = 0;
	int height = 0;

	void printMap(void){
		// int xmin = width;
		// int ymin = height;
		// int xmax = 0;
		// int ymax = 0;
		// for(auto it:elves){
		// 	if(xmin > it.first.first){
		// 		xmin = it.first.first;
		// 	}
		// 	if(xmax < it.first.first){
		// 		xmax = it.first.first;
		// 	}
		// 	if(ymin > it.first.second){
		// 		ymin = it.first.second;
		// 	}
		// 	if(ymax < it.first.second){
		// 		ymax = it.first.second;
		// 	}
		// }
		string s(width, ' ');
		vector<string> surface(height, s);
		cout << "elves: " << endl;
		for(auto it:elves){
			surface[it.first.second][it.first.first] = '0' + it.second;
			cout << it.second << ": (" << it.first.second << ", " << it.first.first << ")" << endl;
		}
		cout << " - - - - - - - - - - - - -" << endl;
		cout << "map:" << endl;
		for(int i = 0; i < surface.size(); i++){
			cout << surface[i] << endl;
		}
		cout << " - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
	}

	public:	
	cSolve(const string& filename){
		name = filename;
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		int y = 0;
		int id = 1;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			if(line[line.length() - 1] == '\r'){
				line.resize(line.length() - 1);
			}

			if(width < line.length()){
				width = line.length();
			}
			for(int x=0; x < line.length(); x++){
				if(line[x] == '#'){
					elves.emplace(pair<int, int>(x ,y), id++ );
				}
			}
			y++;
		}
		height = y;
		ifstr.close();
		printMap();
	}
	void solve(){
	
	}
	
		

};
int main(void){
	long long result;
	string names[] = {"test1","test2","cond"};
	string answers[] = {-1,-1,-1};
	
	for(int i = 0; i < 3; i++){
		cSolve test1(names[i]);
		test1.solve();
		result = test1.result();
		test1.printUsedTime();
		test1.printUsedMemory();
		if(result != answers[i]){
			cout << names[i] + " failed" << result << endl;
			return -2;
		}
		cout << names[i] + " passed" << endl;
	}
		cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
