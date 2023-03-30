//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/11
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#define ull unsigned long long

using namespace std;
class cMonkey{
	int id;
	int div;
	char op;
	int arg;
	vector<int> items;

	public:	
	int idYes;
	int idNo;

	int size(){return items.size();}
	bool checkAndThrow(int* ptr){
		int e = items.back();
		items.pop_back();
		if(e % div == 0)
			return true;
	}
	void operate(int i){
		switch(op){
			case '+':
				items[i] = items[i] + arg;
				break;
			case '-':
				items[i] = items[i] - arg;
				break;
			case '*':
				items[i] = items[i] * arg;
				break;
			case '^':
				items[i] = items[i] * items[i];
				break;
		}
	}

	cMonkey init(string idline, 
				string strartItemsLine, 
				string operationLine,
				string testLine,
				string idYesLIne,
				string idNoLine){
		stringstream(idline)>> id;
		stringstream(testLine)>>div;
		stringstream(idYesLIne)>>idYes;
		stringstream(idNoLine)>>idNo;

		//stringstream(strartItemsLine)>>items;
		//line = "Starting items: 59, 74, 65, 86"
		auto trash =   strartItemsLine.substr(
				startItemsLine.find(':')).
				split(','));
		// items = string.toInt(
		// 		strartItemsLine.substr(
		// 		startItemsLine.find(':')).
		// 		split(',').toInt()
		// );
		//неспешный гротескный смех, безумный и зловещий
		
		stringstream(operationLine.substr(
						operationLine.find(':'))
					)>>op>>arg;
	}
};

class cKeepAway{
	void operateAll(){
		for(int i = 0; i < tribe.size(); i++){
			for(int j = 0; j < tribe[i].size(); j++){
				tribe[i].operate(j);				
			}
		}
	}
	void throwItems(vector<vector<int>> &newItems){
		newItems.resize(tribe.size());

		int value;
		for(int i = 0; i < tribe.size(); i++){
			for(int j = 0; j < tribe[i].size(); j++){
				if(tribe[i].checkAndThrow(&value)){
					newItems[tribe[i].idYes].emplace_back(value);
				}
				else{
					newItems[tribe[i].idNo].emplace_back(value); 
				}
			}
		}
	}

	public:
	vector<cMonkey> tribe;
	void addMonkey(string line1,
				string line2,
				string line3,
				string line4,
				string line5,
				string line6){					
		tribe.emplace_back(line1,
				line2,
				line3,
				line4,
				line5,
				line6);
	}
	
	void move(){
		vector<vector<int>> newItems;
		operateAll();
		throwItems(newItems);
		//catch items
		for(int i = 0; i < tribe.size(); i++){
			tribe[i].items = newItems[i];
		}
	}
}KeepAway;

int readFileToSMt(){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	int id = 0;
	int status = ifstr.is_open();
	string trash;
	while(1){
		getline(ifstr, line);
		
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		//new monkey arrive!
		tribe.emplace_back();
		cMonkey* m = &(tribe.back());
		//1) get id
		stringstream s1(line);
		s1>>trash>>m->id;
		//2) get starting items
		getline(ifstr, line);
		stringstream st2(line);
		st2>>trash>>trash;
		while(1){
			int i = 0;
			st2>>i>>trash;
			if(i == 0)
				break;
			m->items.emplace_back(i);
		}
		//3) get operation what to do here?
		getline(ifstr, line);
		int i = line.find(':');
		m->opStr = line.substr(i);
		stringstream sop(line);
		//4) get divisor for test
		getline(ifstr, line);
		stringstream sdiv(line);
		sdiv>>m->div;
		//5) get monkey to throw if test Yes
		getline(ifstr, line);
		stringstream sidYes(line);
		sidYes>>m->idYes;
		//6) get monkey to throw if test No
		getline(ifstr, line);
		stringstream sidNo(line);
		sidNo>>m->idNo;
	}
	return 0;
}

int void solve(){
	//for(int i = 0; i < 20; i++)
	KeepAway.move();
	return accumulate(max, KeepAway.begin(), KeepAway.end());

}

int main(void){
	int result;
	readFileToSMt();
	solve();
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
