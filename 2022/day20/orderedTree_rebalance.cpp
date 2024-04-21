//[UTF-8 en/ru]
#include "orderedTree.hpp"
using namespace std;
template class cTree<int>;

/************************************************************
* @group группа функций восстановления инвариантов
	после редактирования 
	пересчитывает размеры, высоты, 
	восстанавливает свойство сбалансированности
************************************************************/

// вычисляет инварианты и запоминает в вершине
template<>
void cTree<int>::calculateInvariant(){
	int leftH = 0, rightH = 0, leftSize = 0, rightSize = 0;
	if(left != nullptr){
		leftH = left->height;
		leftSize = left->size;
	}
	if(right != nullptr){
		rightH = right->height;
		rightSize = right->size;
	}
	height = max(leftH, rightH) + 1;
	size = 1 + leftSize + rightSize;
}

// возвращает 
// -1 если правая ветвь слишком большая
// 0 ветви сбалансированы
// 1 если левая ветвь слишком большая
template<>
int cTree<int>::checkBalance(){
	int leftH = 0, rightH = 0;
	if(left != nullptr){
		leftH = left->height;
	}
	if(right != nullptr){
		rightH = right->height;
	}
	return	leftH - rightH;
}

//восстанавливает сбалансированость
template<>
void cTree<int>::restoreInvariants(){
	// высоты по правилам АВЛ
	// размеры - сначала вдоль АВЛ, потом до корня
	if(isEmpty())
		return;
	for(cTree* it = this;; it = it->parent){
		it->calculateInvariant();
		int condition = it->checkBalance();
		if(condition <= -2){
			// правое поддерево выше левого
			if(it->right->checkBalance() > 0){
				it->rotateLeftDouble();
			}
			else{
				it->rotateLeft();
			}
		}

		if(condition >= 2){
			// левое поддерево выше правого
			if(it->left->checkBalance() < 0){
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