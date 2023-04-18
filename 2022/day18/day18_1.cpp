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

class cPoint{
	int z, y, x;
	public:
	cPoint():cPoint(0, 0, 0){}
	cPoint(int nz, int ny, int nx){
		z = nz;
		y = ny;
		x = nx;
	}
	
	void add(int dz, int dy, int dx){
		z += dz;
		y += dy;
		x += dx;
	}

	int compare(cPoint& c){
		if(z != c.z)
			return z - c.z;
		if(y != c.y)
			return y - c.y;
		// if(x != c.x)
			return x - c.x;
	}
	
	bool operator<(cPoint& c){
		return compare(c) < 0;
	}
};
vector<cPoint> cells;
map<cPoint, int> faces;

int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	do{
		getline(ifstr, line);
		int t1 = line.find(',',0);
		int t2 = line.find(',',t1);
		int t3 = line.find(',',t2);
		string sz = line.substr(0,t1 - 1);
		string sy = line.substr(t1 + 1,t2 - 1);
		string sx = line.substr(t2 + 1,t3 - 1);
		int z = 2 * stoi(sz);
		int y = 2 * stoi(sy);
		int x = 2 * stoi(sx);
		cells.emplace_back(cPoint(z, y, x));
	}while(!ifstr.eof());	
	ifstr.close();
	//sort(cells.begin(), cells.end());
	return 0;
}

void addFace(cPoint& cell, int dz, int dy, int dx){
	cPoint face = cell;
	face.add(dz, dy, dx);
	if(!faces.contains(face)){
		faces.emplace(pair<face, 0>);
	}
	faces[face] ++;
}

int solve(){
	//формирую грани
	for(auto it:cells){
		addFace(it,  0,  0,  1);
		addFace(it,  0,  0, -1);
		addFace(it,  0,  1,  0);
		addFace(it,  0, -1,  0);
		addFace(it,  1,  0,  0);
		addFace(it, -1,  0,  0);
	}	
	
	//считаю ответ
	int acc = 0;
	for(auto it:faces){
		if (it.second ==1){
			acc++;
		}
	}	
	return acc;
}

int main(void){
	int result;
	readFileToSMt();	
	result = solve();
	cout<<"there are "<<result<<" in result"<<endl;
	// test должно быть 64
	return 0;
}
