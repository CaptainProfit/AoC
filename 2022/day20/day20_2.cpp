//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <chrono>
#include <fstream>
#include <iostream> 
#include <string>
#include <vector>
#include "orderedTree.hpp"

using namespace std;
//template class cContainer<int>;

class cSolve{
    cContainer storage;
    const ll decrKey = 811589153;
    public:
    cSolve(const string& name){
        string line;
        ifstream ifstr(name, ios::binary);
        int counter = 0;
        for(getline(ifstr, line); !ifstr.eof() && !line.empty(); getline(ifstr, line)){
            T value = stol(line);
            value *= decrKey;
            storage.push_back(value);
            //storage.updateOffset(value);
            //storage.print();
            assert(counter++ < 10000);
        }
        ifstr.close();
    }

    void solve(){
        int d = storage.sizef();
        //cout << "initial position" << endl;
        //storage.print();
        for(int j = 0; j < 10; j++){
            for(int i = 0; i < d; i++){
                storage.move(i);
                //cout << "after " << i << "th move" << endl;
                //storage.print();
            }
        }
    }

    ll mix(){
        //математическая формула из условия задачи
        storage.print();
        int zid = -1;
        for (int i = 0; i < storage.sizef(); i++) {
            if (storage[i]->getValue() == 0) {
                zid = i;
                break;
            }
        }

        int d = storage.sizef();
        int id1 = (zid + 1000)%d;
        int id2 = (zid + 2000)%d;
        int id3 = (zid + 3000)%d;
        return storage[id1]->getValue() + 
                storage[id2]->getValue() +
                storage[id3]->getValue();
    }
};

int main(void){	
    int result;
    const auto tstart = chrono::steady_clock::now();
    cSolve test("test.input");
    test.solve();
    result = test.mix();
    
    if(result != 1623178306){
        cout << "test failed (1623178306):" << result <<endl;
        //return -2;
    }
    cout<<"test passed "<< result << endl;
    cSolve task("cond.input");
    task.solve();
    result = task.mix();
    const auto tend = chrono::steady_clock::now();
	const auto interval = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
    if(result != 6146976244822){
        cout << "cond failed (6146976244822):" << result <<endl;
        //return -2;
    }
    cout<<"cond passed: "<< result << endl;
	std::cout << "time passed: " << std::chrono::microseconds(interval).count() << "us" << std::endl;
    return 0;
}
