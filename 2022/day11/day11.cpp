//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/11
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#define ull unsigned long long

using namespace std;
class cMonkey{
	int id = -1;
	int div = -1;
	char op;
	int arg = -1;
	int counter = -1;
	vector<int> items;

	public:	
	int idYes = -1;	
	int idNo = -1;
	cMonkey(string lines[]){
		string trash;
		stringstream(lines[0])>>trash>> id;
		stringstream(lines[3].substr(
					 lines[3].find(':') + 1))>>trash>>trash>>div;
		stringstream(lines[4].substr(
					 lines[4].find(':') + 1))>>trash>>trash>>trash>>idYes;
		stringstream(lines[5].substr(
					 lines[5].find(':') + 1))>>trash>>trash>>trash>>idNo;

		//stringstream(startItemsLine)>>items;
		//line = "Starting items: 59, 74, 65, 86"
		stringstream x(lines[1].substr(
			   lines[1].find(':') + 1));
		while(1){
			int i = -1;
			x>>i>>trash;
			if(i<0){
				break;
			}
			items.emplace_back(i);
		}		
		stringstream(lines[2].substr(
					 lines[2].find(':') + 1))
					 >>trash //new
					 >>trash //=
					 >>trash //old
					 >>op>>arg;
		counter = 0;
	}

	void operate(){
		for(int j = 0; j < items.size(); j++){				
			switch(op){
				case '+':
					items[j] = items[j] + arg;
					break;
				case '-':
					items[j] = items[j] - arg;
					break;
				case '*':
					items[j] = items[j] * arg;
					break;
				case '^':
					items[j] = items[j] * items[j];
					break;			
			}
		}
	}

	bool check(int i){
		return items[i]% div == 0;
	}

	void checkAndThrow(vector<vector<int>> &newItems){
		for(int j = 0; j < items.size(); j++){
			if(check(j)){
				newItems[idYes].emplace_back(items[j]);
			}
			else{
				newItems[idNo].emplace_back(items[j]); 
			}
			counter++;
		}
	}

	void catchIt(vector<int> &newItems){
		items.clear();
		items.insert(items.begin(),newItems.begin(),newItems.end());
	}

	int result(){
		return counter;
	}
};

class cKeepAway{
	void operateItems(){
		for(int i = 0; i < tribe.size(); i++){
			tribe[i].operate();
		}
	}
	void throwItems(vector<vector<int>> &newItems){
		newItems.resize(tribe.size());
		for(int i = 0; i < tribe.size(); i++){
			tribe[i].checkAndThrow(newItems);
		}
	}
	void catchItems(vector<vector<int>> &newItems){
		for(int i = 0; i < tribe.size(); i++){
			//tribe[i].items = newItems[i];
			tribe[i].catchIt(newItems[i]);
		}		
	}
	public:
	vector<cMonkey> tribe;
	void addMonkey(string lines[]){					
		tribe.emplace_back(lines);
	}
	
	void move(){
		vector<vector<int>> newItems;
		operateItems();
		throwItems(newItems);
		catchItems(newItems);
	}
	int resultAcc(){
		int result = 0;
		accumulate(tribe.begin(), tribe.end(), result,
			 int[] (const cMonkey &m1, const cMonkey &m2){
			return max(m1.result(), m2.result());});
	}
}KeepAway;

int readFileToSMt(){
	ifstream ifstr("cond.input", ios::binary);
	int id = 0;
	int status = ifstr.is_open();
	string trash;
	while(1){
		string lines[7];
		for(int i = 0; i < 7; i++)
			getline(ifstr, lines[i]);
		//new monkey arrive!
		KeepAway.addMonkey(lines);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
	}
	return 0;
}

int solve(){
	//for(int i = 0; i < 20; i++)
	KeepAway.move();	
	return KeepAway.resultAcc();
}

int main(void){
	int result;
	readFileToSMt();
	result = solve();
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}