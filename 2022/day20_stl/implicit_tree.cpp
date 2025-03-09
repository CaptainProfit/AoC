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

typedef long long Type;

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
    nodes_[id] = tree_.node_end();
    holes_.push(id);
}
InnerImplicitTree::node_iterator GetNodeIterator(int pos);
void UpdateId(int pos) {
    auto iter = GetNodeIterator(pos);
    if (comp_id_ == nodes_.size()) {
        nodes_.push_back(iter);
    }
    else {
        nodes_[comp_id_] = iter;
    }
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
    _GLIBCXX_DEBUG_ASSERT(false); // not tested with this debug things
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

int GetIteratorPosition(InnerImplicitTree::node_iterator iter) {
    int pos = 0;
    if (iter.get_l_child() != tree_.node_end()) {
        pos += iter.get_l_child().get_metadata();
    }
    auto mpnd_iter = iter.m_p_nd;
    while (mpnd_iter->m_p_parent != nullptr) {
        if (mpnd_iter->m_p_parent->m_p_right == mpnd_iter) {
            if (mpnd_iter->m_p_parent->m_p_left != nullptr) {
                pos += mpnd_iter->m_p_parent->m_p_left->m_metadata + 1;
            }
            else {
                pos += 1;
            }
        }
        mpnd_iter = mpnd_iter->m_p_parent;
        if (mpnd_iter == tree_.node_begin().m_p_nd) {
            break;
        }
    }
    return pos;
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
    UpdateId(pos);
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

void Clear() {
    while (Size() != 0) {
        Remove(0);
    }
}

void Print() {
    bool is_first = true;
    cout << "[";
    for(int i = 0; i < Size(); i++) {
        if(!is_first) {
            cout << ", ";
        }
        is_first = false;
        // cout << ValueAtPos(i);
        cout << Find(i);
    }
    cout << "]" << endl;
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
