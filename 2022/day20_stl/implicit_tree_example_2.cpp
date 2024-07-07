#include <iostream>
#include <cstdio>
#include <cassert>
#include <iterator>
#include <vector>
#include <stack>
#include <ext/pb_ds/assoc_container.hpp> // Общий файл. 
#include <ext/pb_ds/tree_policy.hpp> // Содержит класс tree_order_statistics_node_update
#include <ext/pb_ds/detail/standard_policies.hpp>

using namespace std;
using namespace __gnu_cxx;
using namespace __gnu_pbds;

typedef int Type;

bool ProxyLess(int, int);
struct MyLess : public binary_function<int, int, bool>
{
    bool operator()(const int& __x, const int& __y) const { 
        return ProxyLess( __x, __y);
    }
};

int new_pos_;
int new_id_;
int offset_;
bool is_erase = false;
typedef tree<
    int,
    Type,
    MyLess, // std::less<int>,
    rb_tree_tag,
    tree_order_statistics_node_update>
        InnerImplicitTree;
InnerImplicitTree tree_;
InnerImplicitTree::node_iterator ptr_;
vector<InnerImplicitTree::node_iterator> nodes_;

stack<int> holes_;
int NewId() {
    if (holes_.empty()) {
        return nodes_.size();
    }
    else {
        int tmp = holes_.top();
        holes_.pop();
        return tmp;
    }
}
void FreeId(int id) {
    nodes_[id] = tree_.node_end();
    holes_.push(id);
}

bool ProxyLessLeft(int new_id, InnerImplicitTree::node_iterator rhs) {
    int sub_pos = 0;
    if (rhs.get_l_child() != tree_.node_end()) {
        sub_pos = rhs.get_l_child().get_metadata();
    }
    if (new_pos_ - offset_ < sub_pos) {
        return true;
    } else if (new_pos_ - offset_ == sub_pos) {
        offset_ += sub_pos;
        return false;
    } else {
        offset_ += sub_pos + 1;
        return false;
    }
    assert(false);
    return false;
}

bool ProxyLessRight(InnerImplicitTree::node_iterator lhs, int new_id) {
    int sub_pos = 0;
    assert(lhs != tree_.node_end());
    if (lhs.get_l_child() != tree_.node_end()) {
        sub_pos = lhs.get_l_child().get_metadata();
    }
    if (new_pos_ - offset_ < sub_pos) {
        return false;
    } else if (new_pos_ - offset_ == sub_pos) {
        offset_ += sub_pos;
        return false;
    } else {
        offset_ += sub_pos + 1;
        return true;
    }
    assert(false);
    return false;
}

bool ProxyLess(int lhs, int rhs) {
    // assert(!is_erase);
    if (lhs == new_id_) {
        if (!is_erase)
            return true;
        return ProxyLessLeft(lhs, nodes_[rhs]);
    }
    else if (rhs == new_id_){
        // if (is_erase)
        //     return true;
        return  ProxyLessRight(nodes_[lhs], rhs);
    }
    assert(false);
    return false;
}

InnerImplicitTree::node_iterator Find(int pos) {
    InnerImplicitTree::node_iterator iter = tree_.node_begin();
    while (iter != tree_.node_end()) {
        int offset = 0;
        if (iter.get_l_child() != tree_.node_end()) {
            offset = iter.get_l_child().get_metadata();
        }
        if (pos < offset) {
            iter = iter.get_l_child();
        } else if (pos == offset) {
            return iter;
        } else {
            pos -= offset + 1;
            iter = iter.get_r_child();
        }
    }
    assert (false);
    return tree_.node_end();
}

void Insert(int pos, Type element) {
    offset_ = 0;
    new_id_ = NewId();
    new_pos_ = pos;
    ptr_ = tree_.node_begin();
    // search_state_ = SearchState::insert;
    auto result = tree_.insert({new_id_, element});
   // assert(result.second);
    //assert(result.first.m_p_nd == Find(pos).m_p_nd);
    nodes_.push_back(Find(pos));
    new_id_ = -1;
}

void Remove(int pos) {
    // op_pos_ = pos;
    // // nodes_.push_back(new_id_);
    // search_state_ = SearchState::insert;
    is_erase = true;
    offset_ = 0;
    new_pos_ = pos;
    new_id_= AtKey(pos);
    tree_.erase(new_id_);
    FreeId(new_id_);
    //nodes_[new_id_] = tree_.node_end();
    is_erase = false;
}

const int AtKey(int pos) {
    // search_state_ = find;
    auto result = (tree_.find_by_order(pos));
    return result->first;
}

const Type& AtValue(int pos) {
    // search_state_ = find;
    auto result = (tree_.find_by_order(pos));
    return result->second;
}


int OOK(Type key) {
    new_id_ = key;
    offset_ = 0;
    new_pos_ = -1;
    is_ook = true;
    return tree_.order_of_key(key);
    is_ook = false;
    new_id_ = -1;
}

size_t Size() {
    return tree_.size();
}

void Print() {
    bool is_first = true;
    cout << "{";
    for(int i = 0; i < Size(); i++) {
        if(!is_first) {
            cout << ", ";
        }
        is_first = false;
        cout << At(i);
    }
    cout << "}" << endl;
}
void PrintTreeInner(InnerImplicitTree::node_iterator it) {
    if (it == tree_.node_end()) {
        return;
    }
    static string indent = "+---";
    cout << indent << it.get_metadata() << "(" << it.m_p_nd->m_value.second << ")" << endl;
    indent = "        " + indent;
    PrintTreeInner(it.get_l_child());
    PrintTreeInner(it.get_r_child());
    indent = indent.substr(8);
}

void PrintTree() {
    PrintTreeInner(tree_.node_begin());
}

void testImplicitTree() {
    cout <<  "test implicit tree" << endl;
    for(int i = 0; i < 20; i++) {
        Insert(i, i);
        Print();
    }
    PrintTree();

    // check key orders
    // for(int i = 0; i < 10; i++) {
    //     cout << OOK(i) << " ";
    // }
    // cout << endl;

    Insert(10, 21);
    Print();
    Insert(5, 22);
    Print();
    Insert(15, 23);
    Print();
    Insert(21, 25);
    Print();
    Insert(0, 26);
    PrintTree();
    Print();

    Remove(4);
    Print(); // -9

    Remove(4);
    Print(); // -4

    Remove(5);
    Print(); // -5

    Remove(0);
    Print(); // -0

    Insert(0, 10); 
    Print(); // {10...

    Insert(0, 11); 
    Print(); // {11...

    Insert(7, 12); 
    Print(); // ...12} 

    Insert(4, 13); 
    Print(); 

    Insert(2, 14); 
    Print(); 

    Insert(4, 15); 
    Print(); 
}

int main(void) {
    testImplicitTree();
}