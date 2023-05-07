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

			//check overloading
			bool over = false;
			switch(op){
				case '+': value - rhs == lhs; break;
				case '*': value / rhs == lhs; break;
				case '-': value + rhs == lhs; break;
				case '~': value + lhs == rhs; break;
				case '/': value * rhs == lhs; break;
				case '\\':value * lhs == rhs; break;
				case '=': value == rhs; 
			}
			if(over){
				cout<<"overloading takes place!"<<endl;
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

	string toString(){
		
		if(isValue){
			return name + ": " + to_string(value);
		}
		else{
			return name + ": " + rhsName + " " + op + " " + lhsName;
		}
	}

};

ostream& operator<<(std::ostream& os, cMonkey& obj){
	os<<obj.toString();
	return os;
}

map<char, char> invOpsL = {
	{'+','-'},
	{'*','/'},
	{'-','+'},
	{'/','*'},
	{'=','='}
};

map<char, char> invOpsR = {
	{'+','~'},
	{'*','\\'},
	{'-','-'},
	{'/','/'},
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
		ofstream ofstr("solve22.dump");
		monkeys["root"].isVar = true;
		// альтернативный подход - выломать ветку в обратную сторону
		string name = "humn";
		monkeys[name].op = '=';
		monkeys[name].isValue = false;
		string gparent = name;
		while(name != "root"){
			string parent = monkeys[name].parent;
			string lhs = monkeys[name].lhsName;
			string rhs = monkeys[name].rhsName;
			char oldOp = monkeys[parent].op;
			char newOp;
			// заменяю  на 				
			if(monkeys[parent].lhsName == name){
				//если у предка слева
				// parent = name op other
				// lhs(x) = name(parent) invLop rhs
				// name = lhs(x) op rhs ->
				monkeys[name].lhsName = parent;
				monkeys[name].rhsName = monkeys[parent].rhsName;
				newOp = invOpsL[oldOp];
			}
			else{//if( monkeys[parent].rhsName == name)
				//если у предка справа
				// parent = other op name
				// name = lhs op rhs(x) ->
				// lhs(x) = lhs invRop name(parent)
				monkeys[name].rhsName = parent;
				monkeys[name].lhsName = monkeys[parent].lhsName;
				newOp = invOpsR[oldOp];
			}
			monkeys[name].parent = gparent;
			monkeys[name].op = newOp;

			cout<< monkeys[name]<<endl;
			gparent = name;
			name = parent;
		}

		if(monkeys[monkeys["root"].lhsName].isVar){
			monkeys["root"].lhsName = monkeys["root"].rhsName;
		}
		else{
			monkeys["root"].rhsName = monkeys["root"].lhsName;
		}
		cout<< monkeys["root"]<<endl;

		// а теперь как и было.
		stack<string> varQueue;
		varQueue.push("humn");
		while(!varQueue.empty()){
			string name = varQueue.top();
			if(monkeys[name].isValue){
				ofstr << name << " = " <<  monkeys[name].value << endl;
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
				ofstr << name << " = " << lhs << " " << monkeys[name].op << " " <<  rhs << endl;
				ofstr << "\t" << monkeys[name].value << " = " << monkeys[lhs].value << " " << monkeys[name].op << " " <<  monkeys[rhs].value << endl;
				cout<<name<<" = "<<monkeys[name].value<<endl;
			}
		}
		ofstr.close();		
		cout<<"humn calculated: "<<monkeys["humn"].isValue<<endl;
		return monkeys["humn"].value;
	}
	
	long long solve1(){
		
		stack<string> varQueue;
		ofstream ofstr("solve11.dump");
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
				//ofstr << name << " = " <<  monkeys[name].value << endl;
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
					ofstr << name << " = " << lhs << " " << monkeys[name].op << " " <<  rhs << endl;
					ofstr << "\t" << monkeys[name].value << " = " << monkeys[lhs].value << " " << monkeys[name].op << " " <<  monkeys[rhs].value	 << endl;
					ofstr << name << " = " << monkeys[name].value << endl;
				}
				else{
					// 
					varQueue.pop();
					monkeys[name].backexec(monkeys[lhs].value, 
											monkeys[rhs].value, 
											monkeys[parent].value, 
											monkeys[lhs].isVar);
					if(monkeys[lhs].isVar){
						ofstr << lhs << " = " << name << " " << invOpsL[monkeys[name].op] << " " <<  rhs << endl;
						ofstr << "\t" << monkeys[name].value << " = " << monkeys[parent].value << " " << invOpsL[monkeys[name].op] << " " <<  monkeys[rhs].value	 << endl;
						ofstr << lhs << " = " << monkeys[name].value << endl;
					}
					else{
						ofstr << rhs << " = " << lhs << " " << invOpsR[monkeys[name].op] << " " <<  name << endl;
						ofstr << "\t" << monkeys[name].value << " = " << monkeys[lhs].value << " " << invOpsR[monkeys[name].op] << " " <<  monkeys[parent].value	 << endl;
						ofstr << rhs << " = " << monkeys[name].value << endl;
					}
				}
				
				cout<<name<<" = "<<monkeys[name].value<<endl;
			}
		}
		ofstr.close();
		// cout<<"humn calculated: "<<monkeys[monkeys["humn"].parent].isValue<<endl;
		return monkeys[monkeys["humn"].parent].value;
	}
};

int main(void){
	long long result;
	cSolve test1("test.input");
	cSolve cond1("cond.input");
	cSolve test2("test.input");
	cSolve cond2("cond.input");
	// bingo! операции и правда строят бинарные деревья, чуть легче.
	// значит по простому - считаю выражение в одной из веток и поднимаю его вверх.
	// 
	cout<<test1.countVars()<<endl;
	cout<<cond1.countVars()<<endl;
	cout<<test2.countVars()<<endl;
	cout<<cond2.countVars()<<endl;
	result = test1.solve1();
	result = test2.solve2();
	if(result != 301){
		cout<<"test failed "<<result<<"(152)"<<endl;
		return -1;
	}
	cout<<"test passed"<<endl;
	result = cond1.solve1();
	result = cond2.solve2();
	if(result != 3343167719435){
		cout<<"cond failed "<<result<<"(3343167719435)"<<endl;
		return -1;
	}
	cout<<"cond passed"<<endl;	
	return 0;
	// 3343167719435 correct!
}
