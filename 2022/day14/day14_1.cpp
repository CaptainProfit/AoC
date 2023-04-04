//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#define ull unsigned long long
string conf = "test";
using namespace std;

class cPath{
	vector<cPoint> a;
	cPoint* it = nullptr;
	cPoint dir;
	int it = 0;
	public:
	void addPoint(cPoint &b){
		a.emplace_back(b);
	}
	void addPoint(const string& b){
		a.emplace_back(b);
	}
	void offSet(int ofs){
		for(vector<cPoint>::iterator b = a.begin(); b != a.end(); b++){
			b->x -= ofs;
		}
	}
	
	void initIt(){
		if(a.size() == 0)
			return;
		it = &a[0];
		it = 0;
		if(a.size() == 1)
			return;		
		dir = a[0]>>a[1];
	}
	cPoint it(){		
		if(it != a[it+1]){
			it +=dir;
			return it;
		}
		else{
			it++;
			if(it != a.size() - 1){
				dir = a[it]>>a[it + 1];
			}
		}
	}
};

class cPoint{
	static int errors;
	public:
	int x, y;
	cPoint(int nx, int ny){
		x=nx;
		y=ny;
	}
	cPoint(const string& s){
		int d = s.find(',');
		if(d == -1)
			errors++;
		string sx = s.substr(0, d);
		string sy = s.substr(d+1, s.length());
		x = stoi(s.substr(0, d));
		y = stoi(s.substr(d+1, s.length()));
	}
	void offset(int ofs){
		x -= ofs;
	}

	friend cPoint& operator>>(cPoint& src, cPoint& dst){
		if(src.x == dst.x){
			if(src.y > dst.y){
				return dir[up];
			}
			else{
				return dir[down];
			}
		}
		else{
			if(src.x > dst.x){
				return dir[left];
			}
			else{
				return dir[right];
			}
		}
	}
};
enum{right, left, down, up};
cPoint src(500, 0), dir[4]={{1,0},{-1,0},{0,1},{0,-1}};

vector<vector<char>> map;
int readFileToSMt(){	
	string line;
	ifstream ifstr(conf+".input", ios::binary);
	//vector<cPath> paths;
	int maxy = 0;
	int minx = 500;
	int maxx = 500;
	while(1){
		getline(ifstr, line);

		paths.emplace_back();
		int i = paths.size()-1;		
		int caret = 0;
		while(caret < line.length() &&
				caret != -1){
			int interval = line.find('-') - caret;
			cPoint p(line.substr(caret, interval - 1));
			caret = interval + 2;//skip ->
			if(p.y>maxy){
				maxy = p.y;
			}
			if(p.x>maxx){
				maxx = p.x;
			}
			if(p.x<minx){
				minx = p.x;
			}
			paths[i].addPoint(p);
		}
		
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
	}
	//postprocessings:
	map.resize(maxy+1);
	int sizex = maxx - minx + 1;
	for(line:map)
		line.resize(sizex, ',');
	
	int ofs = minx;
	src.offset(ofs);
	for path:paths;
		path.offset(ofs);
	
	map[src] = '*';
	for(path:paths){
		for(it:path){
			map[it] = '#';
		}
	}
	return 0;
}

void solve(){	

}

void writeFileSmt(){
	int result = 0;
	ofstream ofstr(conf+".output", ios::binary);
	for(int i = 0 ; i < map.size(); i++){
		for(int j = 0; j < map[i].size(); j++){
			ofstr<<map[i][j];
		}
		ofstr<<endl;
	}
	ofstr<<"there are "<<result<<" in result"<<endl;
	ofstr.close();
}

int main(void){
	readFileToSMt();	
	solve();
	writeFileSmt();
	return 0;
}
