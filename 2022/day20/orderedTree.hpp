//[UTF-8 en/ru]
#pragma once
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <new>
#include <iomanip>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

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
    int offset = 1;
    void updateOffset(int value);
    public:
    class cTreeNode{
        public:
        T value;
        size_t iter_index;

        //инвариант - size = 1 + left->size + right->size
        //пустые деревья имеют размер ноль.
        //инвариант abs(left->height - right->height) < 2
        int size = 0; // размер поддерева из этой вершины, для задачи.
        int height = 0; // для балансировки AVL
        cTreeNode* restoreInvariants(void);
        void calculateInvariant(void);
        int checkBalance();		

        // методы балансировки
        void rotateLeft();
        void rotateRight();
        
        public:	
        cTreeNode* parent = nullptr;
        cTreeNode* left = nullptr;
        cTreeNode* right = nullptr;

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

        const size_t getIndex(){
            cTreeNode* x = this;
            if (x == nullptr) {
                return 0;
            }
            size_t index = x->left->getSize();
            while (x->parent != nullptr) {
                if (x->parent->right == x) {
                    index += x->parent->left->getSize() + 1;
                }
                x = x->parent;
            }
            return index;
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
    vector<cTreeNode*> values;

    public:	
    //интерфейс
    cContainer(){
        root = nullptr;
    };
    
    int sizef(void){ return root->getSize(); }
    //T& operator[](int index);
    cTreeNode* operator[](int index);

    void insert(int index, const T& value, size_t iter_index);
    void remove(int index);

    //places copy of value to the end
    void move(const T& i) {
        cTreeNode* node = values[i];
        int pos = node->getIndex();
        int value = node->getValue();
        int new_pos = (pos + value) % (sizef() - 1);
        new_pos += sizef() - 1;
        new_pos %= sizef() - 1; 
        remove(pos);
        cout << "after remove" << endl;
        print();
        insert(new_pos, value, i);
        values[i] = (*this)[new_pos];
    }
    void push_back(const T& value) {
        insert(sizef(), value, sizef());
        values.push_back((*this)[sizef() - 1]);
    }
    void print() {
        printTree();
        printSimple();
    }
    void printSimple(){
        cout << "[";
        if (root) {
            for (int i = 0; i < sizef(); i++) {
                if (i != 0) {
                    cout <<", ";
                }
                cout << (*this)[i]->getValue();
            }
        }
        cout << "]" << endl;
    }
    struct cNodeMarker{
        cTreeNode* ptr;
        int level;
        bool isMarked;
    };
    void printLine(int len) {
        cout << "+";
        for (int i = 0; i < len; i++) {
            cout << "-";
        }
        cout << "+" << endl;
    }
    void printTree(){
        int offset = 2;
        offset += 2;
        vector<string> canvas;
        string skip2(offset, ' ');
        stack<cNodeMarker> stack_node;
        stack_node.push({root, 0, false});
        int index = 0;
        while(!stack_node.empty()) {
            auto [ptr, level, isMarked] = stack_node.top();
            if (ptr == nullptr) {
                stack_node.pop();
                continue;
            }
            if (!isMarked) {
                stack_node.top().isMarked = true;
                stack_node.push({ptr->left, level + 1, false});
                continue;
            }
            stack_node.pop();
            stack_node.push({ptr->right, level + 1, false});
            for (size_t i = canvas.size(); i <= level; i++) {
                canvas.emplace_back(offset*index, ' ');
            }
            for (size_t i = 0; i < canvas.size(); i++) {
                if ( i == level) {
                    stringstream ss;
                    ss << setw(offset-2) << ptr->value;
                    canvas[i] += " " + ss.str() + " ";
                }
                else {
                    canvas[i] += skip2;
                }
            }
            index ++;
        }
        if (canvas.empty()) {
            cout << "||" << endl;
            return;
        }
        printLine(canvas[0].length());
        for (int i = 0; i < canvas.size(); i++) {
            cout << "|";
            
            cout << canvas[i];
            cout << "|" << endl;
        }
        printLine(canvas[0].length());
    }
};
