//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
// #include <cassert>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#define ull unsigned long long
using namespace std;
#define NDEBUG
#include "implicit_tree.cpp"

class cSolve{
    vector<InnerImplicitTree::node_iterator> storage;
    int size;

    void move(int i){
        InnerImplicitTree::node_iterator iter = storage[i];
        int index = GetIteratorPosition(iter);
        Type el = Find(index);
        Type d = el;
        d %= size - 1;
        d += size - 1;
        d %= size - 1;
        int new_index = index + d;
        new_index %= size - 1;
        Remove(index);
        Insert(new_index, el);
        storage[i] = GetNodeIterator(new_index);
    }
    
    public:
    cSolve(const string& name){
        string line;
        ifstream ifstr(name, ios::binary);
        size = 0;
        for(getline(ifstr, line); !ifstr.eof() && !line.empty(); getline(ifstr, line)){
            long long value = stol(line);
            Insert(size, value);
            storage.push_back(GetNodeIterator(size));
            size++;
        }
        ifstr.close();
    }

    void solve(void){
        int d = storage.size();
        //cout << "start:" << endl;
        for(int i = 0; i < d; i++){
            move(i);
            //if (i % 100 == 0) {
            //    Print();
            //}
        }
        //cout << "finish:" << endl;
        //Print();
    }

    int mix(void){
        //математическая формула из условия задачи
        int zid = -1; //storage[ordByVal[0]].getId();
        for (int i = 0; i < size; i++) {
            Type el = Find(i);
            if (el == 0) {
                zid = i;
                break;
            }
        }

        int d = size;
        int id1 = (zid + 1000) % d;
        int id2 = (zid + 2000) % d;
        int id3 = (zid + 3000) % d;
        return Find(id1) + 
                Find(id2) +
                Find(id3);
    }
};

int main(void){	
    int result;
    const auto tstart = chrono::steady_clock::now();

    cSolve task("cond.input");
    task.solve();
    result = task.mix();
    
    if(result != 7153){
        cout << "cond failed (7153):" << result <<endl;
        return -2;
    }
    cout<<"cond passed "<< result << endl;
    const auto tend = chrono::steady_clock::now();
    const auto interval = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
    std::cout << "time passed: " << std::chrono::microseconds(interval).count() << "us" << std::endl;
    // 484401us
    return 0;
}
