//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/x
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <stack>
#include <map>
#include <string>
#include <algorithm>

#define ull unsigned long long

using namespace std;

class cMonkey{
	string name;
	char op;

	public:
	bool isValue;
	long long value;
	string lhsName, rhsName;
	
	cMonkey(){
		cout<<"why i am here?"<<endl;
	}
	cMonkey(string& line){
		// слово1: слово2 опер слово3
		// слово1: число
		int i1 = line.find(':');
		name = line.substr(0, i1);
		char x = line[i1 +2];
		if(isdigit(x)){
			isValue = true;
			string valueStr = line.substr(i1+1);
			value = stoi(valueStr);
		}
		else{
			isValue = false;
			int i3 = line.find(" ");
			int i4 = line.find(" ", i3 + 1);
			int i5 = line.find(" ", i4 + 1);
			int i6 = line.find("\r", i5 + 1);
			op = line[i4 + 1];
			lhsName = line.substr(i3 + 1, i4 - i3 - 1);
			rhsName = line.substr(i5 + 1, i6 - i5 -1);
			//rhs.resize(4);
		}
	}

	void exec(long long lhs, long long rhs){
		if(!isValue){
			isValue = true;
			switch(op){
				case '-': value = lhs + rhs; break;
				case '+': value = lhs + rhs; break;
				case '*': value = lhs + rhs; break;
				case '/': value = lhs + rhs; break;
			}
		}
	}
	
};

class cSolve{
	map<string, cMonkey> monkeys;
	public:
	
	cSolve(const string& filename){	
		string line;
		ifstream ifstr(filename, ios::binary);
		getline(ifstr, line);
		do{
			monkeys.emplace(line.substr(0,4), cMonkey(line));
			getline(ifstr, line);
		}while(!ifstr.eof());
		ifstr.close();
	}

	long long solve(){
		
		stack<string> varQueue;
		varQueue.push("root");
		while(!varQueue.empty()){
			string name = varQueue.top();
			if(monkeys[name].isValue){
				varQueue.pop();
			}
			else{
				string lhs = monkeys[name].lhsName;
				string rhs = monkeys[name].rhsName;
				if(!monkeys[lhs].isValue){
					varQueue.push(lhs);
					continue;
				}
				if(!monkeys[rhs].isValue){
					varQueue.push(rhs);
					continue;
				}
				monkeys[name].exec(monkeys[lhs].value, monkeys[rhs].value);
				cout<<name<<" = "<<monkeys[name].value<<endl;
			}
		}
		cout<<"root calculated: "<<monkeys["root"].isValue<<endl;
		return monkeys["root"].value;
	}
};

int main(void){
	long long result;
	cSolve test("test.input");
	result = test.solve();
	if(result != 152){
		cout<<"test failed "<<result<<"(152)"<<endl;
		return -1;
	}
	cout<<"test passed"<<endl;
	cSolve cond("cond.input");
	result = cond.solve();
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
