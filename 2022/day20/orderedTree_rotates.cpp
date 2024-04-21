//[UTF-8 en/ru]
#include "orderedTree.hpp"
using namespace std;
template class cTree<int>;
int rotateCnt = 0;
/************************************************************
* @group группа функций поворотов дерева
 операции сохраняют относительный порядок
 изменяют глубину вершин
 см https://en.wikipedia.org/wiki/AVL_tree
************************************************************/

// корень падает, слева приподнимается
template<>
void cTree<int>::rotateLeft(){
	rotateCnt++;
	//1) X->Y to X<-Y
	cTree* X = this;
	cTree* parent = X->parent;
	cTree* Y = X->right;
	//cTree* subtree1 = X->left;
	cTree* subtree23 = Y->left;
	//cTree* subtree4 = Y->right;	

	//2) переключение предка
	if(parent != X){
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
	X->parent = Y;	
	X->right = subtree23;
}

// корень падает, справа приподнимается
template<>
void cTree<int>::rotateRight(){
	//1) X<-Y to X->Y
	rotateCnt++;
	cTree* Y = this;
	cTree* parent = Y->parent;
	cTree* X = Y->left;
	//cTree* subtree1 = X->left;
	cTree* subtree23 = X->right;
	//cTree* subtree4 = Y->right;

	//2) переключение предка
	if(parent != Y){
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
	X->right = Y;
	Y->parent = X;
	Y->left = subtree23;
}

// корень падает, слева внук подскакивает
template<>
void cTree<int>::rotateLeftDouble(){
	// X->Y-<Z to X<-Z->Y
	cTree* Y = left;
	cTree* X = this;
	//cTree* Z = Y->right;
	Y->rotateRight();
	X->rotateLeft();
}

// корень падает, справа внук подскакивает
template<>
void cTree<int>::rotateRightDouble(){
	// X->Y-<Z to X<-Z->Y
	cTree* Y = this;
	cTree* X = right;		
	//cTree* Z = X->left;
	X->rotateLeft();
	Y->rotateRight();
}