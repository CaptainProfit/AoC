//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/15
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
// change - skan on square, find the unique not covered place
//TODO: unstable working - why?
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

#define ull unsigned long long
using namespace std;

class cInterval{
	long lcmp(long x, long y){
		if(x==y)
			return 0;
		return (x>y)? 1: -1;
	}

	public:
	long l;
	long r;
	cInterval(long a, long b){
		l = a;
		r = b;
	}

	long compare(cInterval& rhs){
		if(l == rhs.l){
			return lcmp(r,rhs.r);
		}
		return lcmp(l,rhs.l);
	}
};

inline bool operator>(cInterval& lhs, cInterval& rhs){ 	return rhs.compare(lhs) > 0;}
inline bool operator<(cInterval& lhs, cInterval& rhs){ 	return rhs.compare(lhs) < 0;}
inline bool operator>=(cInterval& lhs, cInterval& rhs){ return rhs.compare(lhs) >= 0;}
inline bool operator<=(cInterval& lhs, cInterval& rhs){ return rhs.compare(lhs) <= 0;}
inline bool operator!=(cInterval& lhs, cInterval& rhs){ return rhs.compare(lhs) != 0;}
inline bool operator==(cInterval& lhs, cInterval& rhs){ return rhs.compare(lhs) == 0;}


class cBorders{
	public:
	long l, r, u, d;
	bool initialized = false;
	long lineCond = 20;
	vector<long> exclude;

	void expand(long a, long b){
		if(!initialized){
			l=a;
			r=a;
			u=b;
			d=b;
			initialized = true;
			return;
		}
	
		if(l > a)
			l=a;
		if(r < a)
			r=a;
		if(u > b)
			u=b;
		if(d < b)
			d=b;

	}	
} borders;

class cSensor{
	long abs(long x){
		return x>0?x:-x;
	}

	// long dist(cPoint& a, cPoint& b){
	// 	return abs(a.x - b.x) + abs(a.y - b.y);
	// }
	public:
	long x;
	long y;
	long radius;
	cSensor(long sx, long sy, long bx, long by){
		x = sx;
		y = sy;
		radius = abs(sx - bx) + abs(sy - by);
	}

	cInterval projectY(long u){
		long d = radius - abs(u-y);
		if(d < 0){
			long redunt1 = borders.l - 1;
			long redunt2 = borders.l - 1;
		 	return cInterval(redunt1, redunt2);
		}
		long l = x - d;
		long r = x + d;
		//как нормально передать?
		cInterval* result = new cInterval(l, r);
		return *result;
	}
};

vector<cSensor> sensors;
vector<cSensor> beacons;
int readFileToSMt(){
	string line;
	ifstream ifstr("cond.input", ios::binary);
	borders.lineCond = 4000000;
	// ifstream ifstr("test.input", ios::binary);

	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;		
		}
		int i1 = line.find('=') + 1;
		int i2 = line.find(',', i1);
		int i3 = line.find('=', i2) + 1;
		int i4 = line.find(':', i3);
		int i5 = line.find('=', i4) + 1;
		int i6 = line.find(',', i5);
		int i7 = line.find('=', i6) + 1;
		int i8 = line.find(':', i7);

		long x1 = stol(line.substr(i1, i2 - i1));
		long y1 = stol(line.substr(i3, i4 - i3));
		long x2 = stol(line.substr(i5, i6 - i5));
		long y2 = stol(line.substr(i7, i8 - i7));
		borders.expand(x1,y1);
		borders.expand(x2,y2);
		sensors.emplace_back(x1, y1, x2, y2);
		beacons.emplace_back(x2, y2, x1, y1);
	}	
	return 0;
}
vector<cInterval> intervals;
tuple<long, long> solve(long lineId){
	// borders.exclude.clear();
	// for(auto b:beacons){
	// 	borders.exclude.push_back(b.x);
	// }
	// sort(borders.exclude.begin(), borders.exclude.end());
	// if(borders.exclude.size() != 0)	{
	// 	long r = borders.exclude[0];
	// 	for(int i = 1; i < borders.exclude.size();){
	// 		if(borders.exclude[i] == r ){
	// 			//oh gush, what a monster!
	// 			borders.exclude.erase(borders.exclude.begin() + i);
	// 		}
	// 		else{
	// 			r = borders.exclude[i];
	// 			i++;
	// 		}
	// 	}
	// }
	intervals.clear();
	cInterval iNone(borders.l - 1, borders.l - 1);
	for(auto b:sensors){
		if(b.projectY(lineId).compare(iNone) != 0)//thats fixed stability?
			intervals.emplace_back(b.projectY(lineId));
	}
	sort(intervals.begin(), intervals.end(), 
		[] (cInterval& l, cInterval& r){
			if(l.l != r.l)
				return l.l < r.l;
			return l.r >= r.r;
		});
	long acc = 0;
	long x = -1;
	long xb = -1;
	vector<cInterval>::iterator it = intervals.begin();
	int cnt = 0;
	for(; it != intervals.end(); it++){
		cnt++;
		if(it->compare(iNone) != 0)
			break;
	}

	for(; it != intervals.end(); it++){
		cnt++;
		//дешево и сердито ящитаю
		if(it->l <= xb && xb <= it->r){
			xb = it->r + 1;
		}
		// cout<<"checks "<<it->l<<"~"<<it->r<<endl;
		if(it->l > x){
			if(it->l > borders.lineCond){
				break;
			}
			x = it->l;
			acc++;
		}
		if(it->r > x){		
			if(it->r > borders.lineCond){
				acc+= borders.lineCond - x;
				break;
			}

			acc += it->r - x;
			x = it->r;
			// cout<<"x\tacc"<<endl;
			// cout<< x <<"\t"<< acc << endl;
			// cout<<"---------------"<<endl;
		}
		// cout<<"ALREADY"<<endl;
		// if marked from border.exclude - acc--;
	}
	
	// int exId = 0;
	// for(vector<cInterval>::iterator it = intervals.begin(); it != intervals.end(); it++){
	// 	for(;exId < borders.exclude.size(); exId++){
	// 		if(borders.exclude[exId] > borders.lineCond)
	// 			break;
	// 		if(it->l > borders.exclude[exId])
	// 			continue;
	// 		if(it->r < borders.exclude[exId])
	// 			break;
	// 		acc--;
	// 	}
	// }
	return make_tuple(acc, xb);
}

int main(void){
	cout<<"compiled as "<<__cplusplus<<" standard"<<endl;
	//201703L ?
	long long result;
	readFileToSMt();
	for(int y = 0; y <= borders.lineCond; y++){
		long res, x;
		tie(res, x) = solve(y);
		//cout<<"y="<< y <<", acc="<<res<<endl;
			
		if(res != borders.lineCond + 1){
			//line found 
			result = 4000000ll*x+y;
			cout<<x<<" "<<y<<endl;
			break;
		}
	}
	cout<<"there are "<<result<<" in result"<<endl;
	// hmmm test is even works!
	// 11016575214126 correct
	return 0;	
}
