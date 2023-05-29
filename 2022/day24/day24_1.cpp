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

const cPoint dir[] = {
	{-1,  0, 0x01, 0x04}, //N
	{ 0,  1, 0x02, 0x08}, //E
	{ 1,  0, 0x04, 0x01}, //S
	{ 0, -1, 0x08, 0x02}, //W
	{ 0,  0}, //center
};

const map<char, uint8_t> char2mask{
	{' ', 0x0},	
	{'^', 0x1},	
	{'>', 0x2},	
	{'V', 0x4},	
	{'<', 0x8},	
	{'#', 0x10},
};

const map<char, uint8_t> mask2char{
	{0x0, ' '},	
	{0x1, '^'},	
	{0x2, '>'},	
	{0x4, 'V'},	
	{0x8, '<'},	
	{0x10, '#'},
};

class cContainer{
	//z, y, x
	vector<vector<vector<uint8_t>>> volume;
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
		assert(1);
		vector<vector<uint8_t>> &layer = volume[depth - 1];
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				uint8_t mask = 0;
				if(layer[y][x] == '#'){
					nextLayer[y][x] = '#';
					continue;
				}
				for(int i = 0; i < 4; i++){
					int yt =1 + (y + dir[i].y + height - 1) % height;
					int xt =1 + (x + dir[i].x + width  - 1) % width;
					if(1){
						mask |= layer[yt][xt] & dir[i].bwMask;
					}
				}
			}
		}
	}

	void makeVolume(void){
		depth = 1;
		while(true){
			vector<vector<uint8_t>> nextLayer(height, vector<uint8_t>(width, 0));
			expandLayer(nextLayer);
			if(checkStitch(nextLayer)){
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
					case 4: line[x] = 'V'; break;
					case 8: line[x] = '<'; break;
					case 3: case 5: case 6: case 9: case 10:
					case 12: line[x] = '2'; break;
					case 7: case 11: case 13:
					case 14: line[x] = '3'; break;
					case 15: line[x] = '4'; break;
					default: line[x] = '#'; break;
				}
			}
			ostr << "|" << line << "|" << endl;
		}
		ostr << verticalBorder << endl;
		ostr << counter << " free places" << endl;
		return counter;
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
		return;
		makeVolume();
	}
};

class cSolve{
	string name;
	system_clock::duration timeInterval;
	cContainer vol;

	public:	
	cSolve(const string& filename){
		name = filename;
		ifstream ifstr(name + ".input", ios::binary);			
		string line;
		vector<vector<uint8_t>> surface;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			vector<uint8_t> buf(line.cbegin(), line.cend());
			buf.reserve(line.length());
			for(int i = 0; i < line.length(); i++){
				if(char2mask.find(line[i]) != char2mask.end()){
					buf.push_back(char2mask.at(line[i]));
				}
			}
			surface.push_back(move(buf));
		}
		ifstr.close();
		vol.init(surface);
		vol.print();
	}

	void solve(void){
		assert(0);
		//time_point<system_clock>
		// const time_point<system_clock> tStart = chrono::system_clock::now();
		// string outfile = name + ".output";
		// ofstream ofstr(outfile, ios::binary);
		// ofstr << "-- initial state --" << endl;
		// ?.printMap(cout);
		// ?.printMap(ofstr);
		// ofstr.close();
		// const time_point<system_clock> tEnd = chrono::system_clock::now();
		// timeInterval = tEnd - tStart;
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
