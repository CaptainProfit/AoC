//[UTF-8 en/ru]
#include "orderedTree.hpp"
#include <utility>
/************************************************************
* @group группа методов интерфейса
************************************************************/

using namespace std;
//template class cContainer<int>;

// возвращает ссылку на элемент с данным индексом
cContainer::cTreeNode* cContainer::operator[](int index){	
    //index++;
    for(cContainer::cTreeNode* it = root; it != nullptr && it->getSize() > 0; ){
        
        int leftSize = 0;
        if(it->left != nullptr){
            leftSize = it->left->getSize();
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
        assert(0);  
    }
    //4) TODO exception чтото пошло не так.
    assert(0);
    return root;
}	

//template<>
void cContainer::insert(int index, const T& value, size_t iter_index){
    //1) структура была пуста. новый элемент теперь корень
    //updateOffset(value);

    cTreeNode* newElement = new cTreeNode;
    newElement->setValue(value);
    newElement->parent = nullptr;
    newElement->left = nullptr;
    newElement->right = nullptr;
    newElement->iter_index = iter_index;
    //3) найти куда его вставить
    if(isEmpty() ){
        root = newElement;
        //root->parent = root;
        root = newElement->restoreInvariants();
        return;
    }
    
    cTreeNode* it;
    if (index < root->getSize()) {
        it = (*this)[index];
        if(it->left == nullptr){
            it->left = newElement;
        }
        else{
            for(it = it->left; it->right != nullptr; it = it->right);
            it->right = newElement;
        }
    }
    else {
        for(it = root; it->right != nullptr; it = it->right);
        it->right = newElement;
    }
    newElement->parent = it;

    //4) восстанавить инварианты
    root = newElement->restoreInvariants();
}

// template<typename T>
// void cContainer<T>::push_back(T value){
// 	insert(size- 1, value);
// }

//template<>
void cContainer::remove(int index){
    
    //2) найти кого удаляю
    cTreeNode* marked = (*this)[index];
    assert(marked);
    
    //3) если удаляю лист - ничего не делаю.
    while(marked->left != nullptr || marked->right != nullptr) {
        cTreeNode* it = marked;
        if(it->right != nullptr){
            it = it->right;
            for(; it->left != nullptr; it = it->left);
        }
        else if( it->left != nullptr) {
            it = it->left;
            for(; it->right != nullptr; it = it->right);
        }
        
        swap(it->value, marked->value);
        swap(it->iter_index, marked->iter_index);
        swap(values[it->iter_index], values[marked->iter_index]);
        marked = it;
    }
    
    if (marked->parent == nullptr) {
        root = nullptr;
    }
    else {
        if (marked->parent->left == marked) {
            marked->parent->left = nullptr;
        }
        else if (marked->parent->right == marked) {
            marked->parent->right = nullptr;
        }
        else {
            assert(0);
        }
        root = marked->parent->restoreInvariants();
    }        
    delete marked;
}

void cContainer::updateOffset(T value) {
    int len = 0;
    if (value < 0) {
        value = -value;
        len++;
    }
    do {
        len++;
        value /= 10;
    } while(value > 0);
    offset = std::max(offset, len);
}