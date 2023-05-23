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

string sne = "";
string sob = "";
string scb = "";

int cntl = 0;
int cntr = 0;
int cntdl = 0;
int cntdr = 0;
int cntdel = 0;
int cntins = 0;
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
				// assert(0);
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
				// assert(0);
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

	cTreeNode* restoreRoot(void){
		cTreeNode* it = this;
		while(it->parent != nullptr) it = it->parent;
		return it;
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
		cTreeNode* it = this;
		// while(it->parent != nullptr){
		// 	it = it->parent;
		// 	it->refreshNode();
		// }
		// return;
		// leafs to root
		if(this == nullptr){
			return;
		}
		refreshNode();
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
		// //assert(0);
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

	void trickParent(cTreeNode* newKid){
		if(parent != nullptr){
			if(parent->left == this){
				parent->left = newKid;
			}
			else if(parent->right == this){
				parent->right = newKid;
			}
			else{
				// assert(0);
			}
		}
	}

	void bindKids(void){
		if(left != nullptr){
			left->parent = this;
		}
		if(right != nullptr){
			right->parent = this;
		}
	}

	cTreeNode* remove(cTreeNode* oldEl){
		cntdel++;
		cTreeNode* oldRoot = this;
		//cTreeNode* par = oldEl->parent;
		cTreeNode* it = nullptr;
		//1) если хотя бы одного ребенка нет
		if(oldEl->left == nullptr || oldEl->right == nullptr){
			
			//1.1) определиться, на кого буду менять.
			if(oldEl->left != nullptr){
				it = oldEl->left;
			}
			else if(oldEl->right != nullptr){
				it = oldEl->right;
			}

			//1.2) восстановить баланс
			oldEl->trickParent(it);
			if(it != nullptr){
				it->parent = oldEl->parent;
				//it->refreshNode();
				it->restoreBalance();
			}
			else{
				if(oldEl->parent != nullptr){
					oldEl->parent->refreshNode();
					oldEl->parent->restoreBalance();
				}
				else{
					//нереализовано
					// assert(0); 
				}
			}
			
			//1.3) потереть ячейку элемента.
			oldEl->parent = nullptr;
			oldEl->left = nullptr;
			oldEl->right = nullptr;
			oldEl->size = -1;
			oldEl->h = -1;

			//1.4) восстановить корень
			oldRoot = oldRoot->restoreRoot();
			// assert(oldRoot->isCorrect());
			return oldRoot->restoreRoot();
		}

		//2) если оба ребенка присутствуют
		//2.1) найти самого левого справа.
		cTreeNode* branch = oldEl->left;
		while(branch->right != nullptr){
			branch = branch->right;
		}
		if(oldEl == this){
			oldRoot = branch;
		}
		
		//2.2) переобозначаю предку детей для удаляемого элемента
		oldEl->trickParent(branch);
		//2.2) переобозначаю предку детей для подмены
		branch->trickParent(oldEl);
		//2.3) меняю параметры ячеек в дереве местами.
		swap(oldEl->parent, branch->parent);
		swap(oldEl->left, 	branch->left);
		swap(oldEl->right, 	branch->right);
		swap(oldEl->h, 		branch->h);
		swap(oldEl->size, 	branch->size);

		oldEl->bindKids();
		branch->bindKids();
		//и ещё раз!
		oldRoot = oldRoot->restoreRoot();
		// assert(oldRoot->isCorrect());
		return oldRoot->remove(oldEl);
	}

	cTreeNode* insertAfter(int id, cTreeNode* newEl){
		cntins++;
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
		return restoreRoot();
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
		// assert(0);
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
		// assert(0);
		return getValue();
	}

	void print(void){
		#ifndef LOG
		return;
		#endif
		if(this == nullptr){
			cout << ".";
			return;
		}
		cout << sob;
		left->print();
		cout << " " << value;
		right->print();
		cout << scb;
	}
	void printH(void){
		#ifndef LOG_H
		return;
		#endif
		if(this == nullptr){
			cout << sne;
			return;
		}
		cout << sob;
		left->printH();
		cout << " " << h;
		right->printH();
		cout << scb;
	}
	
	void printSize(void){
		#ifndef LOG_SIZE
		return;
		#endif
		if(this == nullptr){
			cout << sne;
			return;
		}
		cout << sob;
		left->printSize();
		cout << " " << size;
		right->printSize();
		cout << scb;
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
		int d = tmp->getValue();
		d %= size - 1;
		d += size - 2;
		d %= size - 1;
		int newId = oldId + d;
		newId %= size - 1;
		root = root->remove(tmp);
		// assert(root->isCorrect());
		//root->print();
		root = root->insertAfter(newId, tmp);
		// //assert(root->isCorrect());
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
			root = root->insertAfter(i - 1, &storage[i]);
			// assert(root->isCorrect());
		}
		ifstr.close();
	}

	void solve(void){
		int d = storage.size();
		cout << "start: "; 
		//root->print();
		// root->print();
		// cout << endl << "\t";
		// root->printH();
		// cout << endl << "\t";
		// root->printSize();
		// cout << endl;
		// assert(root->isCorrect());
		for(int i = 0; i<d; i++){
			move(i);
			//cout << "move " << i + 1 << ":"; 
			// root->print();
			// cout << endl << "\t";
			// root->printH();
			// cout << endl << "\t";
			// root->printSize();
			// cout << endl;
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
	const auto tstart = chrono::steady_clock::now();

	cSolve test("test.input");
	test.solve();
	result = test.mix();
	
	if(result != 3){
		cout << "test failed (3):" << result <<endl;
		return -2;
	}
	cout << "test passed!" << endl;

	cSolve cond("cond.input");
	cond.solve();
	result = cond.mix();
	if(result != 7153){
		cout << "cond failed (7153):" << result <<endl;
		return -2;
	}
	cout<<"cond passed "<< result << endl;
	const auto tend = chrono::steady_clock::now();
	const auto interval = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
	std::cout << "time passed: " << std::chrono::microseconds(interval).count() << std::endl;
	return 0;
}
