//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/8
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
#define ull unsigned long long

using namespace std;

vector<vector<char>> table;//tallness
vector<vector<int>> visibility;// visible from # sides

int readFileToSMt(){	
	string line;
	table.reserve(100);
	visibility.reserve(100);
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		table.emplace_back(line.begin(), line.end());
		visibility.emplace_back(line.length(), 4);
	}
	ifstr.close();
	return 0;
}

int solve(){
	int counter = 0; // counts visible
	int width = table[0].size();
	int height = table.size();
	//check visibles from left/right
	for(int i = 0; i < height; i++){
		int t = -1;
		for(int j = 0; j < width; j++){
			if(t < table[i][j])
				t = table[i][j];			
			else
				visibility[i][j]--;
		}
		t = -1;
		for(int j = width - 1; j >=0; j--){
			if(t < table[i][j])
				t = table[i][j];			
			else
				visibility[i][j]--;
		}		
	}
	//check visibles from top/bottom
	for(int j = 0; j < width; j++){
		int t = -1;		
		for(int i = 0; i < height; i++){
			if(t < table[i][j])
				t = table[i][j];			
			else
				visibility[i][j]--;
		}
		t = -1;
		for(int i = height - 1; i >=0; i--){
			if(t < table[i][j])
				t = table[i][j];			
			else
				visibility[i][j]--;
		}		
	}
	//count visibles
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(visibility[i][j]>0)
				counter++;
		}
	}
	return counter;
}

int main(void){
	readFileToSMt();
	int result;
	result = solve();
	//now come from tree
	cout<<"there are "<<result<<" visible trees"<<endl;
	//1816 correct
	return 0;
}
