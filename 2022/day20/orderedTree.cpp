//[UTF-8 en/ru]
#include <orderedTree.hpp>
	
cTree::T& cTree::operator[](int index){
	cTree* ptr = this;
	
	//нужно развернуть рекурсию в цикл, тут не нужен стек.
	int leftSize = 0;
	if(left != nullptr){
		leftSize = left->size;
	}
	// 1) рекурсивный поиск в левом поддереве
	if(index < leftSize){
		return left->[index];
		//try 
		return left[index];
		// is it legal as sintaxis?
		return this->[index];
		/// == ?
		return [index];
	}
	// 2) ответ корень
	if(index == leftSize){
		return &this;
	}
	// 3) рекурсивный поиск в правом поддереве
	if(right != nullptr){		
		return right->[index - leftSize];
	}
	// 4) exception чтото пошло не так.
}

// bool cTree::isMeLeftChild(){
// 	return ptr->paretn->left == ptr;
// }

bool cTree::isRoot(){
	return parent == this;
}

int cTree::getIndex(cTree* ptr){
	int leftSize = 0;
	if(ptr->left != nullptr){
		leftSize = ptr->left->size;
	}
	if(isRoot()){
		return leftSize;
	}
	return leftSize + getIndex(ptr->parent);
}

cTree::cTree(T newData){
	data = newData;
	height = 0; // высота листа
	size = 1; // размер листа, то есть содержит только свой элемент.

}

void cTree::rotateLeft(cTree* X){
	// 1) X->Y to X<-Y
	cTree* parent = X->parent;
	cTree* Y = X->right;
	//cTree* subtree1 = X->left;
	cTree* subtree23 = Y->left;
	//cTree* subtree4 = Y->right;	

	// 2) переключение предка
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

	// 3) поворот
	Y->parent = parent;
	Y->left = X;
	X->right = subtree23;
}

void cTree::rotateRight(cTree* Y){
	// 1) X<-Y to X->Y
	cTree* parent = Y->parent;
	cTree* X = Y->left;
	//cTree* subtree1 = X->left;
	cTree* subtree23 = X->right;
	//cTree* subtree4 = Y->right;

	// 2) переключение предка
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

	// 3) поворот
	X->parent = parent;
	X->left = left;
	Y->right = subtree23;
}

void rotateLeftDouble(cTree* X){
	// X->Y-<Z to X<-Z->Y
	cTree* Y = X->left;
	cTree* Z = Y->right;
	rotateRight(Y);
	rotateLeft(X);
}

void rotateRightLeftDouble(cTree* Y){
	// X->Y-<Z to X<-Z->Y
	cTree* X = X->right;
	cTree* Z = X->left;
	rotateLeft(X);
	rotateRight(Y);
}


void cTree::insert(int index, T value){
	//1) структура была пуста. новый элемент теперь корень 
	// и значит он сам себе папа
	if(size == 0){
		parent = this;
		return;
	}

	// 2)создать элемент и найти куда его вставить.
	cTree* newElement = new cPoint(value);
	
	// 3) найти куда его вставить.
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

	// 4) восстанавить инварианты.
	// высоты по правилам АВЛ
	// размеры - сначала вдоль АВЛ, потом до корня 
	TODO восстановление инвариантов
}

cTree::void remove(int index){
	
	// 1) остался только корень. 
	if(size == 1){
		parent = nullptr;
		return;
	}

	// 2) найти кого удаляю
	cTree* it = &[index];

	// 3) свопаю его до листа.
	for(; it != nullptr; it = ){
		ptr->element = move(next->element);
		ptr = next;
	}
	//delete leaf
	cTree* prev = ptr;
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
	
	// 4) восстанавить инварианты.
	// высоты по правилам АВЛ
	// размеры - сначала вдоль АВЛ, потом до корня 	
	TODO восстановление инвариантов

}
