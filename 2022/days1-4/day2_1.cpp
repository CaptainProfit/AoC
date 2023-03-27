#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
using namespace std;
#define ull unsigned long long

//A - rock, B - paper, C - scissors
// A<B<C<A
map<char, char> mapXtoA;
void initMap(){
	mapXtoA.insert({'X', 'A'});
	mapXtoA.insert({'Y', 'B'});
	mapXtoA.insert({'Z', 'C'});
}

int roundResult1(pair<char, char> &input){
	input.second = mapXtoA[input.second];
	int score = input.second - 'A' + 1;
	int tmp = input.second - input.first;
	tmp %= 3;
	tmp += 3;
	tmp %= 3;
	switch(tmp){
		case 0: score += 3; break; //draw
		case 1: score += 6;	break; //win
		case 2: break;
	}
	return score;
}

int roundResult2(pair<char, char> &input){
	// enemyDecision = input.first
	// result = input.second
	// (myDecision - enemyDecision)%3 = result
	// 0 - draw
	// 1 - my win
	// 2 - my lose
	// hense myDecision	= (result + enemyDecision)%3

	input.second -= 'X';
	int score = input.second*3;
	int myDecision = (input.second + input.first - 'A')%3;//
	myDecision += myDecision?0:3;
	score += myDecision;
	return score;
}


int readFileToSMt(vector<pair<char, char>> &input){
	char a, b;
	ifstream ifstr("day2_1.input", ios::binary);
	int srreadLimit = 10000;
	while(srreadLimit-- > 0){
		ifstr>>a>>b;
		if(ifstr.eof()){
			break;
		}		
		input.push_back(pair<char, char> (a,b));
		//cout<<a<<" "<<b<<endl;
	}
	ifstr.close();
	if(srreadLimit > 0)
		return 0;
	else 
		return -1;
}

int main(void){
	initMap();

	vector<pair<char, char>> input;
	readFileToSMt(input);
	vector<unsigned long> packs;
	
	ull score = 0;
	int counter = 0;
	for(auto round:input){
		int roundScore = roundResult2(round);
		score += roundScore;
		//cout<<round.second<<" VS "<<round.first<<" >> "<<roundScore<<endl;
	}
	cout<<"input size is "<<input.size()<<endl;
	cout<<"result is "<<score<<endl; 
	return 0;
}
