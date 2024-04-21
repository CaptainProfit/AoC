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
#include <numeric>
#define ll long long
#define ull unsigned long long

using namespace std;
class cMonkey{
	int id = -1;
	int div = -1;
	char op;
	int arg = -1;
	int counter = -1;
	vector<ll> items;

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
		string sarg;
		stringstream(lines[2].substr(
					 lines[2].find(':') + 1))
					 >>trash //new
					 >>trash //=
					 >>trash //old
					 >>op>>sarg;
		if(sarg.compare("old") == 0){
			op = '^';
		}
		else{
			arg = stoi(sarg);
		}

		counter = 0;
	}
	int size(){
		return items.size();
	}
	void inspect(){
		for(int j = 0; j < items.size(); j++){				
			switch(op){
				case '+':
					items[j] += arg;
					break;
				case '-':
					items[j] -= arg;
					break;
				case '*':
					items[j] *= arg;
					break;
				case '^':
					items[j] *= items[j];
					break;			
		
			}
			//inspect doesnot hurt item - relax a bit
			items[j]/=3;
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
		items.clear();
	}

	void catchIt(vector<int> &newItems){
		//items.clear();
		items.insert(items.begin(),newItems.begin(),newItems.end());
	}

	int result() const{
		return counter;
	}

	void print(){
		for(int j = 0; j < items.size(); j++){
			cout<<items[j];
			if( j < items.size() - 1)
				cout<<", ";
		}
		cout<<endl;
	}
};

class cKeepAway{
	void inspectItems(){
		for(int i = 0; i < tribe.size(); i++){
			tribe[i].inspect();
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
		// макаки ходят по очереди, а не одновременно!
		//inspectItems();
		//throwItems(newItems);
		for(int i = 0; i < tribe.size(); i++){
			tribe[i].inspect();
			newItems.resize(tribe.size());
			tribe[i].checkAndThrow(newItems);
			catchItems(newItems);
			newItems.clear();
		}
	}
	int resultAcc(){
		int result = 0;
		sort(tribe.begin(), tribe.end(), 
			[] (cMonkey const &m1, cMonkey const &m2){
				return m1.result() > m2.result();}
			);		
		return tribe[0].result() * tribe[1].result();
	}
	ll calcItems(){
		return accumulate(tribe.begin(), tribe.end(), 0, 
			[&] (ll acc,  cMonkey m){	
				return acc+ m.size();}
			);
	}
	ll calcActivity(){
		return accumulate(tribe.begin(), tribe.end(), 0, 
			[&] (ll acc, cMonkey &m2){	
				return acc + m2.result();}
			);
	}
	void printTribe(){
		for(int i = 0 ; i < tribe.size(); i++){
			cout<<"monkey "<<i<<": ";
			tribe[i].print();
		}
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
	ll totalItems, totalActivity;
	totalItems = KeepAway.calcItems();
	cout<<"start"<<endl;
	KeepAway.printTribe();
	for(int i = 0; i < 20; i++){
		KeepAway.move();
		cout<<"round "<<i+1<<endl;
		KeepAway.printTribe();
		totalItems = KeepAway.calcItems();
		totalActivity = KeepAway.calcActivity();
	} 
	

	return KeepAway.resultAcc();
}

int main(void){
	int result;
	readFileToSMt();
	result = solve();
	//total 32 items. dropping 20 times
	// total count =
	cout<<"there are "<<result<<" in result"<<endl;
	//17820 too low
	//61005 correct
	//test passed
	return 0;
}