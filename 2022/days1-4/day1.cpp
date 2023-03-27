
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
//мораль: если бы я паки клал в кучу - я бы сразу получил И ответ на первую И малые изменения на вторую.
using namespace std;

int main(void){
	ifstream input("day1_1.input", ios::binary);
	vector<unsigned long> packs;
	std::stringstream ss;
	char str[8];
	packs.push_back(0);

	int srreadLimit = 10000;
	while(srreadLimit-- > 0){
		input.getline(str, 7, '\n');
		if(input.eof()){
			break;
		}
		if(str[0] == '\r'){
			packs.push_back(0);
		}
		else{
			int value;
			ss<<str;
			ss>>value;
			packs.back() += value;
		}
	}
	vector<unsigned long> results(3,0);
	int id = 0;
	int maxIds[3] =  {0, 0, 0};
	sort(results.begin(), results.end(), [](unsigned long const &x, unsigned long const &y){return x > y;});
	//обработка
	for(auto pack: packs){
		id++;
		for(int i = 0; i < 3; i++){
			if(results[i] < pack){
				for(int j = 3-1-1; j >= i; j--){
					results[j + 1] = results[j];
					maxIds[j + 1] = maxIds[j];
				}
				results[i] = pack;
				maxIds[i] = id;
				break;
			}
		}
	}
	input.close();
	if(srreadLimit <0){
		cout<<"error, readLimit exceeded"<<endl;
		return -1;
	}
	int result = 0;
	for(int i = 0; i < 3; i++){
		result+= results[i];
	}
	cout<<"result is "<<result<<endl; 
	return 0;
}
