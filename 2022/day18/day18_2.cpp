//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/18
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
//difference - надо считать не всю поверхность, а 
// только внешнюю, замкнутые пустоты выбросить
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
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

	int compareZYX(const cPoint& c) const{
		if(z != c.z)
			return z - c.z;
		if(y != c.y)
			return y - c.y;
		// if(x != c.x)
			return x - c.x;
	}
	
	int compareXZY(const cPoint& c) const{
		if(x != c.x)
			return x - c.x;
		if(z != c.z)
			return z - c.z;
		// if(y != c.y)
			return y - c.y;
	}
	
	int compareYXZ(const cPoint& c) const{
		if(y != c.y)
			return y - c.y;
		if(x != c.x)
			return x - c.x;
		// if(z != c.z)
			return z - c.z;
	}

	int distTo(const cPoint& c) const{
		return 	abs(z - c.z) +
				abs(y - c.y) +
				abs(x - c.x);
	}	

};

int dist(const cPoint& lhs, const cPoint& rhs){
	return lhs.distTo(rhs);
}

bool less1(const cPoint& lhs, const cPoint& rhs){
		return lhs.compareZYX(rhs) < 0;
}
bool less2(const cPoint& lhs, const cPoint& rhs){
		return lhs.compareYXZ(rhs) < 0;
}
bool less3(const cPoint& lhs, const cPoint& rhs){
		return lhs.compareXZY(rhs) < 0;
}

bool operator<(const cPoint& lhs, const cPoint& rhs){
		return lhs.compareZYX(rhs) < 0;
}

vector<cPoint> cells;
map<cPoint, int> faces;

int readFileToSMt(string filename){
	string line;
	ifstream ifstr(filename, ios::binary);
	for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){

		int t1 = line.find(',', 0);
		int t2 = line.find(',', t1 + 1);
		int t3 = line.find(',', t2 + 1);
		string sz = line.substr(0, t1);
		string sy = line.substr(t1 + 1, t2 - 1);
		string sx = line.substr(t2 + 1, t3 - 1);
		int z = 2 * stoi(sz);
		int y = 2 * stoi(sy);
		int x = 2 * stoi(sx);
		cells.emplace_back(cPoint(z, y, x));
	}
	ifstr.close();
	return 0;
}

void addFace(cPoint& cell, int dz, int dy, int dx){
	cPoint face = cell;
	face.add(dz, dy, dx);
	map<cPoint, int>::iterator iter = faces.find(face);
	if(iter != faces.end()){
		faces.emplace(pair<cPoint,int>(face, 0));
	}
	faces[face] ++;
}

int solve1(){
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
		if (it.second == 1){
			acc++;
		}
	}	
	return acc;
}

int calculateNeighbours(){
	int acc = 0;
	for(int i = 1; i < cells.size(); i++){
		if(dist(cells[i - 1], cells[i]) == 2){
			acc++;
		}
	}
	return acc;
}

// нет, всё сложнее. бывают ситуации, 
// когда сортирую вдоль одной оси  -
// и сохраняются соседства по другим 
// осям и считается несколько раз.
int solve2(){
	int acc = 0;
	sort(cells.begin(), cells.end(), less1);
	acc += calculateNeighbours();
	sort(cells.begin(), cells.end(), less2);
	acc += calculateNeighbours();
	sort(cells.begin(), cells.end(), less3);
	acc += calculateNeighbours();
	return cells.size()*6 - acc;
}

int main(void){
	int result;
	readFileToSMt("test.input");
	result = solve1();
	// test должно быть 64
	if( result != 64){
		cout<<"test failed with "<<result<<" in test"<<endl;
		return 0;
	}
	cells.clear();
	faces.clear();

	readFileToSMt("cond.input");
	result = solve1();
	//result = solve2();
	cout<<"there are "<<result<<" in result"<<endl;
	// 4580 correct
	return 0;
}
