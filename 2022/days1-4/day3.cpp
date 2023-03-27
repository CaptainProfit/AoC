#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
#define ull unsigned long long


int readFileToSMt(vector<string> &input){
	char a, b;
	ifstream ifstr("day3_1.input", ios::binary);
	int srreadLimit = 10000;
	while(srreadLimit-- > 0){
		string str;
		ifstr>>str;
		input.push_back(str);
		//cout<<a<<" "<<b<<endl;
		if(ifstr.eof()){
			break;
		}		
	}
	ifstr.close();
	if(srreadLimit > 0)
		return 0;
	else 
		return -1;
}

char findError(string &sack){
	map<char, int> exp;
	string compartment1 = sack.substr(0, sack.length()/2);
	string compartment2 = sack.substr(sack.length()/2, sack.length()/2);
	for(int i = 0; i < compartment1.length(); i++){
		exp[compartment1[i]] = 1;
	}
	for(int i = 0; i < compartment2.length(); i++){
		if(exp[compartment2[i]] == 1)
			return compartment2[i];		
	}
	return ' ';
}

char findCommon(string &sack1, string &sack2, string &sack3){
	map<char, int> exp;
	for(int i = 0; i < sack1.length(); i++){
		exp[sack1[i]] = 1;
	}

	for(int i = 0; i < sack2.length(); i++){
		if(exp[sack2[i]] == 1)
			exp[sack2[i]]++;
	}
	
	for(int i = 0; i < sack3.length(); i++){
		if(exp[sack3[i]] == 2)
			exp[sack3[i]]++;
	}	
	int counter = 0;
	char answer = ' ';
	for(auto par:exp){
		if(par.second == 3){
			answer = par.first;
			counter++;
		}			
	}
	if(counter == 1){
		return answer;
	}		
	else{
 		return '!';
	}		
}

int priority(char item){
	if(item >='a' && item <= 'z')
		return 1 + item - 'a';
	if(item >='A' && item <= 'Z')
		return 27 + item - 'A';
	return -1;
}

int main(void){
	vector<string> input;
	readFileToSMt(input);
	ull result = 0;
	for(int i = 0 ; i < input.size(); i += 3){
		char item = findCommon(input[i], input[i + 1], input[i + 2]);
		if(item ==' ' ){
			cout<<"something going wrong in findCommon"<<endl;
			return - 1;
		}
		if(item =='!' ){
			cout<<"something going overwhemling in findCommon"<<endl;
			return - 1;
		}
		result +=priority(item);
	}
	cout<<"input size is "<<input.size()<<endl;
	cout<<"result is "<< result <<endl; 
	return 0;
}
