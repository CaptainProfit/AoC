//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <list>
#include <chrono>
#include <map>
#define ull unsigned long long

using namespace std;

class cSolve{
	list<pair<int, int>> packet;
	int N;
	pair<int, int>& operator[](int i) {
        auto it = packet.begin();
        while (i-- > 0) {
            it++;
        }
        return *it;
    }

	void print(int it = -1){
        cout << "[";
		for (auto i = packet.begin(); i != packet.end(); i++) {
            if (i != packet.begin()) {
                if(i->first == it){
                    cout << "<";
                }
                else{
                    cout << ", ";
                }
            }
            cout << i->second;
		}
		cout << "]" << endl;
	}

	list<pair<int, int>>::iterator find(int queNum){
		for (auto it = packet.begin(); it != packet.end(); it++) {
			if (it->first == queNum){
				return it;
			}
		}
		return packet.end();
	}

	void move(int id){
        list<pair<int, int>>::iterator it = find(id);
        pair<int, int> el = *it;
		int d = it->second;
		d %= N - 1;
		d += N - 1;
		d %= N - 1;
        if (d == 0) {
            return;
        }
        
		list<pair<int, int>>::iterator new_it = it;
        for (int i = 0; i <= d; i++) {
            new_it++;
            if (new_it == packet.end()) {
                new_it = packet.begin();
            }
        }

        packet.erase(it);
        packet.insert(new_it, el);
        //print();
	}

	public:	
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		int it = 0;
		for(getline(ifstr, line); !ifstr.eof() && !line.empty(); getline(ifstr, line)){
			int value = stoi(line);
			packet.push_back(pair<int, int>(it, value));
			it++;
		}
		ifstr.close();
		N = packet.size();
	}
	
	int mix(){
		//математическая формула из условия задачи
		int zid = -1;
		for(int i = 0; i < N; i++){
			if((*this)[i].second == 0){
				zid = i;
				break;
			}
		}

		int id1 = (zid + 1000) % N;
		int id2 = (zid + 2000) % N;
		int id3 = (zid + 3000) % N;
		return (*this)[id1].second + 
				(*this)[id2].second +
				(*this)[id3].second;
	}

	int solve(){
        //cout << "start:" << endl;
        for(int i = 0; i < N; i++){
			move(i);
            //if (i%100 == 0) {
            //    print();
            //}
		}
        //cout << "finish:" << endl;
        
		//print();
		return mix();
	}
};

int main(void){	
	long long result;
	const auto tstart = chrono::steady_clock::now();
	/*cSolve test("test.input");
	test.solve();
	result = test.mix();
	
	if(result != 3){
		cout << "test failed (3):" << result <<endl;
		return -2;
	}
	cout << "test passed " << result << endl;*/

	cSolve cond("cond.input");
	result = cond.solve();
	cout << "result:" << result << endl;
	cout << "(7153)" << endl;
	const auto tend = chrono::steady_clock::now();
	const auto interval = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
	std::cout << "time passed: " << std::chrono::microseconds(interval).count() << "us" << std::endl;
	// 7153 correct
	// 60388us
	return 0;
}
