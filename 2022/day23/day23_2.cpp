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

const pair<uint8_t, int> dirMask[] = {
	{0xFF, 8}, // center
	{0x83, 0}, // nort
	{0x0E, 2}, // east
	{0x38, 4}, // south
	{0xE0, 6}, // west
};
// const uint8_t nortMask = 0x83;
// const uint8_t eastMask = 0x0E;
// const uint8_t soutMask = 0x38;
// const uint8_t westMask = 0xE0;

struct cPoint{
	int y;
	int x;
	int propose;
	//list<int> order;

	cPoint(int _y, int _x, int prop = -1){
		y = _y;
		x = _x;
		propose = prop;
		//order = {0, 1, 3, 4, 2};
	}

	cPoint& operator+=(const cPoint& rhs){
		y += rhs.y;
		x += rhs.x;
		return *this;
	}

	void acceptPropose(){
		// order.remove(propose);
		// order.push_back(propose);
		propose = -1;
	}

	const int getX() const {
		return x;
	}
	
	const int getY() const {
		return y;
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
	int minX, maxX, minY, maxY;
	int changes;
	string verticalBorder;
	bool differentState = true;
	map<cPoint, list<cPoint>> proposes;
	list<int> totOrder = {0, 1, 3, 4, 2};
	
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

	void clear(void){
		points.clear();
		proposes.clear();
	}

	void makeProposes(){
		differentState = false;
		for(set<cPoint>::iterator it = points.begin(); it != points.end(); it++){
			uint8_t mask = 0;
			//осматриваю границы.
			for(int k = 0; k < 8; k++){
				cPoint scanPos(*it);
				scanPos += dir[k];
				if(points.find(scanPos) != points.end()){
					mask |= 1 << k;
				}
			}

			//делаю предположение, куда пытаюсь перейти.
			bool proposed = false;
			for(list<int>::const_iterator it2 = totOrder.cbegin(); totOrder.cend() != it2; it2++){
				if( (mask & dirMask[*it2].first) == 0){
					cPoint scanPos(*it);
					scanPos += dir[dirMask[*it2].second];
					//scanPos.propose = *it2;
					proposes[scanPos].push_back(*it);
					proposed = true;
					if( it2 != totOrder.cbegin()){
						differentState = true;
					}
					break;
				}
			}
			if(!proposed){
				proposes[*it].push_back(*it);
			}
		}
	}
	
	void resolveCollisions(void){
		set<cPoint> nextPoints;
		changes = 0;
		//1) перебираю штуки. составляю список претендентов.
		for(map<cPoint, list<cPoint>>::iterator it = proposes.begin(); it != proposes.end(); it++){
			
			if(it->second.size() == 1){
				//1) на точку претендует ктото один - нет коллизий, 
				cPoint newPoint(it->first);
				//newPoint.acceptPropose();
				nextPoints.emplace(newPoint);
				//it++;
				changes++;
			}
			else{
				//2) на точку претендует несколько типов - всем отказываю.
				for(list<cPoint>::const_iterator it2 = it->second.cbegin(); it2 != it->second.cend(); it2++){
					nextPoints.emplace(*it2);
				}
			}
		}
		points = move(nextPoints);
		proposes.clear();
	}

	void orderChange(void){
		auto it = totOrder.begin();
		it++;
		int t = *it;
		totOrder.erase(it);
		totOrder.push_back(t);
	}

	bool stabilized(void){
		return !differentState;
	}

	int getArea(void){
		return (maxX - minX + 1) * (maxY - minY + 1);
	}

	int getSize(void){
		return points.size();
	}

	int getChanged(void){
		return changes;
	}
};

class cSolve{
	string name;
	cContainer elves;
	int steps = 0;
	system_clock::duration timeInterval;

	void printMap(ostream& ostr){
		elves.printMap(ostr);
	}

	public:	
	cSolve(const string& filename){
		name = filename;
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		int y = 0;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			for(int x=0; x < line.length(); x++){
				if(line[x] == '#'){
					elves.addNewPoint(y ,x);
				}
			}
			y++;
		}
		ifstr.close();
		steps = 0;
		//printMap(cout);
	}

	void solve(void){
		//time_point<system_clock>
		const time_point<system_clock> tStart = chrono::system_clock::now();
		string outfile = name + ".output";
		//ofstream ofstr(outfile, ios::binary);
		//cout << "-- initial state --" << endl;
		//ofstr << "-- initial state --" << endl;
		//elves.printMap(cout);
		//elves.printMap(ofstr);
		for(; !elves.stabilized(); steps++){
			elves.makeProposes();
			elves.resolveCollisions();
			// elves.printMap(ofstr, step);
			// elves.printMap(cout, step);
			if(steps && steps % 100){
				cout << "step " << steps << " changed points " << elves.getChanged() << endl;
			}
			elves.orderChange();
		}
		//ofstr.close();
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
		//elves.updateSizes();
		return steps;
	}
};

int main(void){
	long long result;
	string names[] = {"test1", "test2", "cond"};
	int answers[] = {4, 20, 918}; //6 min считал.
	
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
