// C++ program to illustrate the use
// of ropes using Rope header file
#include <ext/rope>
#include <iostream>

// SGI extension
using namespace __gnu_cxx;

using namespace std;

//тестирую, что итераторы неинвалидируются
void TestIterators() {
    rope<int> r;
    int size = 10;
    int tries = 10;
    vector<rope<int>::const_iterator> nodes_;
    for (int i = 0; i < size; i++) {
        r.insert(r.size(), i);
    }
    for (int i = 0; i < size; i++) {
        nodes_.push_back(r.begin() + i);
    }
    for(int i = 0; i < r.size(); i++) {
        cout << " " << r[i]; 
    }
    cout << endl;
    
    for (int i = 0; i < tries; i++) {
        for (int j = 0; j < size; j++) { 
            rope<int>::const_iterator it = nodes_[j]; // 
            int pos = nodes_[j].index();
            int value = r[pos];
            r.erase(pos, 1);
            int new_pos = (pos + value*value)%(size - 1);
            r.insert(new_pos, value); // итератор инвалидируется. Изза векторной части.
            nodes_[j] = r.begin() + new_pos;
            cout << "    ";
            for(int i = 0; i < r.size(); i++) {
                cout << " " << r[i]; 
            }
            cout << endl;
        }
        for(int i = 0; i < r.size(); i++) {
            cout << " " << r[i]; 
        }
        cout << endl;
    }
}

// Driver Code
int main()
{
	rope<int> r;
    for (int i = 0; i < 10; i++) {
        r.insert(r.size(), i);
    }
    r.insert(0, 10);
    r.insert(3, 11);
    r.insert(6, 12);
    r.insert(r.size(), 13);
    auto x = r[8];
    // r.remove(8); no;
    // r.erase(8); no
    // r.erase(r.begin() + 8);
    // r.erase(r.const_begin() + 8);
    for(int i = 0; i < r.size(); i++) {
        cout << " " << r[i]; 
    }
    r.erase(8, 1);
    r.insert(13, x);
    r.balance();
    /// r[3] = 4; ???

    cout << endl; 
    for (const auto& x : r) {
        cout << " " << x; 
    }
    cout << endl; 
    TestIterators();
	return 0;
}
