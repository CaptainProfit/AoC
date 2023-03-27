//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/5
//second part - moves not by one crate, but full count
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <list>
using namespace std;
#define ull unsigned long long

class cMove{
	public:
	int count;
	int src;
	int dest;
};

vector<list<char>> stacks;
vector<cMove> moves;

int readFileToSMt(){
	
	ifstream ifstr("y2022d5.input", ios::binary);
	//чтобы при чтении строки не пропускал пробелы
	// а фиг там, не разобрался.
	//ifstr>>skipws;

	int srreadLimit = 20000;
	//1) read initial stacks
	vector<string> store;
	while(srreadLimit-- > 0){
		string line;
		//чтобы при чтении строки не пропускал пробелы
		getline(ifstr, line);
		store.push_back(line);
		if(ifstr.eof()){
			ifstr.close();
			return -2;
		}
		if(line[1] == '1'){
			int size = line[line.length() - 2] - '0';
			stacks.resize(size);
			break;
		}
	}
	//1) parse initial stacks	
	//characters in positon 1,5,9,..., 4n+1
	for(int i = store.size() - 2; i >=0; i--){
		for(int j = 0; j< stacks.size(); j++){
			char sym = store[i][4*j+1];
			if( sym !=' '){
				stacks[j].push_back(sym);
			}

		}
	}
	//2) read moves
	while(srreadLimit-- > 0){
		int cnt = 0, src = 0, dest = 0;
		string trash;
		ifstr>>trash>>cnt>>trash>>src>>trash>>dest;
		dest--;src--;
		if(ifstr.eof()){
			break;
		}
		moves.push_back({cnt,src,dest});
	}
	ifstr.close();
	if(srreadLimit > 0)
		return 0;
	else 
		return -1;
}


int main(void){
	readFileToSMt();
	list<char> middle;
	for(int i = 0; i < moves.size(); i++){
		for(int j = 0; j < moves[i].count; j++){
			char tmp = stacks[moves[i].src].back();
			stacks[moves[i].src].pop_back();
			middle.push_back(tmp);
			//stacks[moves[i].dest].push_back(tmp);
		}
		for(int j = 0; j < moves[i].count; j++){
			char tmp = middle.back();
			middle.pop_back();			
			stacks[moves[i].dest].push_back(tmp);
		}
	}

	cout<<"input size is "<<moves.size()<<endl;
	cout<<"result is "<<endl; 
	for(int i = 0; i < stacks.size(); i ++)
		cout<< stacks[i].back();
	cout<<endl;
	return 0;
}
