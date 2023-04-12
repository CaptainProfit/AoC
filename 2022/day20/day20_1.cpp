//[UTF-8 en/ru]
//problem - https://adventofcode.com/2022/day/20
//2147483647 - LONG_MAX
//18446744073709551615 - ULLONG_MAX
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#define ull unsigned long long

using namespace std;

//not fenwick!


//Классика
template<typename T>
class cBinaryTree{
	cBinaryTree* ChooseChild(cBinaryTree* ptr){
		if(ptr->left == nullptr)
			return ptr->right;
		if(ptr->right == nullptr)
			return ptr->left;
		if(ptr->right->size > ptr->left->size)
			return ptr->right;
		//if(ptr->right->size <= ptr->left->size)
			return ptr->left;
	}
	public:
	T element;
	int size = 0;
	cBinaryTree *left, *right, *parent = nullptr;

	T& find(int index){
		if(index <= 0 || size <= index){
			//exception
		}
		// unwrap to stack
		//exception чтото пошло не так.
	}
	
	void remove(int index){
		// сейчас достаточно просто удалить ссылку на элемент, 
		// в инсерте этот же элемент появится обратно
		// иначе - через new/delete?

		cBinaryTree* ptr = &find(index);
		cBinaryTree* next = chooseChild(ptr);

		//swap to leaf
		for(; next != nullptr; next = chooseChild(ptr)){
			ptr->element = move(next->element);
			ptr = next;
		}
		//delete leaf
		cBinaryTree* prev = ptr;
		delete ptr;

		//go back - restore sizes
		ptr->size -= 1;
		for(; prev != nullptr; prev = ptr->parnet){
			//restore sizes;
			ptr = prev;
			if(abs(ptr->left->size + ptr->right->size) >=2){
				// если такое бывает - в чем я не уверен пока что
				// стоит задуматься над балансировкой.
			}
			ptr->size = 1 + ptr->left->size + ptr->right->size;
		}
	}


	void insert(int index){
		// либо формирую дерево в конструкторе из массива
		// либо только что удаленый элемент переставляю в другое место
		// иначе - через new/delete?

		cBinaryTree* ptr = &find(index);
		cBinaryTree* next = chooseChild(ptr);

		//swap to leaf
		for(; next != nullptr; next = chooseChild2(ptr)){ // выбрать пустое или где меньше.
			ptr->element = move(next->element);
			ptr = next;
		}
		//delete leaf
		cBinaryTree* prev = ptr;
		delete ptr;

		//go back - restore sizes
		ptr->size -= 1;
		for(; prev != nullptr; prev = ptr->parnet){
			//restore sizes;
			ptr = prev;
			if(abs(ptr->left->size + ptr->right->size) >=2){
				// если такое бывает - в чем я не уверен пока что
				// стоит задуматься над балансировкой.
			}
			ptr->size = 1 + ptr->left->size + ptr->right->size;
		}
				
	}
};

// template<typename T> cBinaryTree<T>& operator[](cBinaryTree<T>& lhs, int index){
// 		return lhs.find[index];
// 	}

template<typename T>
class cOrderedTree{
	// структура бинарного дерева, которая сохраняет порядок элементов
	// и поддерживает операции:
	// поиска по индексу за log(N)
	// удаления по индексу за log(N)
	// вставки после индекса за log(N)	

	private:
	vector<T> elements;
	
	public:	
	cOrderedTree(vector<T> &b){
		// записать элементы
		elements = b;
		//теперь построить структуру дерева.
		for(int i = 0; i < elements.size(); i++){

		}
	}	
};


int readFileToSMt(vector<int>& arr){	
	string line;
	ifstream ifstr("cond.input", ios::binary);
	arr.reserve(5000);
	while(1){
		getline(ifstr, line);
		if(ifstr.eof()){
			ifstr.close();
			break;
		}
		arr.push_back(stoi(line));		
	}
	
	return 0;
}

tuple<int, int,int> solve(vector<int>& arr){
	cOrderedTree<int> array(move(arr));

	int d = arr.size();
	int z = 0;
	//mix!
	
	return make_tuple(arr[(z + 1000)%d], arr[(z + 2000)%d], arr[(z + 3000)%d]);	
}

int main(void){
	vector<int> arr;
	int r1, r2, r3, result;

	readFileToSMt(arr);	
	tie(r1, r2, r3)= solve(arr);
	result = r1 + r2 + r3;
	cout<<"there are "<<result<<" in result"<<endl;
	return 0;
}
