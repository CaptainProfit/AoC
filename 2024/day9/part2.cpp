#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string line;
vector<int> filesystem;
struct File {
	int id;
	int offset;
	int len;
	File(int id_, int offset_, int len_) :
		id(id_),
		offset(offset_),
		len(len_){
	}
};
vector<File> gaps;
vector<File> files;
void ReadFile(string filename) {
    ifstream f(filename);
    getline(f, line);
}

void Arrange() {
	int id = 0;
	int pos = 0;
	for (int i = 0 ; i < line.length(); i++) {
		int d = line[i] - '0';
		for (int j = 0; j < d; j++) {
			if (i%2 == 0) {
				filesystem.push_back(id);
			}
			else {
				filesystem.push_back(-1);
			}
		}
		if (i%2 == 0) {
			files.emplace_back(id, pos, d);
			id++;
		}
		else {
			gaps.emplace_back(-1, pos, d);
		}
		pos += d;
	}
}

void Print() {
    for (auto& id : filesystem) {
        cout << id << " ";
    }
    cout << endl;
}

void Defragment() {
	for (int i2 = files.size() - 1; i2 >= 0; i2--) {
		for (int i1 = 0; i1 < gaps.size(); i1++) {
			if (files[i2].offset < gaps[i1].offset) {
				break;
			}
			if (files[i2].len > gaps[i1].len) {
				continue;
			}
			for (int i = 0; i < files[i2].len; i++) {
				int pos1 = files[i2].offset + i;
				int pos2 = gaps[i1].offset + i;
				swap(filesystem[pos1], filesystem[pos2]);
			}
			gaps[i1].len -= files[i2].len;
			gaps[i1].offset += files[i2].len;
			break;
		}
	}
}

unsigned long long Calculate() {
	unsigned long long sum = 0;
	for (int i = 0; i <filesystem.size(); i++) {
		if (filesystem[i] == -1) {
			continue;
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
    // 6250605700557
}
