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
#include <queue>
#include <algorithm>
#define ull unsigned long long

using namespace std;

class cPoint{
	public:
	int z, y, x;
	cPoint():cPoint(0, 0, 0){}
	cPoint(int nz, int ny, int nx){
		z = nz;
		y = ny;
		x = nx;
	}
	
	cPoint(const cPoint& c){
		z = c.z;
		y = c.y;
		x = c.x;
	}
	
	cPoint& add(const cPoint& c){
		z += c.z;
		y += c.y;
		x += c.x;
		return *this;
	}
	
	cPoint& sub(const cPoint& c){
		z -= c.z;
		y -= c.y;
		x -= c.x;
		return *this;
	}
	
	cPoint& add(int dz, int dy, int dx){
		z += dz;
		y += dy;
		x += dx;
		return *this;
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

	cPoint& operator+=(const cPoint& rhs){
		this->add(rhs);
		return *this;
	}

	cPoint& operator-=(const cPoint& rhs){
		this->sub(rhs);
		return *this;
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

bool operator==(const cPoint& lhs, const cPoint& rhs){
		return lhs.compareZYX(rhs) == 0;
}

cPoint& operator+(cPoint lhs, const cPoint& rhs){
	return lhs.add(rhs);
}

bool less(pair<int, cPoint> lhs,pair<int, cPoint> rhs){
	return lhs.first < rhs.first;
}

class cSolve{
	string nameId;
	vector<cPoint> cells;
	map<cPoint, int> faces;
	map<cPoint, int> waterCells;
	int result = -1;
	public:
	
	cSolve(string filename){
		// 1) read data
		nameId = filename;
		string line;
		ifstream ifstr(filename, ios::binary);
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){

			int t1 = line.find(',', 0);
			int t2 = line.find(',', t1 + 1);
			int t3 = line.find(',', t2 + 1);
			string sz = line.substr(0, t1);
			string sy = line.substr(t1 + 1, t2 - 1);
			string sx = line.substr(t2 + 1, t3 - 1);
			int z = stoi(sz);
			int y = stoi(sy);
			int x = stoi(sx);
			cells.emplace_back(cPoint(z, y, x));
		}
		ifstr.close();
	}

	int solveIt(){
		// формирую направления 
		vector<cPoint> dirs;
		dirs.push_back(cPoint( 1, 0, 0));
		dirs.push_back(cPoint(-1, 0, 0));
		dirs.push_back(cPoint( 0, 1, 0));
		dirs.push_back(cPoint( 0,-1, 0));
		dirs.push_back(cPoint( 0, 0, 1));
		dirs.push_back(cPoint( 0, 0,-1));	
	
		//1) взять точку рядом с камнем
		sort(cells.begin(), cells.end());
		
		//2) делаю мап из точек и их дальности от поверхности булыжника
	
		cPoint start = cells[0];
		start += dirs[1];
		//waterCells.emplace(start, 1);
		priority_queue<pair<int, cPoint>> stack;
		stack.push(pair<int, cPoint>(1, start));
		

		//3) покрываю булыжник слоем воды толщиной 3 
		// этого должно хватить для всяких поворотов
		int acc = 0;
		for(; !stack.empty();){
			//4) достану и рассмотрю точку из списка
			pair<int, cPoint> iter = stack.top();
			stack.pop();
			
			//5) смотрю на соседей в первый раз - если точка рядом с булыжником - изменю её расстояние
			// и попутно посчитаю поверхность.
			for(int i = 0; i < 6; i++){
				cPoint temp = iter.second + dirs[i];
				if(find(cells.begin(), cells.end(), temp) != cells.end()){
					iter.first = 1;
				}
			}
			
			//ой нет, эта точка существует и, что хуже - границы рядом с булыжником с ней уже посчитаны.
			if(waterCells.find(iter.second) != waterCells.end() &&
				waterCells[iter.second] == 1){
				continue;
			}
			waterCells[iter.second] = iter.first;

			//6) точка слишком далеко от булыжника
			if(iter.first >= 4){
				continue;
			}

			//5) смотрю на соседей ещё раз - если вершина не намокла - добавляю в очередь
			// если вершина мокрая, но индекс будет - уменьшаю индекс и в очередь. 
			// это приведет к нахлесту, но к счастью глубина все равно не больше 4х, 
			// так что нахлест будет конечным.
			for(int i = 0; i < 6; i++){
				cPoint temp = iter.second + dirs[i];
				//пропускаю точки булыжника
				if(find(cells.begin(), cells.end(), temp) != cells.end() ){
					acc++;
					continue;
				}
				// добавляю в стак и буду рассматривать новые точки
				if(waterCells.find(temp) == waterCells.end() ){
					stack.push(pair<int, cPoint>(iter.first + 1, temp));
				}
				// повторно добавляю в стак и буду рассматривать 
				else if(waterCells[temp] > iter.first + 1){
					stack.push(pair<int, cPoint>(iter.first + 1, temp));
					waterCells[temp] = iter.first + 1;
				}
			}
		}
		result = acc;
		return result;
	}
	
	void printBoulder(){
		//3) print result
		int miny = +10000;
		int maxy = -10000;
		int minx = +10000;
		int maxx = -10000;
		int minz = +10000;
		int maxz = -10000;
		for(auto p:waterCells){
			maxx = (p.first.x > maxx)? p.first.x: maxx;
			minx = (p.first.x < minx)? p.first.x: minx;
			maxy = (p.first.y > maxy)? p.first.y: maxy;
			miny = (p.first.y < miny)? p.first.y: miny;
			maxz = (p.first.z > maxz)? p.first.z: maxz;
			minz = (p.first.z < minz)? p.first.z: minz;
		}

		cout<<"****** "<<nameId<<" ******"<<endl;
		cout<<"z: ["<<minz<<" "<<maxz<<"]"<<endl;
		cout<<"y: ["<<miny<<" "<<maxy<<"]"<<endl;
		cout<<"x: ["<<minx<<" "<<maxx<<"]"<<endl;
		int dec = 100/(maxx-minx + 3);
		for(int z = minz; z <= maxz; z+=dec){
			cout<<"layers "<<z<<" to " << z + dec - 1 << " :"<<endl;
			for(int y = miny; y <= maxy; y++){
				for(int i = 0; i < dec; i++){
					for(int x = minx; x <= maxx; x++){
						cPoint temp(z + i, y, x);
						if(waterCells.find(temp ) != waterCells.end()){
							cout << waterCells[temp];
						}				
						else if(find(cells.begin(), cells.end(), temp) != cells.end()){
							cout << "#";
						}
						else{
							cout << ".";
						}
					}
					cout << "   ";
				}
				cout << endl;
			}
		}
	}

	int getResult(){
		return result;
	}
};


	
int main(void){
	int result;
	string testFileName = "test01.input";
	int testResults[] = {6, 10, 12, 12, 9*6,9+2*(8+5), 18};
	for(int i = 1; i <= 5; i++){
		testFileName[5] = '0' + i;
		cSolve test(testFileName);
		test.solveIt();
		result = test.getResult();
		//test.printBoulder();

		if( result != testResults[i - 1]){
			cout<<testFileName<<" failed with "<<result<<" in test"<<endl;
			return 0;
		}
	}
	
	cSolve test("test.input");
	result = test.solveIt();
	//test.printBoulder();
	// test должно быть 58
	if( result != 58){
		cout<<"test failed with "<<result<<" in test"<<endl;
		return -1;
	}
	
	//return 3;

	cSolve cond("cond.input");
	
	result = cond.solveIt();
	cond.printBoulder();
	//result = solve2();
	cout<<"there are "<<result<<" in result"<<endl;
	// 2610 correct 

	return 0;
}
