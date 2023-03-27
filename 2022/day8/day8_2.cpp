//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/8
//part two changes - find a tree with maximum scenic scope
// is product of #visible trees from this one to 4 directions
//need the maxium scenic
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
//vector<vector<int>> visibility;// visible from # sides

int readFileToSMt(){	
	string line;
	table.reserve(100);
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		table.emplace_back(line.begin(), line.end());
	}
	ifstr.close();
	return 0;
}

ull solve(int si, int sj){
	if(si == 0 || sj == 0 || si == table.size()-1 || sj ==table[0].size() - 1)
		return 0;
	int width = table[0].size();
	int height = table.size();
	long long lc = 1, rc = 1, uc = 1, dc = 1;
	//check visibles from left/right	
	for(int i = si+1; i < height; i++){
		if(table[si][sj] > table[i][sj])
			rc++;
		else break;
	}
	
	for(int i = si-1; i >= 0; i--){
		if(table[si][sj] > table[i][sj])
			lc++;
		else break;
	}
	
	//check visibles from top/bottom	
	for(int j = sj + 1; j < width; j++){
		if(table[si][sj] > table[si][j])
			uc++;
		else break;
	}
	for(int j = sj - 1; j >= 0; j--){
		if(table[si][sj] > table[si][j])
			dc++;
		else break;
	}

	return lc*rc*uc*dc;
}

int main(void){
	readFileToSMt();
	ull result = 0;
	int width = table[0].size();
	int height = table.size();
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			ull value = solve(i, j);
			if(value > result)
				result = value;
		}
	}	
	//now come from tree
	cout<<"there are "<<result<<" maximum scenic"<<endl;
	//5090328 too high
	//4662000 too high
	//2331000 too high hehehe okay, stop it.
	//419328 not the right answer	
	return 0;
}
