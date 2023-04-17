//[UTF-8 en/ru]
#include "orderedTree.hpp"
/************************************************************
* @group группа методов интерфейса
************************************************************/

using namespace std;
template class cTree<int>;

template<>
cTree<int>::cTree(int newData){
	element = newData;
	height = 0; // высота листа
	size = 1; // размер листа, то есть содержит только свой элемент
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	root = nullptr;
}

// возвращает ссылку на элемент с данным индексом
template<>	
cTree<int>& cTree<int>::operator[](int index){
	
	
	for(cTree* it = root; ; it != nullptr && it->size > 0 && index > 0; ){
		
		int leftSize = 0;
		if(it->left != nullptr){
			leftSize = it->left->size;
		}

		// 1) искомая вершина в левом поддереве
		if(index < leftSize){
			it = it->left;
			continue;
		}
		// 2) сейчас стоим в искомой вершине
		if(index == leftSize){
			return *it;
		}
		// 3) искомая вершина в правом поддереве
		if(right != nullptr){		
			index -= leftSize + 1;
			it = it->right;
			continue;
		}
	}
	
	//4) TODO exception чтото пошло не так.
	return *this;
}	

template<>
int cTree<int>::getIndex(){
	int leftSize = 0;
	for(cTree* it = root; ; it = it->parent){
		if(this->left != nullptr){
			leftSize += this->left->size;
		}
		if(it->isRoot()){
			break;
		}
	}
	return leftSize;		
}

template<>
void cTree<int>::insert(int index, int value){
	//1) структура была пуста. новый элемент теперь корень 
	// и значит он сам себе папа
	if(isEmpty()){
		element = value;
		parent = this;
		restoreInvariants();
		root = this;
		return;
	}

	//2) создать элемент
	cTree* newElement = new cTree(value);
	
	//3) найти куда его вставить
	
	int tmp = (*this)[0].element;
	cTree* it = &((*this)[index]);
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
// void cTree<T>::push_back(T value){
// 	insert(size- 1, value);
// }

template<>
void cTree<int>::remove(int index){
	//1) остался только корень
	if(size == 1){
		parent = nullptr;
		return;
	}

	//2) найти кого удаляю
	cTree* marked = &((*this)[index]);
	//3) пусть он всплывет в лист
	
	// if(marked->right == nullptr){
	// 	if(marked->parent->left == marked){
	// 		marked->parent->left = marked->left;
	// 	}
	// 	else{
	// 		marked->parent->right = marked->left;
	// 	}
	// }
	// else
	{
		cTree* it = marked;
		if(it->right != nullptr){
			it = it->right;
		}
		for(; it->left != nullptr; it = it->left){			
		}
		//меняю местами
		swap(marked->parent, it->parent);
		swap(marked->left, it->left);
		swap(marked->right, it->right);
		if(!marked->isLeaf()){
			swap(marked->right->parent, marked->parent);
			swap(marked->right->left, marked->left);
			swap(marked->right->right, marked->right);
			//it = it->right;
		}
		
	}

	//4) восстанавить инварианты
	marked->parent->restoreInvariants();

	//5) удаляю 
	delete marked;
}
