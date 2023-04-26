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
#include <list>
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
		int z = stoi(sz);
		int y = stoi(sy);
		int x = stoi(sx);
		cells.emplace_back(cPoint(z, y, x));
	}
	ifstr.close();
	return 0;
}

int solve1(){
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
	map<cPoint, int> waterCells;
	cPoint start = cells[0];
	start += dirs[1];
	waterCells.emplace(start, 1);
	list<cPoint> stack;
	stack.push_back(start);

	//3) покрываю булыжник слоем воды толщиной 3 
	// этого должно хватить для всяких поворотов
	int acc = 0;
	for(; !stack.empty();){
		//4) достану и рассмотрю точку из списка
		cPoint iter = stack.back();
		stack.pop_back();

		//5) если точка рядом с булыжником - изменю её расстояние
		for(int i = 0; i < 6; i++){
			cPoint temp = iter + dirs[i];
			if(find(cells.begin(), cells.end(), temp) != cells.end()){
				waterCells[iter] = 1;
				break;
			}
		}

		//6) если такие точки уже есть
		if(waterCells.count(iter) == 0){
			continue;
		}
		// если точка слишком далеко от булыжника - она больше не интересует
		if(waterCells[iter] > 3){
			continue;
		}

		//7) рассмотрю всех соседей
		for(int i = 0; i < 6; i++){
			
			//8) если сосед булыжник, то это грань, считаю её
			if(find(cells.begin(), cells.end(), iter + dirs[i]) != cells.end()){
				acc++;
				continue; // автоматически не может быть ячейкой воды
			}

			//8) если сосед уже в списке воды - то больше не интересна
			// иначе добавляю в стак и буду рассматривать, 
			if(waterCells.find(iter+dirs[i]) == waterCells.end()){
				waterCells.emplace(iter+dirs[i], waterCells[iter] + 1);
				stack.push_front(iter+dirs[i]);
			}
		}
	}
	
	return acc;
}

int main(void){
	int result;
	string testFileName = "test01.input";
	int testResults[] = {6, 10, 12, 12, 9*6,9+2*(8+5), 18};
	for(int i = 1; i <= 5; i++){
		testFileName[5] = '0' + i;
		readFileToSMt(testFileName);
		result = solve1();
		// test должно быть 6
		if( result != testResults[i - 1]){
			cout<<testFileName<<" failed with "<<result<<" in test"<<endl;
			return 0;
		}
		cells.clear();
	}
	//return 2;

	readFileToSMt("test.input");
	result = solve1();
	// test должно быть 58
	if( result != 58){
		cout<<"test failed with "<<result<<" in test"<<endl;
		return -1;
	}
	cells.clear();

	readFileToSMt("cond.input");
	result = solve1();
	//result = solve2();
	cout<<"there are "<<result<<" in result"<<endl;
	// correct
	cells.clear();

	return 0;
}
