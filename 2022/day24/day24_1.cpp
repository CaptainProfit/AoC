//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/23
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <cassert>
#include <queue>
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
	uint8_t fwMask;
	uint8_t bwMask;
	
	cPoint(int _y, int _x, uint8_t fw = 0xFF, uint8_t bw = 0xFF ){
		y = _y;
		x = _x;
		fwMask = fw;
		bwMask = bw;
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

struct c3Point{
	int z;
	int y;
	int x;
	c3Point(int _z, int _y, int _x){
		z = _z;
		y = _y;
		x = _x;
	}
	c3Point& operator+=(const cPoint& rhs){
		z ++;
		y += rhs.y;
		x += rhs.x;
		return *this;
	}
};

const cPoint dir[] = {
	{-1,  0, 0x01, 0x04}, //N
	{ 0,  1, 0x02, 0x08}, //E
	{ 1,  0, 0x04, 0x01}, //S
	{ 0, -1, 0x08, 0x02}, //W
	{ 0,  0}, //center
};

const map<char, uint8_t> char2mask{
	{'.', 0x0},	
	{'^', 0x1},	
	{'>', 0x2},	
	{'v', 0x4},	
	{'<', 0x8},	
	{'#', 0x10},
};

const map<char, uint8_t> mask2char{
	{0x0, ' '},	
	{0x1, '^'},	
	{0x2, '>'},	
	{0x4, 'v'},	
	{0x8, '<'},	
	{0x10, '#'},
	{0x20, '.'},
};

class cContainer{
	//z, y, x
	vector<vector<vector<uint8_t>>> volume;
	vector<vector<vector<int>>> dist;
	set<cPoint> points;
	string verticalBorder;
	int height = 0;
	int width = 0;
	int depth = 0;

	bool checkStitch(vector<vector<uint8_t>> &layer){
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				if(volume[0][y][x] != layer[y][x])
					return false;
			}
		}
		return true;
	}

	void expandLayer(vector<vector<uint8_t>> &nextLayer){
		vector<vector<uint8_t>> &layer = volume[depth - 1];
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				uint8_t mask = 0;
				if( x == 0 ||
					y == 0 ||
					x == width - 1 ||
					y == height - 1){
					nextLayer[y][x] = layer[y][x];
					continue;
				}
				
				for(int i = 0; i < 4; i++){
					int yt = 1 + (y + dir[i].y + height - 3) % (height - 2);
					int xt = 1 + (x + dir[i].x + width  - 3) % (width - 2);
					if(layer[yt][xt] & dir[i].bwMask){
						mask |= dir[i].bwMask;
					}
				}
				nextLayer[y][x] = mask;
			}
		}
	}

	void makeVolume(void){
		depth = 1;
		while(true){
			vector<vector<uint8_t>> nextLayer(height, vector<uint8_t>(width, 0));
			expandLayer(nextLayer);
			if(!checkStitch(nextLayer)){
				volume.push_back(move(nextLayer));
				depth++;
			}
			else{
				return;
			}
		}
	}	

	int printLayer(int z, ostream& ostr = cout){
		ostr << verticalBorder << endl;
		int counter = 0;
		vector<vector<uint8_t>> &layer = volume[z];
		for(int y = 0; y < height; y++){
			string line(width, ' ');
			for(int x = 0; x < width; x++){
				switch(layer[y][x]){
					case 0: line[x] = ' '; counter++; break;
					case 1: line[x] = '^'; break;
					case 2: line[x] = '>'; break;
					case 4: line[x] = 'v'; break;
					case 8: line[x] = '<'; break;
					case 3: case 5: case 6: case 9: case 10:
					case 12: line[x] = '2'; break;
					case 7: case 11: case 13:
					case 14: line[x] = '3'; break;
					case 15: line[x] = '4'; break;
					case 16: line[x] = '#'; break;
					case 32: line[x] = '.'; break;
					case 64: line[x] = ','; break;
					default: line[x] = '?'; break;
				}
			}
			ostr << "|" << line << "|" << endl;
		}
		ostr << verticalBorder << endl;
		return counter;
	}

	void solidifyVolume(void){
		dist.resize(depth, 
					vector<vector<int>>(height,
					vector<int> (width, INT_MAX) ) );
		// dist.resize(depth); 
		for(int z = 0; z < depth; z++){
			// dist[z].resize(height);
			for(int y = 0; y < height; y++){
				// dist[z][y].resize(width, INT_MAX);
				for(int x = 0; x < width; x++){
					if(volume[z][y][x] != 0){
						volume[z][y][x] = 0x10;
					}
				}
			}
		}
		dist[0][0][1] = 0;
	}

	public:

	void print(ostream& ostr = cout){
		int counter = 0;
		for(int z = 0; z < depth; z++){
			ostr << " == layer " << z << " == " << endl;
			counter += printLayer(z, ostr);
		}
		ostr << counter << " total free places" << endl;
	}

	void init(vector<vector<uint8_t>> &seed){
		volume.push_back(move(seed));
		width = volume[0][0].size();
		height = volume[0].size();
		depth = volume.size();
		verticalBorder = "+" + string(width, '-') + "+";
		makeVolume();
		solidifyVolume();
	}

	int& operator[](c3Point& s){
		return dist[s.z][s.y][s.x];
	}

	uint8_t& operator[](c3Point* s ){
		return volume[s->z][s->y][s->x];
	}

	bool wrap(c3Point &s){
		s.z %= depth;
		return s.y >= 0 && s.x >= 0 && s.y < height && s.x < width;
	}

	bool isFinished(c3Point &s){
		return s.y == (height - 1);
	}

	// ~cContainer(void){
	// 	// vector<vector<vector<uint8_t>>> volume;
	// 	for(auto itz = volume.begin(); itz != volume.end(); itz++){
	// 		for(auto ity = itz->begin(); ity != itz->end(); ity++){
	// 			ity->clear();
	// 		}
	// 		itz->clear();
	// 	}
	// 	volume.clear();
		
	// 	// vector<vector<vector<int>>> dist;
	// 	for(auto itz = dist.begin(); itz != dist.end(); itz++){
	// 		for(auto ity = itz->begin(); ity != itz->end(); ity++){
	// 			ity->clear();
	// 		}
	// 	}
	// 	for(auto itz = dist.begin(); itz != dist.end(); itz++){
	// 		try{
	// 			itz->clear();
	// 		}catch(const std::exception& e){
	// 			cout << "ayayay: " << e.what() << endl;
	// 			throw;
	// 		}
	// 	}
	// 	dist.clear();
	// 	points.clear();
	// 	verticalBorder.clear();
	// }
};

class cSolve{
	string name;
	time_point<system_clock> tStart;
	time_point<system_clock> tReaded;
	time_point<system_clock> tFormed;
	time_point<system_clock> tSolved;
	
	cContainer vol;
	int steps;

	public:	
	cSolve(const string& filename){
		name = filename;
		steps = -2;
		tStart = chrono::system_clock::now();
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		vector<vector<uint8_t>> surface;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			vector<uint8_t> buf;
			buf.reserve(line.length() - 1);
			for(int i = 0; i < line.length(); i++){
				if(char2mask.find(line[i]) != char2mask.end()){
					buf.push_back(char2mask.at(line[i]));
				}
			}
			surface.push_back(move(buf));
		}
		ifstr.close();
		tReaded = chrono::system_clock::now();
		vol.init(surface);
		tFormed = chrono::system_clock::now();
		ofstream ofstr(name + ".map", ios::binary);
		vol.print(ofstr);
		ofstr.close();
	}

	void solve(void){
		queue<c3Point> que;
		steps = 0;
		int zOld = -1;
		// string outfile = name + ".output";
		// ofstream ofstr(outfile, ios::binary);
		que.push(c3Point(0, 0, 1));
		//vol[&(que.top())] = 0x20;
		while(1){
			c3Point s = que.front();
			vol[&s] = 0x40;
			que.pop();
			if(zOld != s.z){
				zOld = s.z;
				steps++;
			}
			if(vol.isFinished(s)){
				break;
			}
			for(int i = 0; i < 5; i++){
				c3Point t(s);
				t += dir[i];
				if(vol.wrap(t)){
					if(vol[&t] == 0){
						vol[&t] = 0x20;
						vol[t] = vol[s] + 1;
						que.push(t);
					}
				}
			}
			//vol.print();
		}
		// ofstr.close();
		tSolved = chrono::system_clock::now();
	}

	void printUsedTime(void){
		system_clock::duration timeInterval = tSolved - tStart;
		system_clock::duration t1 = tReaded - tStart;
		system_clock::duration t2 = tFormed - tReaded;
		system_clock::duration t3 = tSolved - tFormed;
		
		auto dta= microseconds(duration_cast<microseconds>(timeInterval));		
		auto dt1= microseconds(duration_cast<microseconds>(t1));
		auto dt2= microseconds(duration_cast<microseconds>(t2));
		auto dt3= microseconds(duration_cast<microseconds>(t3));
		long long dt = dta.count();
		float perc1 = (float)(dt1.count())/dta.count();
		float perc2 = (float)(dt2.count())/dta.count();
		float perc3 = (float)(dt3.count())/dta.count();
		cout << "read: " << perc1 << " form: " << perc2 << " solve: " << perc3 << endl;
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
		return steps - 1;
	}
};

int main(void){
	long long result;
	string names[] = {"test1", "test2", "cond"};
	int answers[] = {10, 18, 334};
	
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
