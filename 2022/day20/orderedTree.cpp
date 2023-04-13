//[UTF-8 en/ru]
#include <orderedTree.hpp>

/************************************************************
* @group группа методов интерфейса
************************************************************/

template<typename T>
cTree<T>::cTree(T newData){
	data = newData;
	height = 0; // высота листа
	size = 1; // размер листа, то есть содержит только свой элемент
}

template<typename T>
T& cTree<T>::operator[](int index){
	return this->[index]->element;
}

template<typename T>
cTree<T>& cTree<T>::operator[](int index){
	cTree* ptr = this;
	
	//нужно развернуть рекурсию в цикл, тут не нужен стек.
	int leftSize = 0;
	if(left != nullptr){
		leftSize = left->size;
	}
	//1) рекурсивный поиск в левом поддереве
	if(index < leftSize){
		return left->[index];
		//try 
		return left[index];
		// is it legal as sintaxis?
		return this->[index];
		/// == ?
		return [index];
	}
	//2) ответ корень
	if(index == leftSize){
		return &this;
	}
	//3) рекурсивный поиск в правом поддереве
	if(right != nullptr){		
		return right->[index - leftSize];
	}
	//4) exception чтото пошло не так.
}

template<typename T>
int cTree<T>::getIndex(cTree* ptr){
	int leftSize = 0;
	if(ptr->left != nullptr){
		leftSize = ptr->left->size;
	}
	if(isRoot()){
		return leftSize;
	}
	return leftSize + getIndex(ptr->parent);
}

template<typename T>
void cTree<T>::insert(int index, T value){
	//1) структура была пуста. новый элемент теперь корень 
	// и значит он сам себе папа
	if(isEmpty()){
		parent = this;
		return;
	}

	//2) создать элемент
	cTree* newElement = new cPoint(value);
	
	//3) найти куда его вставить
	cTree* it = &[index];
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

template<typename T>
void cTree<T>::remove(int index){
	//1) остался только корень
	if(size == 1){
		parent = nullptr;
		return;
	}

	//2) найти кого удаляю
	cTree* marked = &[index];
	
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

/************************************************************
* @group группа функций балансировки дерева
************************************************************/

template<typename T>
void cTree<T>::rotateLeft(cTree* X){
	//1) X->Y to X<-Y
	cTree* parent = X->parent;
	cTree* Y = X->right;
	//cTree* subtree1 = X->left;
	cTree* subtree23 = Y->left;
	//cTree* subtree4 = Y->right;	

	//2) переключение предка
	if(X->parent != &X){
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
	X->right = subtree23;
}

template<typename T>
void cTree<T>::rotateRight(cTree* Y){
	//1) X<-Y to X->Y
	cTree* parent = Y->parent;
	cTree* X = Y->left;
	//cTree* subtree1 = X->left;
	cTree* subtree23 = X->right;
	//cTree* subtree4 = Y->right;

	//2) переключение предка
	if(Y->parent != &Y){
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
	X->left = left;
	Y->right = subtree23;
}

template<typename T>
void cTree<T>::rotateLeftDouble(cTree* X){
	// X->Y-<Z to X<-Z->Y
	cTree* Y = X->left;
	cTree* Z = Y->right;
	rotateRight(Y);
	rotateLeft(X);
}

template<typename T>
void cTree<T>::rotateRightDouble(cTree* Y){
	// X->Y-<Z to X<-Z->Y
	cTree* X = X->right;
	cTree* Z = X->left;
	rotateLeft(X);
	rotateRight(Y);
}

/************************************************************
* @group группа функций работы с инвариантами
************************************************************/

template<typename T>
int cTree<T>::calculateInvariant(){
	int leftH = 0, rightH = 0, leftSize = 0, rightSize = 0;
	if(left != nullptr){
		leftH = left->height;
		leftSize = left->size;
	}
	if(right != nullptr){
		rightH = right->height;
		rightSize = right->size;
	}
	h = min(leftH, rightH) + 1;
	size = 1 + leftSize + rightSize;
	if(abs(leftH - rightH) < 2)
		return 0;
	return	leftH - rightH;
}

template<typename T>
void cTree<T>::restoreInvariants(){
	// высоты по правилам АВЛ
	// размеры - сначала вдоль АВЛ, потом до корня

	it = newElement;
	while(1){
		int condition = it->calculateInvariant();
		if(condition > 0){
			// левое поддерево выше правого
			if(it->left->calculateInvariant() < 0){
				it->rotateLeftDouble();
			}
			else{
				it->rotateLeft();
			}
		}
		if(condition < 0){
			// правое поддерево выше левого
			if(it->right->calculateInvariant() > 0){
				it->rotateRightDouble();
			}
			else{
				it->rotateRight();
			}
		}
		if(it->isRoot()){
			break;
		}
	}
}
