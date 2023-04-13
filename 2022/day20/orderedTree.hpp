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
	
	//misc
	bool isRoot(void);
	// bool isLeaf(void);
	// bool isMeLeftChild(void);	
	
	// методы для инвариантов авл
	void rotateLeft(cTree* X);
	void rotateRight(cTree* Y);
	void rotateLeftDouble(cTree& left, cTree& center, cTree& right);
	void rotateRightDouble(cTree& left, cTree& center, cTree& right);
	public:
	cTree(T x);
	cTree& operator[](int index);
	int getIndex(cTree* ptr);
	void insert(int index, int value);
	void push_back(int value);
	void delete(int index);

	int size(void);
};