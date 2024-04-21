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
#define CENTERX 500
using namespace std;
string conf = "test";

typedef enum{eleft = 0, eright = 1, eup = 2, edown = 3} eDir;
class cPoint{
	int b;
	int c;
	public:
	int z;
	int x;
	int y;
	
	cPoint(const int &nx, const int &ny){
		x = nx;
		y = ny;
	}
	
	cPoint(const string& s){
		int d = s.find(',');
		string sx = s.substr(0, d);
		string sy = s.substr(d+1, s.length());
		x = stoi(s.substr(0, d));
		y = stoi(s.substr(d+1, s.length()));
	}

	void operator>>(const eDir& dir);

	eDir operator-(cPoint& start){
		if(start.x > this->x){
			return eleft;
		} 
		if(start.x < this->x){
			return eright;
		}
		if(start.y > this->y){
			return eup;
		} 
		return edown;
	}

};

bool cPoint::operator!=(cPoint& a, cPoint& b){
		return (a.x != b.x) || (a.y != b.y);
	}

cPoint dirs[4]={{-1,0},{1,0},{0,-1},{0,1}};

cPoint& cPoint::operator+=(eDir dir){
	this->x += dirs[dir].x;
	this->y += dirs[dir].y;
	return *this;
}

void cPoint::operator>>(const eDir& dir){
	this->x+= dirs[dir].x;
	this->y+= dirs[dir].y;
}

template<class T>
class c2vec{
	int ofs;
	vector<T> leftv;
	T center;
	vector<T> rightv;
	int min = CENTERX;
	int max = CENTERX;
	public:
	void setOfs(int i){
		ofs = i;
	}

	T& operator[](int i){
		if(i > CENTERX)
			return rightv[i - 1];
		if(i < CENTERX)
			return leftv[i - 1];
		return center;		
	}
	
	c2vec(int min, int max, T c){
		leftv.resize(min, c);
		center = c;
		rightv.resize(max, c);
	}

	ostream& operator<<(ostream& os){
		for(int i = -leftv.size() ; i < rightv.size() ; i++)
			os<<this[i];
		return os;
	}

	void resize(int minx, int maxx, T def){
		center = def;
		leftv.resize(CENTERX - minx, def);
		rightv.resize(maxx - CENTERX, def);
	}
};

cPoint src(500, 0);

class cMap{
	vector<c2vec<char>> map;
	bool isPen;
	char pen;
	cPoint penPos;
	int minx;
	int maxx;
	public:
	
	void startPen(cPoint&n,  const char& symb){
		isPen = true;
		penPos = n;		
		pen = symb;
		map[penPos.y][penPos.x] = pen;
	}

	void continuePen(cPoint n ){
		eDir dir = n - penPos;
		while(penPos != n){
			penPos += dir;
			map[penPos.y][penPos.x] = pen;
		}
		map[penPos.y][penPos.x] = pen;
	}

	void endPen(){
		isPen = false;
	}

	void convex(cPoint& n){
		bool resize = false;
		if(map.size()>= n.y){
			map.resize(n.y + 1);
			resize = true;
		}
		if(minx < n.x){
			minx = n.x;
			resize = true;
		}
		if(maxx < n.x){
			maxx = n.x;
			resize = true;
		}
		for(int i = 0; i < map.size(); i ++){
			map[i].resize(minx, maxx, '.');
		}
	}

	c2vec<char>& operator[](int p){
		return map[p];
	}

	int size(){
		return map.size();
	}
} map;

int readFileToSMt(){	
	string line;
	ifstream ifstr(conf+".input", ios::binary);
	//vector<cPath> paths;
	int maxy = 0;
	int minx = 500;
	int maxx = 500;
	while(1){
		getline(ifstr, line);
		int caret = 0;
		int interval = line.find('-') - caret;
		cPoint a(line.substr(caret, interval - 1));
		map.startPen(a, '#');
		map[a.y][a.x] = 's';

		while(1){			
			caret = interval + 2;//skip ->
			interval = line.find('-', caret) - caret;
			if(caret >= line.length() ||
				caret == -1){
					break;
			}
			cPoint b(line.substr(caret, interval - 1));
			map[b.y][b.x] = 'e';
			map.continuePen(b);			
			a = b;
		}
		map.endPen();
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
	}
	//postprocessings:
	return 0;
}

void solve(){	

}

void writeFileSmt(){
	int result = 0;
	ofstream ofstr(conf+".output", ios::binary);
	for(int i = 0 ; i < map.size(); i++){
		ofstr<<map[i]<<endl;
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
