//[UTF-8 en/ru]
#include "orderedTree.hpp"
using namespace std;
// template class cContainer<int>;

/************************************************************
* @group группа функций восстановления инвариантов
	после редактирования 
	пересчитывает размеры, высоты, 
	восстанавливает свойство сбалансированности
************************************************************/

// вычисляет инварианты и запоминает в вершине

void cContainer::cTreeNode::calculateInvariant(){
    assert(this);
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

int cContainer::cTreeNode::checkBalance(){
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

// высоты по правилам АВЛ
// размеры - сначала вдоль АВЛ, потом до корня
cContainer::cTreeNode* cContainer::cTreeNode::restoreInvariants() {
    assert(this);
	for (cTreeNode* it = this; it != nullptr; it = it->parent) {
        it->calculateInvariant();
		int condition = it->checkBalance();
		if(condition <= -2){
			if(it->right->checkBalance() > 0){
				it->right->rotateRight();
			}
			it->rotateLeft();
            it->calculateInvariant();
		}
        else if (condition >= 2){
			if(it->left->checkBalance() < 0){
				it->left->rotateLeft();
			}
            it->rotateRight();
            it->calculateInvariant();
		}

        if (it->parent == nullptr) {
            return it;
        }
	}
    assert(0);
    return nullptr;
}