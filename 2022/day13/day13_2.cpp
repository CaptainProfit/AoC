//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/13
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
// now sort them all!
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <iterator>
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
	cPacket(const string &line){
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
		if(isNum){
			ptr->l.emplace_back(num);
			isNum = false;
			num = 0;
		}
	}

	friend int operator!=(cPacket& left, cPacket& right) {
		//compare integers; typeProd = 1
		if(left.type != 2 && right.type != 2){
			return right.v - left.v;			
		} 
		//compare lists; typeProd = 4
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
		//if(compare list and integer); typeProd = 2
		if(left.type == 2 && right.type != 2){
			if(left.l.empty())
				return 1;
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
		else return -(right != left);
		return 0;
	}

	friend ostream& operator<<(ostream& os, const cPacket &pak){
		if(pak.type != 2){
			os<<pak.v;
		}
		if(pak.type == 2){
			os<<'[';
			for(list<cPacket>::const_iterator it = pak.l.begin(); it != pak.l.end();){
				os<<*it;
				it++;
				if(it != pak.l.end()){
					os<<',';
				}
			}	
			os<<']';
		}
		return os;
	}
};

vector<cPacket> packets;
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
		packets.emplace_back(line1.substr(1, line1.length() - 3));
		packets.emplace_back(line2.substr(1, line2.length() - 3));
		//solve(line1, line2);
		getline(ifstr, line1);//to trash
	}
	return 0;
}

void printPackets(){
	for(auto packet:packets){
		cout<<packet<<endl;
	}
}
 
long solve(){
	packets.emplace_back("[2]");
	packets.emplace_back("[6]");
	cPacket startPacket("[2]");
	cPacket finishPacket("[6]");
	sort(packets.begin(), packets.end(), [](cPacket &left, cPacket &right){
		return (left != right) > 0;
	});
	printPackets();
	int cnt = 0;
	int idStart = 0;
	int idFinish = 0;
	for(int i = 0; i < packets.size(); i++){
		if((packets[i] != startPacket) == 0){
			idStart = i + 1;
			cnt++;
		}
		if((packets[i] != finishPacket) == 0){
			idFinish = i + 1;
			cnt++;
		}
	}
	// cPacket pak1(line1);
	// cPacket pak2(line2);
	// int diff = (pak1 != pak2);
	// if(diff > 0){
	// 	sum+= id;
	// }
	// id++;
	cout<<"test value 2="<<cnt<<endl;
	return idStart*idFinish;
}

int main(void){
	long result;
	readFileToSMt();	
	result = solve();
	cout<<"there are "<<result<<" in result"<<endl;
	//13182 too low, sort not sorted fully
	// check comparing number with empty list
	// check comparing number with list of empty lists
	// 20280 correct
	return 0;
}
