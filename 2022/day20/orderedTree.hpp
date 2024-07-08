//[UTF-8 en/ru]
#pragma once
#include <cstdlib>
#include <algorithm>
#include <new>
#include <iostream>
#include <vector>

#define ull unsigned long long

using namespace std;

typedef int T;
//template<typename T>
class cContainer{
    // структура хранит набор элементов типа Т
    // и поддерживает операции:
    // 1)поиска по индексу за log(N)
    // 2)определение индекса по указателю на элемент в структуре.
    // 3)вставки после индекса за log(N)
    // 4)вставки в конец за log(N)
    // 5)удаления по индексу за log(N)
    // реализация через AVL дерево
    // признак корня - он сам себе парент.
    // признак пустого дерева - парент нулевой.
    public:
    class cTreeNode{
        public:
        T value;

        //инвариант - size = 1 + left->size + right->size
        //пустые деревья имеют размер ноль.
        //инвариант abs(left->height - right->height) < 2
        int size = 0; // размер поддерева из этой вершины, для задачи.
        int height = 0; // для балансировки AVL
        void restoreInvariants(void);
        void calculateInvariant(void);
        int checkBalance();		

        // методы балансировки
        void rotateLeft();
        void rotateRight();
        void rotateLeftDouble();
        void rotateRightDouble();
        
        public:	
        cTreeNode* parent = nullptr;
        cTreeNode* left = nullptr;
        cTreeNode* right = nullptr;

        void createChild(T newVal, bool isLeft){
            cTreeNode* newChild = new (cTreeNode);
            newChild->value = newVal;
            if(isLeft){
                removeChild(left);			
                left = newChild;
                left->parent = this;
            }
            else{
                removeChild(right);			
                right = newChild;
                right->parent = this;
            }
        }

        void removeChild(bool isLeft){
            if(isLeft){
                if(left != nullptr){
                    left->removeChild(left);
                    left->removeChild(right);
                }
                delete left;
                left = nullptr;
            }
            else{
                if(right != nullptr){
                    right->removeChild(left);
                    right->removeChild(right);
                }
                delete right;
                right = nullptr;
            }
        }

        ostream& print(ostream& os){
            if(left == nullptr && right == nullptr){
                os << value << " ";
                return os;
            }

            if(left != nullptr)
                left->print(os);
            else
                os << "nil ";
            os << value << " ";		
            if(right != nullptr)
                right->print(os);
            else
                os << "nil ";
            return os;
        }
        bool isRoot(void){ return parent == nullptr; }

        bool isLeaf(void){ return left == nullptr && right == nullptr; }
        
        const T& getValue(){
            return value;
        }

        void setValue(const T& t){
            value = t;
        }

        long long getSize(){
            if (this == nullptr) {
                return 0;
            }
            return size;
        }	
    };

    cTreeNode* root = nullptr;
    bool isEmpty(void){	return root == nullptr; }
    vector<T> values;

    public:	
    //интерфейс
    cContainer(){
        root = nullptr;
    };
    
    int sizef(void){ return root->getSize(); }
    //T& operator[](int index);
    cTreeNode* operator[](int index);

    void insert(int index, const T& value);
    void remove(int index);

    //places copy of value to the end
    void move(const T& i) {
        int pos = root->find(i);
    }
    void push_back(const T& value) {
        values.push_back(value);
        insert(this->sizef(), value);
    }
    void print(){

    }
};
