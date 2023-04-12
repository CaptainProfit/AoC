//[UTF-8 en/ru]
#pragma once
#define ull unsigned long long

using namespace std;

template<typename T>
class cTree{
	// структура бинарного дерева, которая сохраняет порядок элементов
	// и поддерживает операции:
	// поиска по индексу за log(N)
	// вставки после индекса за log(N)
	// удаления по индексу за log(N)
	bool isRoot(void);
	bool isLeaf(void);
	T element;
	int size = 0; // размер поддерева вершины
	int height = 0; // для AVL
	cTree *left, *right, *parent = nullptr;
	public:
	cTree& operator[](int index);
	void insert(int index, int value);
	void push_back(int value);
	void delete(int index);

	int size(void);
};