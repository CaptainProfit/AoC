//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#define ull unsigned long long

using namespace std;
class cTreeNode{
	cTreeNode *left, *right;
	int h;
	int size;
	int value;

	void rotateLeft(void){
		//1) X->Y to X<-Y
		cTreeNode* X = this;
		cTreeNode* parent = X->parent;
		cTreeNode* Y = X->right;
		//cTreeNode* subtree1 = X->left;
		cTreeNode* subtree23 = Y->left;
		//cTreeNode* subtree4 = Y->right;	

		//2) переключение предка
		if(parent != X){
			if(parent->left == X){
				parent->left = Y;
			}
			else{
				parent->right = Y;
			}
		}
		else{
			//корень
			parent = Y;
		}

		//3) поворот
		Y->parent = parent;
		Y->left = X;
		X->parent = Y;	
		X->right = subtree23;
	}

	void rotateRight(void){
		//1) X<-Y to X->Y
		cTreeNode* Y = this;
		cTreeNode* parent = Y->parent;
		cTreeNode* X = Y->left;
		//cTree* subtree1 = X->left;
		cTreeNode* subtree23 = X->right;
		//cTreeNode* subtree4 = Y->right;

		//2) переключение предка
		if(parent != Y){
			if(parent->left == Y){
				parent->left = X;
			}
			else{
				parent->right = X;
			}
		}
		else{
			//корень
			parent = X;
		}

		//3) поворот	
		X->parent = parent;
		X->right = Y;
		Y->parent = X;
		Y->left = subtree23;
	}

	void rotateLeftDouble(void){
		// X->Y-<Z to X<-Z->Y
		cTreeNode* Y = left;
		cTreeNode* X = this;
		//cTreeNode* Z = Y->right;
		Y->rotateRight();
		X->rotateLeft();
	}

	void rotateRightDouble(void){
		// X->Y-<Z to X<-Z->Y
		cTreeNode* Y = this;
		cTreeNode* X = right;		
		//cTreeNode* Z = X->left;
		X->rotateLeft();
		Y->rotateRight();
	}

	int getH(void){
		if(this == nullptr)
			return 0;
		else return h;
	}
	int getSize(void){
		if(this == nullptr)
			return 0;
		else return size;
	}

	int getSizeI(void){
		if(this == nullptr){
			return 0;
		}
		int result = 1;
		if(left != nullptr){
			result += left->size;
		}
		if(right != nullptr){
			result += right->size;
		}
		return result;
	}
	void restoreBalance(void){
		// leafs to root
		if(this == nullptr){
			return;
		}
		int diff = left->getH() - right->getH();
		if(diff > 2){
			if(left->left->getH() >= left->right->getH() ){
				rotateRight();
			}
			else{
				rotateRightDouble();
			}
		}
		if(diff < -2){
			if(right->left->getH() <= right->right->getH()){
				rotateLeft();
			}
			else{
				rotateLeftDouble();
			}
		}

		size = getSizeI();
		h = 1 + min(left->getH(), right->getH());
		parent->restoreBalance();
		//assert(0);
	}

	void downToLeaf(void){
		cTreeNode* it = this->left;
		while(it->right != nullptr){
			it = it->right;
		}
		swap(it->left, this->left);
		swap(it->right, this->right);
		swap(it->parent, this->parent);
		swap(it->h, this->h);
		swap(it->size, this->size);
	}

	public:	
	cTreeNode* parent;

	cTreeNode(int value){
		size = 1;
		h = 1;
		this->value = value;
		parent = nullptr;
		left = nullptr;
		right = nullptr;
	}

	void remove(cTreeNode* oldEl){
		// если есть обе ребенки 
		if(oldEl->left != nullptr && oldEl->right != nullptr){			
			//есть оба ребенка.
			oldEl->downToLeaf();
			remove(oldEl);
		}
		cTreeNode* branch = oldEl->left;
		if(branch == nullptr){
			branch = oldEl->right;
		}
		cTreeNode* tmp = oldEl->parent;
		oldEl->left = nullptr;
		oldEl->right = nullptr;
		oldEl->parent = nullptr;
		if(tmp != nullptr){
			if(tmp->left == oldEl){
				tmp->left = branch;
			}
			if(tmp->right == oldEl){
				tmp->right = branch;
			}
			if(branch != nullptr){
				branch->parent = tmp;
			}
		}
		h = -1;
		size = -1;
		tmp->restoreBalance();
	}

	void insertAfter(int id, cTreeNode* newEl){
		cTreeNode* it = (*this)[id];
		if(it->right == nullptr){
			it->right = newEl;			
		}
		else{
			it = it->right;
			while( it->left != nullptr){
				it = it->left;
			}		
			it->left = newEl;
		}
		newEl->parent = it;
		newEl->restoreBalance();
		assert(1);
	}

	cTreeNode* operator[](int id){
		if(id < left->getSize()){
			return (*left)[id];
		}
		if(id == left->getSize()){
			return this;
		}
		if(id > left->getSize()){
			return (*right)[id - left->getSize() - 1];
		}
		assert(0);
		return nullptr;		
	}

	int getId(void){
		if(parent == nullptr){
			return 0;
		}
		if(parent->left == this){
			return left->size;
		}
		if(parent->right == this){
			return left->size + parent->getId();
		}		
		assert(0);
		return -1;
	}

	int getValue(void){
		return value;
	}

	int getValue(int i ){
		assert(0);
		return getValue();
	}

	void print(void){
		if(this == nullptr){
			return;
		}
		left->print();
		cout << " " << value;
		right->print();
	}
};

class cSolve{
	vector<cTreeNode> storage;
	map<int, int> ordByVal; // orderByValue[value] = index of value in values;
	cTreeNode* root;
	int size;

	void move(int i){
		cTreeNode* tmp = &storage[i];
		int oldId = tmp->getId();
		int newId = oldId + tmp->getValue();
		
		root->remove(tmp);
		while(root->parent != nullptr)
			root = root->parent;
		root->print();
		cout << endl;

		root->insertAfter(newId, tmp);
		while(root->parent != nullptr)
			root = root->parent;
	}
	
	public:
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		int size = 0;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			int value = stoi(line);
			storage.push_back(value);
			ordByVal.emplace(value, size);
			size++;
		}
		root = &storage[0];
		for(int i = 1; i < size; i++){
			root->insertAfter(i - 1, &storage[i]);
		}
		ifstr.close();
	}

	void solve(void){
		int d = storage.size();
		root->print();
		cout << endl;
		for(int i=0; i<d; i++){
			move(i);
			cout << "move " << i << ":"; 
			root->print();
			cout << endl;
		}
	}

	int mix(void){
		//математическая формула из условия задачи
		int zid = storage[ordByVal[0]].getId();
		int d = size;
		int id1 = (zid + 1000) % d;
		int id2 = (zid + 2000) % d;
		int id3 = (zid + 3000) % d;
		return (*root)[id1]->getValue() + 
				(*root)[id2]->getValue() +
				(*root)[id3]->getValue();
	}
};

int main(void){	
	int result;
	cSolve test("test.input");
	test.solve();
	result = test.mix();
	
	if(result != 3){
		cout << "test failed (3):" << result <<endl;
		return -2;
	}
	cout<<"test passed "<< result << endl;
	return 0;
}
