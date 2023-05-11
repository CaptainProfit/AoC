//[UTF-8 en/ru]
#include "orderedTree.hpp"
/************************************************************
* @group группа методов интерфейса
************************************************************/

using namespace std;
typedef int T;
//template class cContainer<int>;

// возвращает ссылку на элемент с данным индексом

cTreeNode* cContainer::operator[](int index){	
	
	for(cTreeNode* it = root; ; it != nullptr && it->size() > 0 && index > 0; ){
		
		int leftSize = 0;
		if(it->left != nullptr){
			leftSize = it->left->size();
		}

		// 1) искомая вершина в левом поддереве
		if(index < leftSize){
			it = it->left;
			continue;
		}
		// 2) сейчас стоим в искомой вершине
		if(index == leftSize){
			return it;
		}
		// 3) искомая вершина в правом поддереве
		if(it->right != nullptr){		
			index -= leftSize + 1;
			it = it->right;
			continue;
		}
	}
	
	//4) TODO exception чтото пошло не так.
	return root;
}	

//template<>
void cContainer::insert(int index, const T& value){
	//1) структура была пуста. новый элемент теперь корень 
	// и значит он сам себе папа
	
	if(isEmpty() ){
		root = new cTreeNode;
		root->setValue(value);
	}
	
	//3) найти куда его вставить
	
	int tmp = (*this)[0].element;
	cContainer* it = &((*this)[index]);
 	if(it->right == nullptr){
		it->right = newElement;
	}
	else{
		for(; it->left != nullptr; it = it->left){
		}
		it->left = newElement;
	}
	newElement->parent = it;

	//4) восстанавить инварианты
	newElement->restoreInvariants();
}

// template<typename T>
// void cContainer<T>::push_back(T value){
// 	insert(size- 1, value);
// }

//template<>
void cContainer::remove(int index){
	
	//2) найти кого удаляю
	cTreeNode* mark = (*root)[index];
	
	//3) если удаляю лист - ничего не делаю.
	// если удаляю и нет левого потомка, но есть правый - меняю местами с правым.
	// если удаляю и есть левый потомок - проверяю у него правого потомка.
	// 		если есть правый - меняю с правым.
	// 		если нет правого, но есть левый - 

	if(mark->left != nullptr || mark->right != nullptr){
		cTreeNode* it = mark;

		...if(it->right != nullptr){
		...	it = it->right;
		...}
		...for(; it->left != nullptr; it = it->left){			
		...}
		...//меняю местами
		...swap(marked->parent, it->parent);
		...swap(marked->left, it->left);
		...swap(marked->right, it->right);
		...if(!marked->isLeaf()){
		...	swap(marked->right->parent, marked->parent);
		...	swap(marked->right->left, marked->left);
		...	swap(marked->right->right, marked->right);
		...	//it = it->right;
		...}
		
	}

	//4) восстанавить инварианты
	marked->parent->restoreInvariants();

	//5) удаляю 
	delete marked;
}
