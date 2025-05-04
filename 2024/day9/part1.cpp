#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string line;
vector<int> filesystem;
void ReadFile(string filename) {
    ifstream f(filename);
    getline(f, line);
}

void Arrange() {
	int id = 0;
	for (int i = 0 ; i < line.length(); i++) {
		for (int j = 0; j < line[i] - '0'; j++) {
			if (i%2 == 0) {
				filesystem.push_back(id);
			}
			else {
				filesystem.push_back(-1);
			}
		}
		if (i%2 == 0) {
			id++;
		}
	}
}

void Print() {
    for (auto& id : filesystem) {
        cout << id << " ";
    }
    cout << endl;
}

void Defragment() {
	int i2 = filesystem.size() - 1;
	for (int i1 = 0; i1 < i2; i1++) {
		if (filesystem[i1] == -1) {
			for (;filesystem[i2] == -1; i2--);
			swap(filesystem[i1], filesystem[i2]);
			i2--;
		}
	}
}

unsigned long long Calculate() {
	unsigned long long sum = 0;
	for (int i = 0; i <filesystem.size(); i++) {
		if (filesystem[i] == -1) {
			break;
		}
		sum += i * filesystem[i];
	}
	return sum;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }
    ReadFile(argv[1]);
    Arrange();
    Print();
    Defragment();
    Print();
    cout << Calculate() << endl;
    // 6225730762521
}
