//[UTF-8 en/ru]
#include "orderedTree.hpp"
#include <utility>
/************************************************************
* @group группа методов интерфейса
************************************************************/

using namespace std;
typedef int T;
//template class cContainer<int>;

// возвращает ссылку на элемент с данным индексом

cContainer::cTreeNode* cContainer::operator[](int index){	
    
    for(cContainer::cTreeNode* it = root; it != nullptr && it->getSize() > 0 && index > 0; ){
        
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
    cTreeNode* newElement = new cTreeNode;
    newElement->setValue(index);
    newElement->parent = nullptr;
    newElement->left = nullptr;
    newElement->right = nullptr;
    //3) найти куда его вставить
    
    cTreeNode* it = (*this)[index];
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
    cTreeNode* marked = (*this)[index];
    
    //3) если удаляю лист - ничего не делаю.
    // если удаляю и нет левого потомка, но есть правый - меняю местами с правым.
    // если удаляю и есть левый потомок - проверяю у него правого потомка.
    // 		если есть правый - меняю с правым.
    // 		если нет правого, но есть левый - 

    if(marked->left != nullptr || marked->right != nullptr){
        cTreeNode* it = marked;

        if(it->right != nullptr){
            it = it->right;
        }
        for(; it->left != nullptr; it = it->left){			
        }
        //меняю местами
        std::swap(marked->parent, it->parent);
        std::swap(marked->left, it->left);
        std::swap(marked->right, it->right);
        if(!marked->isLeaf()){
            std::swap(marked->right->parent, marked->parent);
            std::swap(marked->right->left, marked->left);
            std::swap(marked->right->right, marked->right);
            //it = it->right;
        }
        
    }

    //4) восстанавить инварианты
    marked->parent->restoreInvariants();

    //5) удаляю 
    delete marked;
}
