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
#include <cassert>
#include <algorithm>
#define ull unsigned long long
int cntl = 0;
int cntr = 0;
int cntdl = 0;
int cntdr = 0;
using namespace std;
class cTreeNode{
	cTreeNode *left, *right;
	int h;
	int size;
	int value;

	void rotateLeft(void){
		cntl++;
		//1) X->Y to X<-Y
		cTreeNode* X = this;
		cTreeNode* parent = X->parent;
		cTreeNode* Y = X->right;
		//cTreeNode* subtree1 = X->left;
		cTreeNode* subtree23 = Y->left;
		//cTreeNode* subtree4 = Y->right;	

		//2) переключение предка
		if(parent != nullptr){
			if(parent->left == X){
				parent->left = Y;
			}
			else if(parent->right == X){
				parent->right = Y;
			}
			else{
				assert(0);
			}
		}
		
		if(subtree23 != nullptr){
			subtree23->parent = X;
		}

		//3) поворот
		Y->parent = parent;
		X->parent = Y;
		Y->left = X;
		//X->left - same
		X->right = subtree23;
		//Y->right - same
		
	}

	void rotateRight(void){
		cntr++;
		//1) X<-Y to X->Y
		cTreeNode* Y = this;
		cTreeNode* parent = Y->parent;
		cTreeNode* X = Y->left;
		//cTree* subtree1 = X->left;
		cTreeNode* subtree23 = X->right;
		//cTreeNode* subtree4 = Y->right;

		//2) переключение предка
		if(parent != nullptr){
			if(parent->left == Y){
				parent->left = X;
			}
			else if(parent->right == Y){
				parent->right = X;
			}
			else{
				assert(0);
			}
		}

		if(subtree23 != nullptr){
			subtree23->parent = Y;
		}

		//3) поворот	
		X->parent = parent;
		X->right = Y;
		Y->parent = X;
		Y->left = subtree23;
	}

	void rotateLeftDouble(void){
		cntdl++;
		// X->Y-<Z to X<-Z->Y
		cTreeNode* Y = right;
		cTreeNode* X = this;
		//cTreeNode* Z = Y->right;
		Y->rotateRight();
		X->rotateLeft();
	}

	void rotateRightDouble(void){
		cntdr++;
		// X->Y-<Z to X<-Z->Y
		cTreeNode* Y = this;
		cTreeNode* X = left;		
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

	void refreshNode(void){
		if(this == nullptr){
			return;
		}
		size = getSizeI();
		h = 1 + max(left->getH(), right->getH());
	}

	void restoreBalance(void){
		// leafs to root
		if(this == nullptr){
			return;
		}
		int diff = left->getH() - right->getH();
		if(diff >= 2){
			if(left->left->getH() >= left->right->getH() ){
				rotateRight();
			}
			else{
				left->rotateLeft();
				left->left->refreshNode();
				rotateRight();
			}
		}
		if(diff <= -2){
			//left.h < right.h
			if(right->left->getH() <= right->right->getH()){
				//rl.h <= rr.h
				rotateLeft();
			}
			else{
				//rl.h > rr.h
				right->rotateRight();
				right->right->refreshNode();
				rotateLeft();
			}
		}
		
		refreshNode();
		parent->restoreBalance();
		//assert(0);
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

	bool isCorrect(cTreeNode* myPar = nullptr){
		if(this == nullptr){
			return true;
		}
		if(parent != myPar){
			return false;
		}
		if(size != 1 + left->getSize() + right->getSize()){
			return false;
		}
		if( h > 1+ max(left->getH(), right->getH())){
			return false;
		}
		return left->isCorrect(this) && right->isCorrect(this);
	}

	void remove(cTreeNode* oldEl){
		cTreeNode* par = oldEl->parent;
		cTreeNode* it = nullptr;
		//1) если хотя бы одного ребенка нет
		if(oldEl->left == nullptr || oldEl->right == nullptr){
			if(oldEl->left != nullptr){
				it = oldEl->left;
			}
			else if(oldEl->right != nullptr){
				it = oldEl->right;
			}
			if(par->left == oldEl){
				par->left = it;
			}
			else if(par->right == oldEl){
				par->right = it;
			}
			if(it != nullptr){
				it->parent = par;
			}

			oldEl->parent = nullptr;
			oldEl->left = nullptr;
			oldEl->parent = nullptr;
			oldEl->size = -1;
			oldEl->h = -1;
			par->restoreBalance();
			return;
		}

		//2) если оба ребенка - надо найти чтото пустоватое и свапнуть туда.
		cTreeNode* branch = oldEl->left;
		while(branch->right != nullptr){
			branch = branch->right;
		}
		cTreeNode* branchpar = branch->parent;
		
		if(par != nullptr){
			if(par->left == oldEl){
				par->left = branch;
			}
			else if(par->right == oldEl){
				par->right = branch;
			}
			else {
				assert(0);
			}
		}

		if(branchpar != nullptr){
			if(branchpar->left == branch){
				branchpar->left = oldEl;
			}
			else if(branchpar->right == branch){
				branchpar->right = oldEl;
			}
			else {
				assert(0);
			}
		}
		swap(branch->parent, 	oldEl->parent	);
		swap(branch->left, 		oldEl->left		);
		swap(branch->right, 	oldEl->right	);
		swap(branch->h, 		oldEl->h		);
		swap(branch->size, 		oldEl->size		);
		
		//и ещё раз!
		remove(oldEl);
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
		stack<cTreeNode*> que;
		que.push(this);
		while(que.top()->parent != nullptr){
			que.push(que.top()->parent);
		}
		int id = 0;
		while(!que.empty()){
			cTreeNode* it = que.top();
			que.pop();
			if(que.empty()){
				id += it->left->getSize();
			}
			else{
				if(it->right == que.top()){
					id += it->left->getSize() + 1;
				}
			}
		}
		return id;
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
			cout << ".";
			return;
		}
		cout << "[";
		left->print();
		cout << " " << value;
		right->print();
		cout << "]";
	}
	void printH(void){
		if(this == nullptr){
			cout << ".";
			return;
		}
		cout << "[";
		left->printH();
		cout << " " << h;
		right->printH();
		cout << "]";
	}
	
	void printSize(void){
		if(this == nullptr){
			cout << ".";
			return;
		}
		cout << "[";
		left->printSize();
		cout << " " << size;
		right->printSize();
		cout << "]";
	}
};

class cSolve{
	vector<cTreeNode> storage;
	map<int, int> ordByVal; // orderByValue[value] = index of value in values;
	cTreeNode* root;
	int size;

	void restoreRoot(void){
		while(root->parent != nullptr){
			root = root->parent;
		}
	}

	void move(int i){
		cTreeNode* tmp = &storage[i];
		int oldId = tmp->getId();
		int d = tmp->getValue();
		d %= size - 1;
		d += size - 2;
		d %= size - 1;
		int newId = oldId + d;
		newId %= size - 1;
		root->remove(tmp);
		restoreRoot();
		cout << "delete: "; 
		root->print();
		cout << endl << "\t";
		root->printH();
		cout << endl << "\t";
		root->printSize();
		cout << endl;
		assert(root->isCorrect());
		//root->print();
		root->insertAfter(newId, tmp);
		restoreRoot();
		//assert(root->isCorrect());
	}
	
	public:
	cSolve(const string& name){
		string line;
		ifstream ifstr(name, ios::binary);
		size = 0;
		for(getline(ifstr, line); !ifstr.eof(); getline(ifstr, line)){
			int value = stoi(line);
			storage.push_back(value);
			ordByVal.emplace(value, size);
			size++;
		}
		root = &storage[0];
		for(int i = 1; i < size; i++){
			root->insertAfter(i - 1, &storage[i]);
			restoreRoot();
			assert(root->isCorrect());
		}
		ifstr.close();
	}

	void solve(void){
		int d = storage.size();
		cout << "start: "; 
		root->print();
		cout << endl << "\t";
		root->printH();
		cout << endl << "\t";
		root->printSize();
		cout << endl;
		assert(root->isCorrect());
		for(int i = 0; i<d; i++){
			move(i);
			cout << "move " << i + 1 << ":"; 
			root->print();
			cout << endl << "\t";
			root->printH();
			cout << endl << "\t";
			root->printSize();
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

	cSolve cond("test.input");
	cond.solve();
	result = cond.mix();
	if(result != 7153){
		cout << "cond failed (7153):" << result <<endl;
		return -2;
	}
	cout<<"cond passed "<< result << endl;
	return 0;
}
