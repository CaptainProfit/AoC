//[UTF-8 en/ru]
#pragma once
#define ull unsigned long long

using namespace std;

template<typename T>
class cTree{
	// структура хранит набор элементов типа Т
	// и поддерживает операции:
	// 1)поиска по индексу за log(N)
	// 2)определение индекса по указателю на элемент в структуре.
	// 3)вставки после индекса за log(N)
	// 4)вставки в конец за log(N)
	// 5)удаления по индексу за log(N)
	// реализация через AVL дерево
	// признак корня - он сам себе парент.
	// признак пустого дерева - парент нулевой.

	T element;
	int size = 0; // размер поддерева из этой вершины, для задачи.
	//инвариант - size = 1 + left->size + right->size
	//пустые деревья имеют размер ноль.
	int height = 0; // для балансировки AVL
	//инвариант abs(left->height - right->height) < 2
	cTree *left, *right, *parent = nullptr;
	
	//методы дерева
	bool isRoot(void){ return parent == this; }
	bool isLeaf(void){ return parent == this; }
	bool isEmpty(void){	return parent == nullptr; }
	
	// bool isMeLeftChild(void);	
	//методы для инвариантов авл
	void restoreInvariants(void);
	int calculateInvariant(void);

	// методы балансировки
	void rotateLeft(cTree*);
	void rotateRight(cTree*);
	void rotateLeftDouble(cTree*);
	void rotateRightDouble(cTree*);
	public:	
	cTree(){};
	cTree(T x);	
	int sizef(void){ return size; }
	//T& operator[](int index);
	cTree<T>& operator[](int index);
	int getIndex();
	T& getValue(void){ return element;} 

	void insert(int index, T value);
	void push_back(int value);
	void remove(int index);
};