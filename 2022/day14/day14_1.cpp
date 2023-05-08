//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <stack>
#define ull unsigned long long
using namespace std;
string conf = "test";

class cPoint{
	public:
	int x;
	int y;
	
	cPoint(){
		x = 0;
		y = 0;
	}
	
	cPoint(const cPoint& tmp ){
		x = tmp.x;
		y = tmp.y;
	}
	
	cPoint(const int& nx, const int& ny){
		x = nx;
		y = ny;
	}
	
	cPoint(const string& s){
		int d = s.find(',');
		string sx = s.substr(0, d);
		string sy = s.substr(d + 1, s.length());
		x = stoi(s.substr(0, d));
		y = stoi(s.substr(d + 1, s.length()));
	}

	cPoint& operator-=(const cPoint& lhs){
		x -= lhs.x;
		y -= lhs.y;
		return *this;
	}

	cPoint& operator+=(const cPoint& lhs){
		x += lhs.x;
		y += lhs.y;
		return *this;
	}

	void normalize(){
		if(x != 0){
			x = x / abs(x);
		}
		if(y != 0){
			y = y / abs(y);
		}
	}
};

bool operator!=(const cPoint& lhs, const cPoint& rhs){
	return (lhs.x != rhs.x) || (lhs.y != rhs.y);
}

bool operator==(const cPoint& lhs, const cPoint& rhs){
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

cPoint& operator-(const cPoint& lhs, const cPoint& rhs){
	cPoint res(lhs);
	return res -= rhs;
}

cPoint& operator+(const cPoint& lhs, const cPoint& rhs){
	cPoint res(lhs);
	return res +=rhs;
}

class cTable{
	cPoint ofs;
	vector<vector<char>> leftv;
	vector<char> centerv;
	vector<vector<char>> rightv;
	cPoint pointer;

	int minX = 0;
	int maxX = 0;
	int maxY = 0;
	bool isInitialized = false;
	public:
	
	cTable(const cPoint& startofs):ofs(startofs){
		minX = startofs.x;
		maxX = startofs.x;
		maxY = startofs.y + 1;
		// ofs = startofs;
		pointer = startofs;
		isInitialized = true;
	}

	char& operator[](cPoint z){
		if( z.x < ofs.x )
			return leftv[ofs.x - z.x - 1][z.y];
		if( z.x > ofs.x )
			return rightv[z.x - ofs.x - 1][z.y];
		return centerv[z.y];		
	}

	ostream& printTo(ostream& os){
		for(int j = 0; j < maxY; j++){
			for(int i = minX ; i <= maxX ; i++){
				os << (*this)[cPoint(i, j)];
			}
			os << endl;		
		}
		return os;
	}

	void place(const cPoint& newPlace){
		pointer = newPlace;
	}
	
	void dotTo(const cPoint& newPlace, char pen){
		(*this)[newPlace] = pen;
	}

	void lineTo(const cPoint& newPlace, char pen){
		cPoint dir = newPlace - pointer;
		dir.normalize();
		for(cPoint x = pointer; x != newPlace; x +=dir){
			(*this)[x] = pen;
		}
		(*this)[newPlace] = pen;
		pointer = newPlace;
	}


	bool checkBorders(const cPoint& z){
		if(z.y >= maxY)
			return false;
		if(z.x > maxX)
			return false;
		if(z.x < minX)
			return false;
		return true;
	}

	void convexTo(const cPoint &c){
		if(!isInitialized){
			isInitialized = true;
			maxY = c.y  + 1;
			maxX = c.x;
			minX = c.x;
			ofs = c;
		}

		bool isUpdate = false;
		if(c.y >= maxY){
			isUpdate = true;
			maxY = c.y  + 1;
		}
		
		if(c.x > maxX){
			isUpdate = true;
			maxX = c.x;
		}

		if(c.x < minX){
			isUpdate = true;
			minX = c.x;
		}

		if(isUpdate){
			resize();
		}
	}

	void resize(){
		leftv.resize(ofs.x - minX);
		rightv.resize(maxX - ofs.x);
		centerv.resize(maxY, ' ');
		for(int i = 0; i < leftv.size(); i++){
			leftv[i].resize(maxY, ' ');
		}
		for(int i = 0; i < rightv.size(); i++){
			rightv[i].resize(maxY, ' ');
		}
	}
};

ostream& operator<<(ostream& os, cTable& tab){
	tab.printTo(os);
	return os;
}

class cSolve{
	int result = 0;
	//int trashx = 500, trashy = 0;
	cPoint src;
	cTable map;
	string outName;
	void print(){
		static bool firstTime = true;
		ofstream ofstr;
		if(firstTime)
			ofstr.open(outName + ".output", ios::binary);
		else
			ofstr.open(outName + ".output", ios::binary);
		firstTime = false;
		ofstr << map << endl;
		ofstr << "sand points stopped: " << result << endl;
		ofstr.close();
	}

	public:
	cSolve(const string& name, const cPoint& start):map(cPoint(start)){
		src = start;
		map.convexTo(src);
		outName = name.substr(0, name.find('.'));		
		ifstream ifstr(name, ios::binary);
		//vector<cPath> paths;
		
		string line;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			int interval = 0;
			interval = line.find('-');
			string sub = line.substr(0, interval - 1);
			map.convexTo(cPoint(sub));
			//cout << sub << endl;
			
			map.place(cPoint(sub));
			do{
				int prevInt = line.find('>', interval) + 2;
				interval = line.find('-', prevInt) ;
				sub = line.substr(prevInt, interval - prevInt - 1);
				//cout << sub << endl;
				map.convexTo(cPoint(sub));
				map.lineTo(cPoint(sub), '#');
			}while(interval != -1);
		}
		ifstr.close();
		map.dotTo(src, '+');
		print();
	}
	
	int  solve(){
		//simulate time!
		stack<cPoint> jet;
		cPoint dirs[3] = {
			{ 0, 1},
			{-1, 1},
			{ 1, 1}
		};
		jet.push(src);
		bool isFlawn = true;
		while(!jet.empty()){
			bool isRestart = false;
			cPoint iter = jet.top();
			if(!isFlawn){
				jet.pop();
				map[iter] = '~';
				continue;
			}
			// чекаю 
			for(int i = 0; i < 3; i++){
				if(map.checkBorders(iter + dirs[i])){
					if(map[iter + dirs[i]] == ' '){
						jet.push(iter+dirs[i]);
						isRestart = true;
						break;
					}
				}
				else{
					isFlawn = false;
					map[iter] = '~';
					jet.pop();
					isRestart = true;
					break;
				}
			}
			if(isRestart){
				continue;
			}

			map[iter] = '*';
			result++;
			for(int i = 0; i < 3; i++){
				if(map[iter + dirs[i]] == '~'){
					map[iter] = '~';
					result--;
					break;
				}
			}
			jet.pop();
		}
		print();
		return result;
	}
};

int main(void){
	int result;
	cPoint startPoint(500, 0);
	cSolve test("test.input", startPoint);
	result = test.solve();
	if(result != 24){
		cout << "test failed (24):" << result << endl;
	}
	cout << "test passed" << endl;

	cSolve cond("cond.input", startPoint);
	result = cond.solve();
	cout << "result: " << result << endl;
	//655 correct!
	return 0;
}
