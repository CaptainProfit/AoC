#include <iostream>
#include <cstdio>
#include <cassert>
#include <ext/pb_ds/assoc_container.hpp> // Общий файл. 
#include <ext/pb_ds/tree_policy.hpp> // Содержит класс tree_order_statistics_node_update
#include <ext/pb_ds/detail/standard_policies.hpp>

using namespace std;
using namespace __gnu_cxx;
using namespace __gnu_pbds;

typedef tree<
    int,
    null_type,
    less<int>,
    rb_tree_tag,
    tree_order_statistics_node_update>
        ordered_set;

void testOrderedSet() {
    cout <<  "test ordered set" << endl;
    ordered_set X;
    X.insert(1);
    X.insert(2);
    X.insert(4);
    X.insert(8);
    X.insert(16);
    
    auto a = X.find_by_order(1);
    cout << a.m_p_nd->m_value << endl;
    
    assert(X.find_by_order(1).m_p_nd->m_value == 2);
    assert(X.find_by_order(2).m_p_nd->m_value == 4);
    assert(X.find_by_order(4).m_p_nd->m_value == 16);
    assert(end(X)==X.find_by_order(6));

    assert(X.order_of_key(-5) == 0);
    assert(X.order_of_key(1) == 0);
    assert(X.order_of_key(3) == 2);
    assert(X.order_of_key(4) == 2);
    assert(X.order_of_key(400) == 5);
    cout <<  "ordered set OK" << endl;
}

int main(void) {
    testOrderedSet();
}