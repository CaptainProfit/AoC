#include <iostream>
#include <cstdio>

#include <random>
#include <cstdlib>

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

typedef enum{none, find, insert} CompState;
CompState comp_state_ = none;
int comp_pos_;
int comp_id_;
int comp_offset_;
typedef tree<
    int,
    Type,
    MyLess, // std::less<int>,
    rb_tree_tag,
    tree_order_statistics_node_update>
        InnerImplicitTree;
InnerImplicitTree tree_;
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
    // nodes_[id] = tree_.node_end();
    holes_.push(id);
}

size_t Size() {
    return tree_.size();
}

bool ProxyLessLeft(int new_id, InnerImplicitTree::node_iterator rhs) {
    int sub_pos = 0;
    if (rhs.get_l_child() != tree_.node_end()) {
        sub_pos = rhs.get_l_child().get_metadata();
    }
    if (comp_pos_ - comp_offset_ < sub_pos) {
        return true;
    } else {
        comp_offset_ += sub_pos + 1;
        return false;
    }
    assert(false);
    return false;
}

bool ProxyLessRight(InnerImplicitTree::node_iterator lhs) {
    int sub_pos = 0;
    assert(lhs != tree_.node_end());
    auto lchild = lhs.get_l_child();
    auto finish = tree_.node_end();
    if (lchild != tree_.node_end()) {
        sub_pos = lchild.get_metadata();
    }
    if (sub_pos < comp_pos_ - comp_offset_) {
        comp_offset_ += sub_pos + 1;
        return true;
    } else {        
        return false;
    }
    assert(false);
    return false;
}

bool ProxyLess(int lhs, int rhs) {
    if (lhs == rhs) {
        return false;
    }
    if (lhs == comp_id_) {
        if (comp_state_ == CompState::insert) 
            return true; //not correct every time
        return ProxyLessLeft(lhs, nodes_[rhs]);
    }
    else if (rhs == comp_id_) {
        return  ProxyLessRight(nodes_[lhs]);
    }
    assert(false);
    return false;
}

InnerImplicitTree::node_iterator GetNodeIterator(int pos) {
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
void CompClear() {
    comp_state_ = CompState::none;
    comp_offset_ = 0;
    comp_pos_ = -1;
    comp_id_ = -1;
}
void CompSet(int pos, int key, CompState comp) {
    comp_state_ = comp;
    comp_offset_ = 0;
    comp_pos_ = pos;
    comp_id_ = key;
}

const int KeyAtPos(int pos) {
    auto result = (tree_.find_by_order(pos));
    return result->first;
}

const Type& ValueAtPos(int pos) {
    auto result = (tree_.find_by_order(pos));
    return result->second;
}

void Insert(int pos, Type element) {
    CompSet(pos, NewId(), CompState::insert);
    auto result = tree_.insert({comp_id_, element});
    nodes_.push_back(GetNodeIterator(pos));
    CompClear();
}

const Type& Find(int pos) {
    CompSet(pos, KeyAtPos(pos), CompState::find);
    auto it = tree_.find(comp_id_);
    CompClear();
    if (it != tree_.end()) {
        return it->second;
    }
}

void Remove(int pos) {
    CompSet(pos, KeyAtPos(pos), CompState::find);
    tree_.erase(comp_id_);
    FreeId(comp_id_);
    CompClear();
}


// int OOK(Type key) {
//     CompSet(-1, key, CompState::ook);
//     int result = tree_.order_of_key(key);
//     CompClear();
//     return result;
// }

void Clear() {
    while (Size() != 0) {
        Remove(0);
    }
}

void Print() {
    bool is_first = true;
    cout << "{";
    for(int i = 0; i < Size(); i++) {
        if(!is_first) {
            cout << ", ";
        }
        is_first = false;
        // cout << ValueAtPos(i);
        cout << Find(i);
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


class naiveSolution{
    vector<Type> naive_buf_;
    int naive_size_ = 0;
public:
    Type Find(int pos) {
        return naive_buf_[pos];
    }

    void Insert(int pos, const Type& value) {
        if (naive_size_ >= naive_buf_.size()) {
            naive_buf_.resize(1 + 2*naive_size_);
        }
        for (int i = naive_size_; i > pos;  i--) {
            naive_buf_[i] = naive_buf_[i - 1];
        }
        naive_buf_[pos] = value;
        naive_size_++;
    }

    void Remove(int pos) {
        for (int i = pos; i < naive_size_ - 1; i++) {
            naive_buf_[i] = naive_buf_[i + 1];
        }
        naive_size_--;
    }
    int Size() {
        return naive_size_;
    }
    
    void Clear() {
        naive_size_ = 0;
    }
    //template<typename Contaier>
    //bool CompareWith (Contaier& ctr) {
        //if (ctr.Find(i) != Find(i)) {
    void Print() {
        bool is_first = true;
        cout << "{";
        for(int i = 0; i < Size(); i++) {
            if(!is_first) {
                cout << ", ";
            }
            is_first = false;
            // cout << ValueAtPos(i);
            cout << naive_buf_[i];
        }
        cout << "}!" << endl;
    }

    bool CompareWith () {
        if (Size() != ::Size()) {
            return false;
        }
        for (int i = 0; i < Size(); i++) {
            if (Find(i) != ::Find(i)) {
                Print();
                return false;
            }
        }
        return true;
    }
};


void TestImplicitTree() {
    cout <<  "test implicit tree" << endl;
    naiveSolution test;
    for(int i = 0; i < 10; i++) {
        Insert(i, 100 + i);
        test.Insert(i, 100 + i);
        Print();
        assert (test.CompareWith());
    }
    PrintTree();

    // check key orders
    // for(int i = 0; i < 10; i++) {
    //     cout << OOK(i) << " ";
    // }
    // cout << endl;1,3,5,7,9,11
    for (int i  = 0; i < 5; i ++) {
        Insert(1 + 2*i, 100 + 10 + i);
        test.Insert(1 + 2*i, 100 + 10 + i);
        Print();
        assert (test.CompareWith());
    }
    // {0, 10, 1, 11, 2, 12, 3, 13, 4, 14, 5, 6, 7, 8, 9}

    for (int i  = 0; i < 5; i ++) {
        test.Insert(Size() - 2*i, 100 + 20 + i); 
        Insert(Size() - 2*i, 100 + 20 + i); 
        Print();
        assert (test.CompareWith());
    }
    // {0, 10, 1, 11, 2, 12, 3, 13, 4, 14, 5, 24, 6, 23, 7, 22, 8, 21, 9, 20}

    Insert(10, 100 + 31);
    test.Insert(10, 100 + 31);
    Print();
    assert(test.CompareWith());
    Insert(5, 100 + 32);
    test.Insert(5, 100 + 32);
    Print();
    assert(test.CompareWith());
    Insert(15, 100 + 33);
    test.Insert(15, 100 + 33);
    Print();
    assert(test.CompareWith());
    Insert(21, 100 + 34);
    test.Insert(21, 100 + 34);
    Print();
    assert(test.CompareWith());
    Insert(0, 100 + 35);
    test.Insert(0, 100 + 35);
    Print();
    assert(test.CompareWith());
    Insert(4, 100 + 36); 
    test.Insert(4, 100 + 36); 
    Print(); 
    assert(test.CompareWith());
    test.Insert(Size() - 1, 100 + 37);
    Insert(Size() - 1, 100 + 37);
    Print(); 
    assert(test.CompareWith());
    test.Insert(Size(), 100 + 38);
    Insert(Size(), 100 + 38);
    Print(); 
    assert(test.CompareWith());

    PrintTree(); 
    Print(); 

    Remove(0);
    test.Remove(0);
    Print(); // -35
    assert(test.CompareWith());
    
    Remove(4);
    test.Remove(4);
    Print(); // -11
    assert(test.CompareWith());
    
    Remove(10);
    test.Remove(10);
    Print(); // -4
    assert(test.CompareWith());

    Remove(3);
    test.Remove(3);
    Print(); // -
    assert(test.CompareWith());

    Clear();
    test.Clear();
    assert(test.CompareWith());
    cout << "Hand test passed" << endl;
}
void TestImplicitTreePlaceAndClear() {
    for (int i = 0; i < 3; i++) {
        naiveSolution test;
        for (int i  = 0; i < 100; i ++) {
            Insert(i, 200 + i);
            test.Insert(i, 200 + i);
            Print();
            assert (test.CompareWith());
        }
        Clear();
        test.Clear();
        assert(test.CompareWith());

        for (int i  = 0; i < 100; i ++) {
            Insert(0, 300 + i);
            test.Insert(0, 300 + i);
            Print();
            assert (test.CompareWith());
        }
        Clear();
        test.Clear();
        assert(test.CompareWith());
        
        for (int i  = 0; i < 100; i ++) {
            int pos = (i*i)%Size();
            Insert(pos, 300 + i);
            test.Insert(pos, 300 + i);
            Print();
            assert (test.CompareWith());
        }
        Clear();
        test.Clear();
        assert(test.CompareWith());
    }
}
void TestImplicitTreeParam (int op_size, int est_size) {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib1(0, 100);
    std::uniform_int_distribution<> distrib2(1, 6);
    static int i = 0;
    Type new_val = 0;
    naiveSolution test;
    for (int j = 0; j < 3; j++) {
        cout <<  "parametrized test implicit tree " << i << "with " << op_size << "operations and " << est_size << "estimated size of end size" << endl;
        for (int i = 0; i < op_size; i++) {
            int random_value = (std::rand() << 16) + std::rand();
            bool is_insert = distrib1(gen)*2*op_size > est_size + op_size;
            if (is_insert || Size() == 0) {
                int pos = random_value%(test.Size() + 1);
                test.Insert(pos, new_val);
                Insert(pos, new_val);
                new_val++;
            } else {
                int pos = random_value%(test.Size());
                test.Remove(pos);
                Remove(pos);
            }
            if (!test.CompareWith()) {
                cout << "Test Error while";
                if (is_insert) {
                    cout << "inserting";
                } else {
                    cout << "removing";
                }
                cout << endl;
                cout << "got: ";
                Print();
                assert (false);
            }
        }
        test.Clear();
        Clear();
        if (!test.CompareWith()) {
            cout << "Test Error while clearing" << endl;
            cout << "waited:  ";
            test.Print();
            cout << "but got: ";
            Print();
            assert (false);
        }
        cout << "Test passed" << endl;
    }
}

void TestImplicitTreeHuge() {
    for (int i = 0; i < 10; i++) {
        TestImplicitTreeParam(100, 100);
        TestImplicitTreeParam(1000, 100);
        TestImplicitTreeParam(1000, 500);
        TestImplicitTreeParam(100000, 5000);
    }
}

int main(void) {
    TestImplicitTree();
    TestImplicitTreePlaceAndClear();
    TestImplicitTreeHuge();
    cout << "All Tests passed" << endl;
}