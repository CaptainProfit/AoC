//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
//find fewest path from elevation a
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
#define ull unsigned long long

using namespace std;

class cPoint{
	public:
	int x = -1, y = -1;
	int dist = -1;
	
	cPoint(int nx, int ny, int ndist = -1){
		x = nx;
		y = ny;
		dist = ndist;
	}

	cPoint(void){
		x = 0;
		y = 0;
		dist = 0;
	}

	cPoint(cPoint &p){
		x = p.x;
		y = p.y;
		dist = p.dist;
	}

	cPoint(vector<int> &p){
		switch(p.size()){
			default:
			case 3: dist = p[2];
			case 2: y = p[1];
			case 1: x = p[0];
			case 0: ;
		}
	}

	cPoint(int* p){
		switch(4){
			default:
			case 3: dist = p[2];
			case 2: y = p[1];
			case 1: x = p[0];
			case 0: ;
		}
	}
	
	cPoint& operator+=(const cPoint &a){
		x += a.x;
		y += a.y;
		dist = min(dist, a.dist);
		return *this;
	}
	
	friend cPoint operator+(cPoint lhs, const cPoint& rhs){
		lhs += rhs;
		return lhs;
	}

	cPoint& operator-=(const int &a){
		x -= a;
		y -= a;
		return *this;
	}	

	friend cPoint operator-(cPoint lhs, const int& rhs){
		lhs -= rhs;
		return lhs;
	}

	friend bool operator<(const cPoint &lhs, const cPoint &rhs){
		return lhs.x < rhs.x && lhs.y < rhs.y;
	}

	friend bool operator<=(const cPoint &lhs, const cPoint &rhs){
		return lhs.x <= rhs.x && lhs.y <= rhs.y;
	}
	// friend bool operator<(cPoint &lhs, cPoint &rhs){
	// 	return lhs.x < rhs.x || lhs.y < rhs.y;
	// }

	friend bool operator>(const cPoint &lhs, const cPoint &rhs){
		return rhs < lhs;
	}
	
};

class cSolve{
	vector<string> heights;
	vector<vector<int>> dist;
	cPoint zero{0, 0};
	//cPoint zero2(0, 0);
	cPoint f;
	cPoint s;
	cPoint sizes;
	cPoint dirs[4] = {cPoint(-1, 0),cPoint(1, 0),cPoint(0, -1),cPoint(0, 1)};

	bool checkDir(cPoint &pos){
		if((zero <= pos) && (pos <= sizes - 1))
			return true;
		return false;
	}

	public:
	void addString(string &line){
		heights.emplace_back();
		heights.back() = line;
		sizes.x = line.length();
		if(line.find('E') != -1){
			f.y = sizes.y;
			f.x = line.find('E');
			//line[f.x] = 'z' + 1; 'A';
		}
		if(line.find('S') != -1){
			s.y = sizes.y;
			s.x = line.find('S');
			//line[f.x] = 'z' + 1; 'A';
		}
		sizes.y++;
	}

	void calculateDistances(void){
		list<cPoint> queue;
		heights[s.y][s.x] = 'a'; // 'S' equal a?
		heights[f.y][f.x] = 'z' + 1;// 'F' equal z?
//		queue.emplace_front(s.x, s.y, 0);

		dist.resize(sizes.y);
		for(int i = 0; i < dist.size(); i++){
			dist[i].resize(sizes.x, -1);		
		}

		for(int i = 0; i < sizes.y; i++){
			for(int j = 0; j < sizes.x; j++){
				if(heights[i][j] != 'a')
					continue;
				dist[i][j] = 0;
				queue.emplace_front(j, i, 0);
			}
		}

		while(!queue.empty()){
			cPoint pos(queue.back());
			queue.pop_back();
			
			for(auto dir:dirs){
				cPoint newPos = pos+dir;
				newPos.dist = pos.dist + 1;
				//if direction correct
				if(!checkDir(newPos))
					continue;
				//if not too high
				if((int)heights[newPos.y][newPos.x] - (int)heights[pos.y][pos.x] > 1)
					continue;
				//if dist better than now
				if(dist[newPos.y][newPos.x] != -1 &&
					newPos.dist >= dist[newPos.y][newPos.x])
					continue;
				dist[newPos.y][newPos.x] = newPos.dist;
				queue.emplace_front(newPos);				
			}
		}
	}
	
	int distTo(void){
		return dist[f.y][f.x];
	}

	void printMap(void){
		cout<<"distance map:"<<endl;
		for(int i = 0; i < sizes.y; i++){
			for(int j = 0; j < sizes.x; j++){
				cout<<dist[i][j]<<"\t";				
			}
			cout<<endl;
		}
	}

}solve1;

int readFileToSMt(void){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		solve1.addString(line);
	}
	return 0;
}

int solve(void){
	solve1.calculateDistances();
	solve1.printMap();
	return solve1.distTo();	
}

int main(void){
	int fewestSteps;
	readFileToSMt();	
	fewestSteps = solve();
	cout<<"there are "<<fewestSteps<<" in result"<<endl;
	return 0;
	//525 correct!
	//534 correct
	//.\day12_1.exe >map.txt for distance map in file
}
