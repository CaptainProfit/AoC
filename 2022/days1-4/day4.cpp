#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
#define ull unsigned long long


int readFileToSMt(vector<pair<pair<int, int>,pair<int,int>>> &input){
	
	ifstream ifstr("day4_1.input", ios::binary);
	int srreadLimit = 10000;
	while(srreadLimit-- > 0){
		int a1, a2, b1, b2;
		char c;
		string str;
		
		ifstr>>a1;
		ifstr>>c;
		ifstr>>a2;
		ifstr>>c;
		ifstr>>b1;
		ifstr>>c;
		ifstr>>b2;
		input.push_back({{a1, a2},{b1, b2}});
		if(ifstr.eof()){
			break;
		}
		//cout<<a<<" "<<b<<endl;
	}
	ifstr.close();
	if(srreadLimit > 0)
		return 0;
	else 
		return -1;
}
bool isContains(pair<int, int> A,pair<int, int> B){
	return (A.first >= B.first && A.second <= B.second);		
}

bool isOverlap(pair<int, int> A,pair<int, int> B){
	if(isContains(A,B))
		return true;
	if (A.first <= B.first && A.second >= B.first)
		return true;
	if (A.first <= B.second && A.second >= B.second)
		return true;
	return false;
}


int main(void){
	vector<pair<pair<int, int>,pair<int,int>>> input;
	readFileToSMt(input);
	ull result = 0;
	for(auto pairs:input){
		if(isOverlap(pairs.first, pairs.second) || isContains(pairs.second, pairs.first)){
			result++;
		}
	}
	
	cout<<"input size is "<<input.size()<<endl;
	cout<<"result is "<< result <<endl; 
	return 0;
}
