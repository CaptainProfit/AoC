//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/13
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
#define ull unsigned long long

using namespace std;

class cPacket{
	list<cPacket> l;
	cPacket* parent = nullptr;
	int type = 2;
	int v = 0;
	bool isDigit(char c){
		return '0'<=c && c<= '9';			
	}

	public:
	cPacket (int i){
		v = i;
		type = 1;
	}
	cPacket (cPacket* p){
		parent = p;
		type = 2;
	}
	cPacket(string &line){
		cPacket* ptr = this;
		int num = 0;
		bool isNum = false;
		for(int i = 0; i < line.length();i++){
			switch(line[i]){
				case '[':
					// if(ptr == nullptr)
					// 	ptr = this;						
					// else
					{
						ptr->l.emplace_back(ptr);					
						ptr = &(ptr->l.back());
					}
					break;
				case ']':					
				case ',':
					if(isNum){
						ptr->l.emplace_back(num);
						isNum = false;
						num = 0;
					}
					if(line[i] == ']')
					{
						ptr = ptr->parent;
					}
					break;
				default:
					if(isDigit(line[i])){
						isNum = true;
						num*=10;
						num+=line[i] - '0';
					}
					break;
			}
		}
	}

	friend int operator!=(cPacket& left, cPacket& right) {
		//compare integers;
		if(left.type != 2 && right.type != 2){
			return right.v - left.v;			
		} 
		//compare lists
		if(left.type == 2 && right.type == 2){
			auto leftIt = left.l.begin();
			auto rightIt = right.l.begin();
			while(leftIt != left.l.end() && rightIt != right.l.end()){
				int diff = ((*leftIt) != (*rightIt));
				if(diff !=0){
					return diff;
				}
				leftIt++;
				rightIt++;
			}
			if(rightIt == right.l.end() 
			 && leftIt != left.l.end())
				return -1;
			if(rightIt != right.l.end() 
			 && leftIt == left.l.end())
				return 1;
			return 0;
		}
		//if(compare list and integer)
		if(left.type == 2 && right.type != 2){
			int diff = (*left.l.begin()) != right;
			if(diff == 0){
				auto it = left.l.begin();
				it++;
				if(it != left.l.end())
					return -1;
				else 
					return 0;
			}
			return diff;
		}
		if(left.type != 2 && right.type == 2){
			int diff = left != (*right.l.begin());
			if(diff == 0){
				auto it = right.l.begin();
				it++;
				if(it != right.l.end())
					return 1;
				else 
					return 0;
			}
			return diff;
		}
		return 0;
	}
};

static int sum = 0;
void solve(string &line1, string &line2){
	static int id = 1;
	cPacket pak1(line1);
	cPacket pak2(line2);
	int diff = (pak1 != pak2);
	if(diff > 0){
		sum+= id;
	}
	id++;
}

int readFileToSMt(){
	string line1;
	string line2;
	ifstream ifstr("cond.input", ios::binary);
	while(1){
		getline(ifstr, line1);
		getline(ifstr, line2);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		solve(line1, line2);
		getline(ifstr, line1);//to trash
	}
	return 0;
}



int main(void){
	int result;
	readFileToSMt();	
//	solve();
	cout<<"there are "<<sum<<" in result"<<endl;
	//5003 correct
		
	return 0;
}
