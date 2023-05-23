//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#define ll long long

using namespace std;

class cSolve{
	vector<pair<int, ll>> packet;
	int N;
	const ll decrKey = 811589153;
	
	void print(int it = -1){
		for(int i = 0; i < packet.size(); i++){
			cout << packet[i].second;
			if(packet[i].first == it){
				cout << "<";
			}
			else{
				cout << " ";
			}
		}
		cout << endl;
	}

	int find(int queNum){
		for(int i = 0; i < packet.size(); i++){
			if(packet[i].first == queNum){
				return i;
			}
		}
		return -1;
	}

	void move(int it){
		ll d = packet[it].second;
		d %= N-1;
		d += N - 1;
		d %= N-1;
				
		ll it2 = it + d;
		it2 %= N;

		for(ll i = it; i != it2; ++i %= N){
			swap(packet[i], packet[(i + 1) % N]);
			// cout << "\t";
			// print();
		}
	}

	public:	
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		int it = 0;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			ll value = stol(line);
			value *= 811589153;
			packet.push_back(pair<int, ll>(it, value));
			it++;
		}
		ifstr.close();		
		N = packet.size();
	}
	
	ll mix(){
		//математическая формула из условия задачи
		int zid = -1;
		for(int i = 0; i < N; i++){
			if(packet[i].second == 0){
				zid = i;
				break;
			}
		}

		int id1 = (zid + 1000) % N;
		int id2 = (zid + 2000) % N;
		int id3 = (zid + 3000) % N;
		return packet[id1].second + 
				packet[id2].second +
				packet[id3].second;
	}

	ll solve(){
		for(int j = 0; j < 10; j++){
			for(int i = 0; i < N; i++){
				// print(i);
				move(find(i));
			}
		}
		// print();
		return mix();
	}
};

int main(void){	
	ll result;
	const auto tstart = chrono::steady_clock::now();
	cSolve test("test.input");
	test.solve();
	result = test.mix();
	
	if(result != 1623178306){
		cout << "test failed (1623178306):" << result <<endl;
		return -2;
	}
	cout << "test passed " << result << endl;

	cSolve cond("cond.input");
	result = cond.solve();
	if(result != 6146976244822){
		cout << "cond failed (6146976244822):" << result <<endl;
		return -2;
	}
	cout << "result:" << result << endl;
	const auto tend = chrono::steady_clock::now();
	const auto interval = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
	std::cout << "time passed: " << std::chrono::microseconds(interval).count() << "us" << std::endl;
	// 6146976244822 correct
	// 5761051us
	return 0;
}
