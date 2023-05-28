//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/23
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <cassert>
#include <list>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <iostream> 
#include <fstream>
#define ull unsigned long long
using namespace std;
using namespace chrono;

struct cPoint{
	int y;
	int x;
	int t;

	cPoint(int _y, int _x, ){
		y = _y;
		x = _x;
		
		//order = {0, 1, 3, 4, 2};
	}

	cPoint& operator+=(const cPoint& rhs){
		y += rhs.y;
		x += rhs.x;
		return *this;
	}

	const long long compareWith(const cPoint& rhs) const {
		if(y == rhs.y)
			return x - rhs.x;
		return y - rhs.y;
	}
};

bool operator<(const cPoint& lhs, const cPoint& rhs){
	return lhs.compareWith(rhs) < 0; 
}

const cPoint dir[] = {
	{-1,  0}, //N
	{-1,  1}, //NE
	{ 0,  1}, //E
	{ 1,  1}, //SE
	{ 1,  0}, //S
	{ 1, -1}, //SW
	{ 0, -1}, //W
	{-1, -1}, //NW
	{ 0,  0}, //center
};

class cContainer{
	set<cPoint> points;
	string verticalBorder;
	
	public:
	void updateSizes(void){
		minX = INT_MAX;
		minY = INT_MAX;
		maxX = INT_MIN;
		maxY = INT_MIN;
		
		for(const cPoint &it:points){
			minX = minX > it.x ? it.x : minX;
			maxX = maxX < it.x ? it.x : maxX;
			minY = minY > it.y ? it.y : minY;
			maxY = maxY < it.y ? it.y : maxY;
		}
		verticalBorder = "+" + string(maxX - minX + 1, '-') + "+";
	}
	
	void addNewPoint(int y, int x){
		points.emplace(y, x);
	}

	void printMap(ostream& ostr, int step = -1){
		updateSizes();
		if(step != -1){
			ostr << " -- end of round " << step << " --" << endl;
		}
		// ostr << "elves: " << endl;
		// for(const cPoint &it:points){
		// 	ostr << " (" << it.y << ", " << it.x << ")" << endl;
		// }
		// ostr << "map: " << endl;
		ostr << verticalBorder << endl;
		for(int j = minY; j <= maxY; j++){
			string line(maxX - minX + 1, ' ');
			for(int i = minX; i <= maxX; i++){
				if(points.find(cPoint(j, i)) != points.end()){
					line[i - minX] = '#';
				}
			}
			ostr << "|" << line << "|" << endl;
		}
		ostr << verticalBorder << endl;
	}

	void orderChange(void){
		auto it = totOrder.begin();
		it++;
		int t = *it;
		totOrder.erase(it);
		totOrder.push_back(t);
	}

};

class cSolve{
	string name;
	system_clock::duration timeInterval;

	public:	
	cSolve(const string& filename){
		name = filename;
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		int y = 0;
		assert(0);
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			for(int x=0; x < line.length(); x++){
				if(line[x] == '#'){
					elves.addNewPoint(y ,x);
				}
			}
			y++;
		}
		ifstr.close();
		//printMap(cout);
	}

	void solve(void){
		//time_point<system_clock>
		const time_point<system_clock> tStart = chrono::system_clock::now();
		string outfile = name + ".output";
		ofstream ofstr(outfile, ios::binary);
		ofstr << "-- initial state --" << endl;
		?.printMap(cout);
		?.printMap(ofstr);
		ofstr.close();
		assert(0);
		const time_point<system_clock> tEnd = chrono::system_clock::now();
		timeInterval = tEnd - tStart;
	}

	void printUsedTime(void){
		auto dta= microseconds(duration_cast<microseconds>(timeInterval));		
		long long dt = dta.count();
		cout << "time used: ";
		if(dt < 1000){
			cout << dt << "us" << endl;
			return;
		}

		dt /= 1000;
		if(dt < 1000){
			cout << dt << "ms" << endl;
			return;
		}
		
		dt /= 1000;
		if(dt < 60){
			cout << dt << "s" << endl;
			return;
		}

		dt /= 60;
			cout << dt << "min" << endl;
			return;
	}	
	
	void printUsedMemory(void){
		cout << "i dont know how much memory was used" << endl;
	}

	int getResult(void){
		assert(0);
		return -2;
	}
};

int main(void){
	long long result;
	string names[] = {"test1", "test2", "cond"};
	int answers[] = {-1, -1, -1};
	
	for(int i = 0; i < 3; i++){
		cSolve test1(names[i]);
		test1.solve();
		result = test1.getResult();
		test1.printUsedTime();
		test1.printUsedMemory();
		if(result != answers[i]){
			cout << names[i] + " failed " << result << endl;
			return -2;
		}
		cout << names[i] + " passed" << endl;
	}
		cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
