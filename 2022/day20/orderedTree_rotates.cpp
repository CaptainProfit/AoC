//[UTF-8 en/ru]
#include "orderedTree.hpp"
using namespace std;
// template class cContainer<int>;
int rotateCnt = 0;
/************************************************************
* @group группа функций поворотов дерева
 операции сохраняют относительный порядок
 изменяют глубину вершин
 см https://en.wikipedia.org/wiki/AVL_tree
************************************************************/

// корень падает, слева приподнимается
//template<>
void cContainer::cTreeNode::rotateLeft(){
	rotateCnt++;
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
		else{
			parent->right = Y;
		}
	}

	//3) поворот
	Y->parent = parent;
	Y->left = X;
	X->parent = Y;	
	X->right = subtree23;
    if (subtree23 != nullptr) {
        subtree23->parent = X;
    }
    calculateInvariant();
}

// корень падает, справа приподнимается
//template<>
void cContainer::cTreeNode::rotateRight(){
	//1) X<-Y to X->Y
	rotateCnt++;
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
		else{
			parent->right = X;
		}
	}

	//3) поворот	
	X->parent = parent;
	X->right = Y;
	Y->parent = X;
	Y->left = subtree23;
    if (subtree23 != nullptr) {
        subtree23->parent = Y;
    }
    calculateInvariant();
}

// корень падает, слева внук подскакивает

void cContainer::cTreeNode::rotateLeftDouble(){
	// X->Y-<Z to X<-Z->Y
	cTreeNode* Y = right;
	cTreeNode* X = this;
	//cTreeNode* Z = Y->right;
	Y->rotateRight();
    X->rotateLeft();
}

// корень падает, справа внук подскакивает
void cContainer::cTreeNode::rotateRightDouble(){
	// X->Y-<Z to X<-Z->Y
	cTreeNode* Y = this;
	cTreeNode* X = right;		
	//cTreeNode* Z = X->left;
	X->rotateLeft();
	Y->rotateRight();
}