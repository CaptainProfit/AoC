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

	public:
	char op;
	bool isValue;
	bool isVar = false;
	bool flag = false;
	long long value;
	string lhsName, rhsName, parent;
	
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
			if(name == "humn"){
				isVar = true;
			}
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
			if(name == "root"){
				op = '=';
			}

			lhsName = line.substr(i3 + 1, i4 - i3 - 1);
			rhsName = line.substr(i5 + 1, i6 - i5 -1);
			//rhs.resize(4);
		}
	}

	bool exec(long long lhs, long long rhs){
		if(!isValue){
			isValue = true;
			switch(op){
				case '+': value = lhs + rhs; break;
				case '*': value = lhs * rhs; break;
				case '-': value = lhs - rhs; break;
				case '~': value = rhs - lhs; break;
				case '/': value = lhs / rhs; break;
				case '\\':value = rhs /lhs; break;
				case '=': value = rhs; 
			}
		}
		return false;
	}

	long long backexec(long long &lhs, long long &rhs, long long &parent, bool ascendToLeft){
		// if(!isValue){
		// 	isValue = true;
		// }
		if(ascendToLeft){
			switch(op){
				case '+': value = parent - rhs; break;
				case '*': value = parent / rhs; break;
				case '=': value = rhs; 			break; 
				case '-': value = parent + rhs; break;
				case '/': value = parent * rhs; break;
			}
		}
		else{
			switch(op){
				case '+': value = parent - lhs; break;
				case '*': value = parent / lhs; break;
				case '=': value = lhs; 			break;
				case '-': value = lhs - parent; break;
				case '/': value = lhs / parent; break;
			}
		}		
		return false;
	}	
};

map<char, char> invOpsR = {
	{'+','-'},
	{'*','/'},
	{'-','+'},
	{'/','*'},
	{'=','='}
};
map<char, char> invOpsL = {
	{'+','~'},
	{'*','\\'},
	{'-','+'},
	{'/','*'},
	{'=','='}
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
	
	int countVars(){
		// обхожу ветки графа в поисках var вконце и считаю такие ветки. 
		// если будет только одна - всё упростится.
		int counter = 0;
		int nodeCounter = 1;
		bool isTree = true;
		stack<string> varQueue;
		varQueue.push("root");

		while(!varQueue.empty()){
			string name = varQueue.top();

			if(name == "humn"){
				counter++;
				// monkeys[name].flag = true;
				varQueue.pop();
				continue;
			}

			if(monkeys[name].isValue){
				// monkeys[name].flag = true;
				varQueue.pop();
				continue;
			}
			string lhs = monkeys[name].lhsName;
			if(!monkeys[lhs].flag){
				varQueue.push(lhs);
				monkeys[lhs].flag = true;
				monkeys[lhs].parent = name;
				nodeCounter++;
				continue;
			}
			string rhs = monkeys[name].rhsName;
			if(!monkeys[rhs].flag){
				varQueue.push(rhs);
				monkeys[rhs].flag = true;
				monkeys[rhs].parent = name;
				nodeCounter++;
				continue;
			}
			varQueue.pop();
		}
		monkeys["root"].parent = "root";
		return nodeCounter;
	}

	long long solve2(){
		for(string x = "humn"; x != "root" ; x = monkeys[x].parent){
			monkeys[x].isVar = true;
		}
		monkeys["root"].isVar = true;
		// альтернативный подход - выломать ветку в обратную сторону
		string name = "humn";
		monkeys[name].op = '=';
		monkeys[name].isValue = false;
		while(name != "root"){
			string parent = monkeys[name].parent;
			string lhs = monkeys[name].lhsName;
			string rhs = monkeys[name].rhsName;
			// заменяю  на 
			if(lhs != "" && monkeys[lhs].isVar){
				monkeys[name].lhsName = parent;
			}
			else{
				monkeys[name].rhsName = parent;
				if(name == "humn"){
					monkeys[name].lhsName = parent;
				}
			}
			char oldOp = monkeys[name].op;
			char newOp;
			if(monkeys[parent].lhsName == name){
				//если у предка слева
				newOp = invOpsL[oldOp];
			}
			else{
				//если у предка справа
				newOp = invOpsR[oldOp];
			}
			monkeys[name].op = newOp;
			name = parent;
			//теряю инфу про родителя но да и не нужна она
		}

		if(monkeys[monkeys["root"].lhsName].isVar){
			monkeys["root"].lhsName = monkeys["root"].rhsName;
		}
		else{
			monkeys["root"].rhsName = monkeys["root"].lhsName;
		}
		
		// а теперь как и было.
		stack<string> varQueue;
		varQueue.push(monkeys["humn"].lhsName);
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
		cout<<"root calculated: "<<monkeys["humn"].isValue<<endl;
		return monkeys["humn"].value;
	}
	
	long long solve(){
		
		stack<string> varQueue;
		// помечаю ветку с переменной
		for(string x = "humn"; x != "root" ; x = monkeys[x].parent){
			varQueue.push(x);	
			monkeys[x].isVar = true;
		}
		varQueue.push("root");
		monkeys["root"].isVar = true;

		while(!varQueue.empty()){
			string name = varQueue.top();
			string lhs = monkeys[name].lhsName;
			string rhs = monkeys[name].rhsName;
			string parent = monkeys[name].parent;
			if(monkeys[name].isValue ){
				varQueue.pop();
			}
			else{
				
				// если левая ветка не посчитана и не относится к переменной
				if( !monkeys[lhs].isValue && !monkeys[lhs].isVar ){
					varQueue.push(lhs);
					continue;
				}

				// если правая ветка не посчитана и не относится к переменной
				if( !monkeys[rhs].isValue && !monkeys[rhs].isVar ){
					varQueue.push(rhs);
					continue;
				}

				if( !monkeys[name].isVar){
					monkeys[name].exec(monkeys[lhs].value, monkeys[rhs].value);
				}
				else{
					// 
					varQueue.pop();
					monkeys[name].backexec(monkeys[lhs].value, 
					  						monkeys[rhs].value, 
											monkeys[parent].value, 
											monkeys[lhs].isVar);
				}
				cout<<name<<" = "<<monkeys[name].value<<endl;
			}
		}
		// cout<<"humn calculated: "<<monkeys[monkeys["humn"].parent].isValue<<endl;
		return monkeys[monkeys["humn"].parent].value;
	}
};

int main(void){
	long long result;
	cSolve test("test.input");
	cSolve cond("cond.input");
	// bingo! операции и правда строят бинарные деревья, чуть легче.
	// значит по простому - считаю выражение в одной из веток и поднимаю его вверх.
	// 
	cout<<test.countVars()<<endl;
	cout<<cond.countVars()<<endl;
	result = test.solve2();
	if(result != 301){
		cout<<"test failed "<<result<<"(152)"<<endl;
		return -1;
	}
	cout<<"test passed"<<endl;
	result = cond.solve2();
	if(result != 3343167719435){
		cout<<"cond failed "<<result<<"(3343167719435)"<<endl;
		return -1;
	}
	cout<<"cond passed"<<endl;	
	return 0;
	// 3343167719435 correct!
}
